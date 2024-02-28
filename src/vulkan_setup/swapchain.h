#pragma once

#include "vulkan_setup.h"
#include <stdbool.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

typedef struct {
    VkSurfaceCapabilitiesKHR capabilities;

    VkSurfaceFormatKHR *formats;
    uint32_t num_formats;

    VkPresentModeKHR *present_modes;
    uint32_t num_present_modes;
} SwapchainSupportDetails;

VkSwapchainKHR createSwapchain(const VulkanState *const s, VkExtent2D *o_swapchain_extent, VkFormat *o_swapchain_image_format, 
                               GLFWwindow *window);

SwapchainSupportDetails SwapchainSupportDetails_create(const VkPhysicalDevice device, const VkSurfaceKHR surface);
bool SwapchainSupportDetails_is_adequate(const SwapchainSupportDetails *const details);
void SwapchainSupportDetails_free(const SwapchainSupportDetails *const details);
