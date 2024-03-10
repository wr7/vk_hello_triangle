#include "vulkan_setup.h"
#include "util.h"

#include "render_pass.h"

VkRenderPass createRenderPass(const VulkanState *const s) {
    VkAttachmentDescription colorAttachment = {
        .format = s->swapchain_image_format,
        .samples = VK_SAMPLE_COUNT_1_BIT,
        .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
        .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
        .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
        .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
        .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
    };

    VkSubpassDependency dependency = {
        .srcSubpass = VK_SUBPASS_EXTERNAL,
        .dstSubpass = 0,
        .srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .srcAccessMask = 0,

        .dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        .dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
    };

    // Sub-passes and color attachment references //
    VkAttachmentReference colorAttachmentRef = {
        .attachment = 0, // Corresponds to location=0 in shader
        .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
    };

    VkSubpassDescription subpass = {
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .colorAttachmentCount = 1,
        .pColorAttachments = &colorAttachmentRef,
    };

    VkRenderPass render_pass;

    VkRenderPassCreateInfo renderPassInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,

        .attachmentCount = 1,
        .pAttachments = &colorAttachment,

        .subpassCount = 1,
        .pSubpasses = &subpass,

        .dependencyCount = 1,
        .pDependencies = &dependency,
    };

    handleVkError("Failed to create render pass", 
        vkCreateRenderPass(s->device, &renderPassInfo, NULL, &render_pass)
    );

    return render_pass;
}
