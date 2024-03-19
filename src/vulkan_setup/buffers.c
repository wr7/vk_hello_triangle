#include "string.h"
#include "vulkan/vulkan_core.h"
#include "vulkan_setup.h"
#include "main_window.h"
#include "util.h"

static VkBuffer createBuffer(const VulkanState *const s, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkDeviceMemory  *const o_buffer_memory);
static void writeToMemory(const VulkanState *const s, VkDeviceMemory memory, const void *const src, VkDeviceSize offset, VkDeviceSize num_bytes);
static void transferBetweenBuffers(const VulkanState *const s, VkBuffer src, VkBuffer dst, VkDeviceSize num_bytes);

VkBuffer createVertexAndIndexBuffer(const VulkanState *const s, VkDeviceMemory *const o_buffer_memory) {
    UNUSED(VERTICES); UNUSED(VERTEX_INDICES); // To silence warnings in main_window.h where VERTICES_AND_INDICES lives
    const uint32_t BUFFER_SIZE = sizeof(VERTICES_AND_INDICES);

    VkDeviceMemory staging_memory;
    const VkBuffer staging_buffer = createBuffer(s, BUFFER_SIZE, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT, &staging_memory);

    writeToMemory(s, staging_memory, &VERTICES_AND_INDICES, 0, BUFFER_SIZE);

    const VkBuffer vertex_buffer = createBuffer(s, 
        BUFFER_SIZE,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, 
        o_buffer_memory
    );

    transferBetweenBuffers(s, staging_buffer, vertex_buffer, (VkDeviceSize) BUFFER_SIZE);

    vkDestroyBuffer(s->device, staging_buffer, NULL);
    vkFreeMemory(s->device, staging_memory, NULL);

    return vertex_buffer;
}

static void transferBetweenBuffers(const VulkanState *const s, VkBuffer src, VkBuffer dst, VkDeviceSize num_bytes) {
    VkCommandBufferAllocateInfo alloc_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandPool = s->transient_command_pool,
        .commandBufferCount = 1,
    };

    VkCommandBuffer command_buffer;
    handleVkError("Failed to create transfer command buffer",
        vkAllocateCommandBuffers(s->device, &alloc_info, &command_buffer)
    );

    VkCommandBufferBeginInfo beginInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
    };

    handleVkError("Failed to begin command buffer",
        vkBeginCommandBuffer(command_buffer, &beginInfo)
    );

    VkBufferCopy copyRegion = {
        .srcOffset = 0, // Optional
        .dstOffset = 0, // Optional
        .size = num_bytes,
    };

    vkCmdCopyBuffer(command_buffer, src, dst, 1, &copyRegion);

    handleVkError("Failed to end command buffer",
        vkEndCommandBuffer(command_buffer)
    );

    VkSubmitInfo submitInfo = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .commandBufferCount = 1,
        .pCommandBuffers = &command_buffer,
    };

    vkQueueSubmit(s->queues.graphics_queue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(s->queues.graphics_queue);

    vkFreeCommandBuffers(s->device, s->transient_command_pool, 1, &command_buffer);
}

static void writeToMemory(const VulkanState *const s, VkDeviceMemory memory, const void *const src, VkDeviceSize offset, VkDeviceSize num_bytes) {
    void *buffer_data;

    handleVkError("Failed to map vertex buffer memory",
        vkMapMemory(s->device, memory, offset, num_bytes, 0, &buffer_data)
    );

    memcpy(buffer_data, src, num_bytes);

    handleVkError("Failed to flush buffer memory", 
        vkFlushMappedMemoryRanges(s->device, 1,
            &(VkMappedMemoryRange) {
                .sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
                .memory = memory,
                .offset = offset,
                .size = num_bytes,
            }
        )
    );

    vkUnmapMemory(s->device, memory);
}

static uint32_t findMemoryType( const VulkanState *const s, uint32_t typeFilter, VkMemoryPropertyFlags properties);

static VkBuffer createBuffer(const VulkanState *const s, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkDeviceMemory  *const o_buffer_memory) {
    VkBufferCreateInfo buffer_info = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = size,
        .usage = usage,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };

    VkBuffer buffer;
    handleVkError("Failed to create buffer",
        vkCreateBuffer(s->device, &buffer_info, NULL, &buffer)
    );

    VkMemoryRequirements mem_requirements;
    vkGetBufferMemoryRequirements(s->device, buffer, &mem_requirements);

    VkMemoryAllocateInfo alloc_info = {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize = mem_requirements.size,
        .memoryTypeIndex = findMemoryType(s, mem_requirements.memoryTypeBits, properties),
    };

    handleVkError("Failed to allocate buffer memory",
        vkAllocateMemory(s->device, &alloc_info, NULL, o_buffer_memory)
    );

    handleVkError("Failed to bind buffer memory",
        vkBindBufferMemory(s->device, buffer, *o_buffer_memory, 0)
    );

    return buffer;
}

static uint32_t findMemoryType( const VulkanState * const s, const uint32_t typeFilter, const VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(s->graphics_card, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    error("Failed to find suitable memory type!");
}
