#include <stdio.h>

#include <vulkan/vk_enum_string_helper.h>

#include "util.h"
#include "vulkan_setup.h"
#include "vulkan_setup/graphics_card.h"
#include "vulkan_setup/device.h"

VkDevice createLogicalDevice(VkPhysicalDevice graphics_card, const QueueFamilyIndices *const indices) {
    const size_t num_unique_queues = QueueFamilyIndices_num_unique_indices(indices);

    VkDeviceQueueCreateInfo *queue_create_infos = emalloc(num_unique_queues * sizeof(VkDeviceQueueCreateInfo));

    float queue_priority = 1.0f;

    size_t i = 0;
    for(
        OptionalU32 unique_index = QueueFamilyIndices_next_unique_index(indices, OptionalU32_empty()); 
        unique_index.present;
        unique_index = QueueFamilyIndices_next_unique_index(indices, unique_index)
    ) {
        queue_create_infos[i] = (VkDeviceQueueCreateInfo){
            .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
            .queueFamilyIndex = unique_index.value,
            .queueCount = 1,
            .pQueuePriorities = &queue_priority,
        };
        i++;
    }

    VkPhysicalDeviceFeatures deviceFeatures = {0};

    VkDeviceCreateInfo createInfo = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,

        .pQueueCreateInfos = queue_create_infos,
        .queueCreateInfoCount = num_unique_queues,

        .pEnabledFeatures = &deviceFeatures,

        .enabledExtensionCount = NUM_REQUIRED_EXTENTIONS,
        .ppEnabledExtensionNames = REQUIRED_EXTENTIONS,
    };

    if (ENABLE_VALIDATION_LAYERS) {
        createInfo.enabledLayerCount = ARRAY_LENGTH(VALIDATION_LAYERS);
        createInfo.ppEnabledLayerNames = VALIDATION_LAYERS;
    } else {
        createInfo.enabledLayerCount = 0;
    }

    VkDevice device;

    VkResult result = vkCreateDevice(graphics_card, &createInfo, NULL, &device);
    handleVkError("Failed to create logical vulkan device", result);

    free(queue_create_infos);

    return device;
}
