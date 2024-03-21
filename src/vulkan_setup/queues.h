#pragma once

#include<stdbool.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "util.h"

/**
 * Iterates over the unique queue family indices in `indices`. Stores the current unique index in `var`.
 */
#define QueueFamilyIndices_foreach_unique(var, indices) \
    for(\
        OptionalU32 var = QueueFamilyIndices_next_unique_index(indices, OptionalU32_empty());\
        var.present;\
        var = QueueFamilyIndices_next_unique_index(indices, var))

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
pure size_t QueueFamilyIndices_num_unique_indices(const QueueFamilyIndices *const indices);
pure OptionalU32 QueueFamilyIndices_next_unique_index(const QueueFamilyIndices *const indices, const OptionalU32 previous_index);

pure bool QueueFamilyIndices_has_minimum_requirements(const QueueFamilyIndices *const indices);
