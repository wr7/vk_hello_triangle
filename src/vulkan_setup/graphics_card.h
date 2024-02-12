#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "util.h"

typedef struct {
    OptionalU32 graphics_family;
} QueueFamilyIndices ;

VkPhysicalDevice selectGraphicsCard(VkInstance instance);
QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
