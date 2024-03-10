#include "images.h"
#include "util.h"
#include "vulkan_setup.h"

VkImage *getSwapchainImages(const VulkanState *const s, uint32_t *o_num_images) {
    *o_num_images = 0;

    handleVkError("Failed to get swapchain images", 
        vkGetSwapchainImagesKHR(s->device, s->swapchain, o_num_images, NULL)
    );

    VkImage *images = emalloc(*o_num_images * sizeof(VkImage));

    handleVkError("Failed to get swapchain images", 
        vkGetSwapchainImagesKHR(s->device, s->swapchain, o_num_images, images)
    );

    return images;
}


VkImageView *createSwapchainImageViews(const VulkanState *const s) {
    VkImageView *image_views = emalloc(s->num_swapchain_images * sizeof(VkImageView));

    for(uint32_t i = 0; i < s->num_swapchain_images; i++) {
        VkImageViewCreateInfo createInfo = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image = s->swapchain_images[i],

            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = s->swapchain_image_format,
    
            .components.r = VK_COMPONENT_SWIZZLE_IDENTITY,
            .components.g = VK_COMPONENT_SWIZZLE_IDENTITY,
            .components.b = VK_COMPONENT_SWIZZLE_IDENTITY,
            .components.a = VK_COMPONENT_SWIZZLE_IDENTITY,

            .subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .subresourceRange.baseMipLevel = 0,
            .subresourceRange.levelCount = 1,
            .subresourceRange.baseArrayLayer = 0,
            .subresourceRange.layerCount = 1,
        };

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
