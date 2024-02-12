#pragma once

#include <stdbool.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

typedef struct {
    VkInstance instance;
    VkPhysicalDevice graphics_card;
    VkDevice device;
    VkQueue graphics_queue;
} VulkanState;

static const char *const VALIDATION_LAYERS[] = {
    "VK_LAYER_KHRONOS_validation"
};
extern const bool ENABLE_VALIDATION_LAYERS;

VulkanState VulkanState_create(void);
void VulkanState_destroy(VulkanState s);
