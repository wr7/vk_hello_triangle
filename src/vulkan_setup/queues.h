#pragma once

#include<stdbool.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "util.h"

typedef struct {
    VkQueue graphics_queue;
    VkQueue presentation_queue;
} Queues;

typedef struct {
    OptionalU32 graphics_family;
    OptionalU32 presentation_family;
} QueueFamilyIndices ;

// Queues //
Queues Queues_create(VkDevice device, const QueueFamilyIndices *const indices);

// QueueFamilyIndices //
size_t QueueFamilyIndices_num_unique_indices(const QueueFamilyIndices *const indices);
OptionalU32 QueueFamilyIndices_next_unique_index(const QueueFamilyIndices *const indices, const OptionalU32 previous_index);

bool QueueFamilyIndices_has_minimum_requirements(const QueueFamilyIndices *const indices);
