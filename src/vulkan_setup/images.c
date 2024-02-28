#include "images.h"
#include "util.h"
#include "vulkan_setup.h"

void getSwapchainImages(const VulkanState *const s, uint32_t *o_num_images, VkImage **o_images) {
    *o_num_images = 0;

    vkGetSwapchainImagesKHR(s->device, s->swapchain, o_num_images, NULL);
    *o_images = emalloc(*o_num_images * sizeof(VkImage));
    handleVkError("Failed to get swapchain images", 
        vkGetSwapchainImagesKHR(s->device, s->swapchain, o_num_images, *o_images)
    );
}


VkImageView *createSwapchainImageViews(const VulkanState *const s) {
    VkImageView *image_views = emalloc(s->num_swapchain_images * sizeof(VkImageView));

    for(uint32_t i = 0; i < s->num_swapchain_images; i++) {
        VkImageViewCreateInfo createInfo = {0};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = s->swapchain_images[i];

        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = s->swapchain_image_format;
    
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        handleVkError("Failed to create swapchain image view", 
            vkCreateImageView(s->device, &createInfo, NULL, &image_views[i])
        );
    }

    return image_views;
}

void destroyImageViews(const VulkanState *const s, VkImageView *views) {
    for(uint32_t i = 0; i < s->num_swapchain_images; i++) {
        vkDestroyImageView(s->device, views[i], NULL);
    }

    free(views);
}
