#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

typedef struct {
    VkInstance instance;
    VkPhysicalDevice graphics_card;
} VulkanState;

VulkanState VulkanState_create(void);
void VulkanState_destroy(VulkanState s);
