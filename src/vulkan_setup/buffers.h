#pragma once

#include "vulkan_setup.h"

VkBuffer createVertexAndIndexBuffer(const VulkanState *const s, VkDeviceMemory *const o_buffer_memory);
