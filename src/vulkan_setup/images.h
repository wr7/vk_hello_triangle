#pragma once

#include "vulkan_setup.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

VkImage *getSwapchainImages(const VulkanState *const s, uint32_t *o_num_images);
VkImageView *createSwapchainImageViews(const VulkanState *const s);
void destroyImageViews(const VulkanState *const s, VkImageView *views);

