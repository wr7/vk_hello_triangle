#include "queues.h"

Queues Queues_create(VkDevice device, const QueueFamilyIndices *const indices) {
    Queues queues = {0};

    vkGetDeviceQueue(device, indices->graphics_family.value, 0, &queues.graphics_queue);
    vkGetDeviceQueue(device, indices->presentation_family.value, 0, &queues.presentation_queue);

    return queues;
}

bool QueueFamilyIndices_has_minimum_requirements(const QueueFamilyIndices *const indices) {
    return indices->graphics_family.present && indices->presentation_family.present;
}

size_t QueueFamilyIndices_num_unique_indices(const QueueFamilyIndices *const indices) {
    uint32_t num_unique_indices = 0;

    for(
        OptionalU32 unique_index = QueueFamilyIndices_next_unique_index(indices, OptionalU32_empty()); 
        unique_index.present;
        unique_index = QueueFamilyIndices_next_unique_index(indices, unique_index)
    ) {
        num_unique_indices += 1;
    }

    return num_unique_indices;
}

OptionalU32 QueueFamilyIndices_next_unique_index(const QueueFamilyIndices *const indices, const OptionalU32 previous_index) {
    const uint32_t num_indices = sizeof(*indices)/sizeof(OptionalU32);
    const OptionalU32 *const pindices = (OptionalU32 *) indices;

    for(uint32_t i = 0; i < num_indices; i++) {
        OptionalU32 index = pindices[i];
        if(!index.present) {
            continue;
        }
        if(index.value > previous_index.value || !previous_index.present) {
            return OptionalU32_of(index.value);
        }
    }
    
    return OptionalU32_empty();
}
