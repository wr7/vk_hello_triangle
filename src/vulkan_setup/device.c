#include "vulkan_setup/device.h"

#include "util.h"
#include "vulkan_setup.h"
#include "vulkan_setup/graphics_card.h"
#include <stdio.h>
#include <vulkan/vk_enum_string_helper.h>

VkDevice createLogicalDevice(VkPhysicalDevice graphics_card, VkQueue *graphics_queue) {
    QueueFamilyIndices indices = findQueueFamilies(graphics_card);

    VkDeviceQueueCreateInfo queueCreateInfo = {0};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphics_family.value;
    queueCreateInfo.queueCount = 1;

    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    VkPhysicalDeviceFeatures deviceFeatures = {0};

    VkDeviceCreateInfo createInfo = {0};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;

    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount = 0;

    if (ENABLE_VALIDATION_LAYERS) {
        createInfo.enabledLayerCount = ARRAY_LENGTH(VALIDATION_LAYERS);
        createInfo.ppEnabledLayerNames = VALIDATION_LAYERS;
    } else {
        createInfo.enabledLayerCount = 0;
    }

    VkDevice device;

    VkResult result = vkCreateDevice(graphics_card, &createInfo, NULL, &device);
    if(result != VK_SUCCESS) {
        fprintf(stderr, "Failed to create logical vulkan device: %s\n", string_VkResult(result));
        exit(result);
    }

    vkGetDeviceQueue(device, indices.graphics_family.value, 0, graphics_queue);

    return device;
}
