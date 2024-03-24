#include "command_buffer.h"
#include "draw.h"
#include "util.h"
#include "vulkan/vulkan_core.h"
#include "vulkan_setup.h"
#include "vulkan_setup/synchronization.h"
#include <stddef.h>

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

VkCommandPool createTransientCommandPool(const VulkanState *const s) {
    VkCommandPoolCreateInfo pool_info = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT,
        .queueFamilyIndex = s->indices.graphics_family.value,
    };

    VkCommandPool pool;
    handleVkError("Failed to create command pool", 
        vkCreateCommandPool(s->device, &pool_info, NULL, &pool)
    );

    return pool;
}

void recordCommandBuffer(const VulkanState *const s, const uint32_t imageIndex) {
    const VkCommandBuffer command_buffer = s->command_buffer_infos[s->current_command_buffer_info].buffer;

    VkCommandBufferBeginInfo beginInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = 0, // Optional
        .pInheritanceInfo = NULL, // Optional
    };

    handleVkError("Failed to record command buffer", 
        vkBeginCommandBuffer(command_buffer, &beginInfo)
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

    vkCmdBeginRenderPass(command_buffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, s->pipeline);

    VkBuffer vertexBuffers[] = {s->vertex_and_index_buffer};
    VkDeviceSize offsets[] = {VERTEX_BUFFER_OFFSET};
    vkCmdBindVertexBuffers(command_buffer, 0, 1, vertexBuffers, offsets);

    vkCmdBindIndexBuffer(command_buffer, s->vertex_and_index_buffer, INDEX_BUFFER_OFFSET, VK_INDEX_TYPE_UINT16);

    VkViewport viewport = {
        .x = 0.0f,
        .y = 0.0f,
        .width = (float) s->swapchain_extent.width,
        .height = (float) s->swapchain_extent.height,
        .minDepth = 0.0f,
        .maxDepth = 1.0f,
    };

    vkCmdSetViewport(command_buffer, 0, 1, &viewport);

    VkRect2D scissor = {.offset = (VkOffset2D){0, 0},.extent = s->swapchain_extent};
    vkCmdSetScissor(command_buffer, 0, 1, &scissor);

    vkCmdBindDescriptorSets(command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, s->pipeline_layout, 0, 1, &s->descriptor_sets[s->current_command_buffer_info], 0, NULL);
    vkCmdDrawIndexed(command_buffer, ARRAY_LENGTH(VERTEX_INDICES), 1, 0, 0, 0);
    vkCmdEndRenderPass(command_buffer);

    handleVkError("Failed to record command buffer",
        vkEndCommandBuffer(command_buffer)
    );
}

static VkCommandBuffer createCommandBuffer(const VulkanState *const s);

CommandBufferInfo CommandBufferInfo_create(const VulkanState *const s) {
    return (CommandBufferInfo) {
        .buffer = createCommandBuffer(s),
        .render_finished_semaphore = createSemaphore(s),
        .image_available_semaphore = createSemaphore(s),
        .in_flight_fence = createFence(s),
    };
}

static VkCommandBuffer createCommandBuffer(const VulkanState *const s) {
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
