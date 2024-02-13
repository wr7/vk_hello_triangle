#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "queues.h"

VkDevice createLogicalDevice(VkPhysicalDevice graphics_card, const QueueFamilyIndices *const indices);
