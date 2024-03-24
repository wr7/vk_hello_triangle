#pragma once

#include "vulkan_setup.h"

VkDescriptorSetLayout createDescriptorSetLayout(const VulkanState *const s);
VkDescriptorPool createDescriptorPool(const VulkanState *const s);
void createDescriptorSets(const VulkanState *const s, VkDescriptorSet (*const o_sets)[FRAMES_IN_FLIGHT]);
