#pragma once

#include "vulkan_setup.h"

VkBuffer createVertexAndIndexBuffer(const VulkanState *const s, VkDeviceMemory *const o_buffer_memory);
VkBuffer createUniformBuffer(const VulkanState *const s, VkDeviceMemory *const o_buffer_memory, UniformBufferObject (**const o_mapped_memory)[FRAMES_IN_FLIGHT]);
void destroyUniformBuffer(const VulkanState *const s);
