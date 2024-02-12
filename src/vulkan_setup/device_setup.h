#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

VkPhysicalDevice selectGraphicsCard(VkInstance instance);
