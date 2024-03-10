#pragma once
#include "vulkan_setup.h"

VkSemaphore createSemaphore(const VulkanState *const s);
VkFence createFence(const VulkanState *const s);
