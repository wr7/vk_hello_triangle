#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "vulkan_setup/queues.h"

VkPhysicalDevice selectGraphicsCard(VkInstance instance, VkSurfaceKHR window_surface, QueueFamilyIndices *out_queue_indices);
QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR window_surface);
