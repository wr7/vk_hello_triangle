#include "command_buffer.h"
#include "util.h"
#include "vulkan_setup.h"

VkCommandPool createCommandPool(const VulkanState *const s) {
    VkCommandPoolCreateInfo poolInfo = {0};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = s->indices.graphics_family.value;

    VkCommandPool pool;

    handleVkError("Failed to create command pool", 
        vkCreateCommandPool(s->device, &poolInfo, NULL, &pool)
    );

    return pool;
}

VkCommandBuffer createCommandBuffer(const VulkanState *const s) {
    VkCommandBufferAllocateInfo allocInfo = {0};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = s->command_pool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer command_buffer;

    handleVkError("Failed to create command buffer", 
        vkAllocateCommandBuffers(s->device, &allocInfo, &command_buffer)
    );

    return command_buffer;
}
