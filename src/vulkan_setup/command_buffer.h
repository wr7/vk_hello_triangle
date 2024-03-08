#pragma once

#include "vulkan_setup.h"

VkCommandPool createCommandPool(const VulkanState *const s);
VkCommandBuffer createCommandBuffer(const VulkanState *const s);
