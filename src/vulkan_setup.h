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
    Queues queues;
} VulkanState;

static const char *const VALIDATION_LAYERS[] = {
    "VK_LAYER_KHRONOS_validation"
};
extern const bool ENABLE_VALIDATION_LAYERS;

VulkanState VulkanState_create(GLFWwindow *window);
void VulkanState_destroy(VulkanState s);
