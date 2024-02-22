#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "vulkan_setup/queues.h"

extern const char *const REQUIRED_EXTENTIONS[];

extern const size_t NUM_REQUIRED_EXTENTIONS;

VkPhysicalDevice selectGraphicsCard(const VkInstance instance, const VkSurfaceKHR window_surface, QueueFamilyIndices *const out_queue_indices);
QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice device, const VkSurfaceKHR window_surface);
