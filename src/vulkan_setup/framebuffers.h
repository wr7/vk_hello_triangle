#pragma once

#include "vulkan_setup.h"

VkFramebuffer *createFramebuffers(const VulkanState *const s);
void destroyFramebuffers(const VulkanState *const s, VkFramebuffer *frame_buffers);

