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
        VkDeviceQueueCreateInfo queue_create_info = {0};
        queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_info.queueFamilyIndex = unique_index.value;
        queue_create_info.queueCount = 1;
        queue_create_info.pQueuePriorities = &queue_priority;
        queue_create_infos[i] = queue_create_info;

        i++;
    }

    VkPhysicalDeviceFeatures deviceFeatures = {0};

    VkDeviceCreateInfo createInfo = {0};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.pQueueCreateInfos = queue_create_infos;
    createInfo.queueCreateInfoCount = num_unique_queues;

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = NUM_REQUIRED_EXTENTIONS;
    createInfo.ppEnabledExtensionNames = REQUIRED_EXTENTIONS;

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
