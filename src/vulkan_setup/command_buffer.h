#pragma once

#include "vulkan_setup.h"

void recordCommandBuffer(const VulkanState *const s, uint32_t imageIndex);
CommandBufferInfo CommandBufferInfo_create(const VulkanState *const s);
VkCommandPool createCommandPool(const VulkanState *const s);
