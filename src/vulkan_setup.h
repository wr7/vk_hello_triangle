#pragma once

#include "vulkan_setup/queues.h"
#include <stdbool.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

typedef struct {
    VkInstance instance;
    VkSurfaceKHR window_surface;
    VkPhysicalDevice graphics_card;
    VkDevice device;
    VkSwapchainKHR swapchain;

    Queues queues;
    QueueFamilyIndices indices;

    // Swapchain stuff //

    uint32_t num_swapchain_images;
    VkImage *swapchain_images;

    VkExtent2D swapchain_extent;
    VkFormat swapchain_image_format;
} VulkanState;

static const char *const VALIDATION_LAYERS[] = {
    "VK_LAYER_KHRONOS_validation"
};
extern const bool ENABLE_VALIDATION_LAYERS;

VulkanState VulkanState_create(GLFWwindow *window);
void VulkanState_destroy(VulkanState s);
