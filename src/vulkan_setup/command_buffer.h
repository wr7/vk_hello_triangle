#pragma once

#include "vulkan_setup.h"

void recordCommandBuffer(const VulkanState *const s, uint32_t imageIndex);

VkCommandPool createCommandPool(const VulkanState *const s);
VkCommandBuffer createCommandBuffer(const VulkanState *const s);
