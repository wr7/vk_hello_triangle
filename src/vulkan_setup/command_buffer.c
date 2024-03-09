#include "command_buffer.h"
#include "util.h"
#include "vulkan/vulkan_core.h"
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

void recordCommandBuffer(const VulkanState *const s, uint32_t imageIndex) {
    VkCommandBufferBeginInfo beginInfo = {0};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0; // Optional
    beginInfo.pInheritanceInfo = NULL; // Optional

    handleVkError("Failed to record command buffer", 
        vkBeginCommandBuffer(s->command_buffer, &beginInfo)
    );

    VkRenderPassBeginInfo renderPassInfo = {0};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = s->render_pass;
    renderPassInfo.framebuffer = s->frame_buffers[imageIndex];

    renderPassInfo.renderArea.offset = (VkOffset2D){0, 0};
    renderPassInfo.renderArea.extent = s->swapchain_extent;

    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;

    vkCmdBeginRenderPass(s->command_buffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(s->command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, s->pipeline);

    VkViewport viewport = {0};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float) s->swapchain_extent.width;
    viewport.height = (float) s->swapchain_extent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    vkCmdSetViewport(s->command_buffer, 0, 1, &viewport);

    VkRect2D scissor = {0};
    scissor.offset = (VkOffset2D){0, 0};
    scissor.extent = s->swapchain_extent;
    vkCmdSetScissor(s->command_buffer, 0, 1, &scissor);

    vkCmdDraw(s->command_buffer, 3, 1, 0, 0);
    vkCmdEndRenderPass(s->command_buffer);

    handleVkError("Failed to record command buffer",
        vkEndCommandBuffer(s->command_buffer)
    );
}
