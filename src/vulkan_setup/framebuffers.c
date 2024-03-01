#include "util.h"
#include "vulkan_setup.h"
#include <vulkan/vulkan_core.h>
VkFramebuffer *createFramebuffers(const VulkanState *const s) {
    VkFramebuffer *frame_buffers = emalloc(s->num_swapchain_images * sizeof(VkFramebuffer));

    for (uint32_t i = 0; i < s->num_swapchain_images; i++) {
        VkImageView attachments[] = {
            s->swapchain_image_views[i]
        };

        VkFramebufferCreateInfo framebufferInfo = {0};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = s->render_pass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = s->swapchain_extent.width;
        framebufferInfo.height = s->swapchain_extent.height;
        framebufferInfo.layers = 1;

        handleVkError("Failed to create frame buffer", 
            vkCreateFramebuffer(s->device, &framebufferInfo, NULL, &frame_buffers[i])
        );
    }

    return frame_buffers;
}

void destroyFramebuffers(const VulkanState *const s, VkFramebuffer *frame_buffers) {
    for(uint32_t i = 0; i < s->num_swapchain_images; i++) {
        vkDestroyFramebuffer(s->device, frame_buffers[i], NULL);
    }

    free(frame_buffers);
}
