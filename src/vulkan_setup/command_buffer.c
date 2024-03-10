#include "command_buffer.h"
#include "util.h"
#include "vulkan/vulkan_core.h"
#include "vulkan_setup.h"

VkCommandPool createCommandPool(const VulkanState *const s) {
    VkCommandPoolCreateInfo poolInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = s->indices.graphics_family.value,
    };

    VkCommandPool pool;

    handleVkError("Failed to create command pool", 
        vkCreateCommandPool(s->device, &poolInfo, NULL, &pool)
    );

    return pool;
}

VkCommandBuffer createCommandBuffer(const VulkanState *const s) {
    VkCommandBufferAllocateInfo allocInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = s->command_pool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 1,
    };

    VkCommandBuffer command_buffer;

    handleVkError("Failed to create command buffer", 
        vkAllocateCommandBuffers(s->device, &allocInfo, &command_buffer)
    );

    return command_buffer;
}

void recordCommandBuffer(const VulkanState *const s, uint32_t imageIndex) {
    VkCommandBufferBeginInfo beginInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = 0, // Optional
        .pInheritanceInfo = NULL, // Optional
    };

    handleVkError("Failed to record command buffer", 
        vkBeginCommandBuffer(s->command_buffer, &beginInfo)
    );

    VkRenderPassBeginInfo renderPassInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .renderPass = s->render_pass,
        .framebuffer = s->frame_buffers[imageIndex],

        .renderArea.offset = (VkOffset2D){0, 0},
        .renderArea.extent = s->swapchain_extent,

        .clearValueCount = 1,
        .pClearValues = &(VkClearValue){{{0.0f, 0.0f, 0.0f, 1.0f}}},
    };

    vkCmdBeginRenderPass(s->command_buffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(s->command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, s->pipeline);

    VkViewport viewport = {
        .x = 0.0f,
        .y = 0.0f,
        .width = (float) s->swapchain_extent.width,
        .height = (float) s->swapchain_extent.height,
        .minDepth = 0.0f,
        .maxDepth = 1.0f,
    };

    vkCmdSetViewport(s->command_buffer, 0, 1, &viewport);

    VkRect2D scissor = {.offset = (VkOffset2D){0, 0},.extent = s->swapchain_extent};
    vkCmdSetScissor(s->command_buffer, 0, 1, &scissor);

    vkCmdDraw(s->command_buffer, 3, 1, 0, 0);
    vkCmdEndRenderPass(s->command_buffer);

    handleVkError("Failed to record command buffer",
        vkEndCommandBuffer(s->command_buffer)
    );
}
