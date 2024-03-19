#include "string.h"
#include "vulkan/vulkan_core.h"
#include "vulkan_setup.h"
#include "main_window.h"
#include "util.h"

static uint32_t findMemoryType( const VulkanState *const s, uint32_t typeFilter, VkMemoryPropertyFlags properties);

VkBuffer createVertexBuffer(const VulkanState *const s, VkDeviceMemory *const o_buffer_memory) {
    VkBufferCreateInfo buffer_info = {
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = sizeof(VERTICES),
        .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
    };

    VkBuffer vertex_buffer;

    handleVkError("Failed to create vertex buffer",
        vkCreateBuffer(s->device, &buffer_info, NULL, &vertex_buffer)
    );

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(s->device, vertex_buffer, &memRequirements);

    VkMemoryAllocateInfo alloc_info = {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize = memRequirements.size,
        .memoryTypeIndex = findMemoryType(s,
            memRequirements.memoryTypeBits,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        ),
    };

    handleVkError("Failed to allocate vertex buffer memory", 
        vkAllocateMemory(s->device, &alloc_info, NULL, o_buffer_memory)
    );

    handleVkError("Failed to bind vertex buffer memory",
        vkBindBufferMemory(s->device, vertex_buffer, *o_buffer_memory, 0)
    );

    // Write vertices to buffer //

    void *buffer_data;

    handleVkError("Failed to map vertex buffer memory",
        vkMapMemory(s->device, *o_buffer_memory, 0, sizeof(VERTICES), 0, &buffer_data)
    );

    memcpy(buffer_data, &VERTICES[0], sizeof(VERTICES));

    handleVkError("Failed to flush vertex buffer memory", 
        vkFlushMappedMemoryRanges(s->device, 1,
            &(VkMappedMemoryRange) {
                .sType = VK_STRUCTURE_TYPE_MAPPED_MEMORY_RANGE,
                .memory = *o_buffer_memory,
                .offset = 0,
                .size = sizeof(VERTICES),
            }
        )
    );

    vkUnmapMemory(s->device, *o_buffer_memory);

    return vertex_buffer;
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
