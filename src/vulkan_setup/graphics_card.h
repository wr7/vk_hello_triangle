#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "vulkan_setup/queues.h"

extern const char *const REQUIRED_EXTENTIONS[];

extern const size_t NUM_REQUIRED_EXTENTIONS;

VkPhysicalDevice selectGraphicsCard(VkInstance instance, VkSurfaceKHR window_surface, QueueFamilyIndices *out_queue_indices);
QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR window_surface);
