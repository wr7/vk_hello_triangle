#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

VkDevice createLogicalDevice(VkPhysicalDevice graphics_card);
