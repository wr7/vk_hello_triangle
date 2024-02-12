#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include<stdbool.h>

#include <vulkan/vk_enum_string_helper.h>

#include "vulkan_setup/device_setup.h"

#include "util.h"

static bool isDeviceSuitable(VkPhysicalDevice device) {
    QueueFamilyIndices families = findQueueFamilies(device);

    if(families.graphicsFamily.present) {
        return true;
    } else {
        return false;
    }
}

VkPhysicalDevice selectGraphicsCard(VkInstance instance) {
    uint32_t num_devices = 0;
    VkResult result = vkEnumeratePhysicalDevices(instance, &num_devices, NULL);
    if(result != VK_SUCCESS) {
        fprintf(stderr, "Failed to query graphics cards: %s\n", string_VkResult(result));
        exit(result);
    }

    VkPhysicalDevice *available_devices = emalloc(num_devices * sizeof(VkPhysicalDevice));
    vkEnumeratePhysicalDevices(instance, &num_devices, available_devices);

    VkPhysicalDevice selected_device = NULL;

    for(uint32_t i = 0; i < num_devices; i++) {
        VkPhysicalDevice device = available_devices[i];

        if(isDeviceSuitable(device)) {
            selected_device = device;
            break;
        }
    }

    free(available_devices);

    if(selected_device == NULL) {
        fprintf(stderr, "Failed to find suitable Vulkan graphics card.\n");
        exit(1);
    }

    return selected_device;
}

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) {
    QueueFamilyIndices indices = {0};

    uint32_t num_queue_families = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &num_queue_families, NULL);


    VkQueueFamilyProperties *queue_families = emalloc(num_queue_families * sizeof(VkQueueFamilyProperties));
    vkGetPhysicalDeviceQueueFamilyProperties(device, &num_queue_families, queue_families);

    for(uint32_t i = 0; i < num_queue_families; i++) {
        VkQueueFamilyProperties family = queue_families[i];

        if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = OptionalU32_of(i);
        }
    }

    free(queue_families);

    return indices;
}
