#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include<stdbool.h>

#include <vulkan/vk_enum_string_helper.h>
#include <vulkan/vulkan_core.h>

#include "vulkan_setup/graphics_card.h"

#include "util.h"

VkPhysicalDevice selectGraphicsCard(VkInstance instance, VkSurfaceKHR window_surface, QueueFamilyIndices *out_queue_indices) {
    uint32_t num_devices = 0;
    VkResult result = vkEnumeratePhysicalDevices(instance, &num_devices, NULL);
    if(result != VK_SUCCESS) {
        fprintf(stderr, "Failed to query graphics cards: %s\n", string_VkResult(result));
        exit(result);
    }

    VkPhysicalDevice *available_devices = emalloc(num_devices * sizeof(VkPhysicalDevice));
    vkEnumeratePhysicalDevices(instance, &num_devices, available_devices);

    for(uint32_t card_num = 0; card_num < num_devices; card_num++) {
        VkPhysicalDevice device = available_devices[card_num];

        QueueFamilyIndices families = findQueueFamilies(device, window_surface);

        if(QueueFamilyIndices_has_minimum_requirements(&families)) {
            *out_queue_indices = families;

            free(available_devices);
            return device;
        }
    }

    fprintf(stderr, "Failed to find suitable Vulkan graphics card.\n");
    exit(1);
}

QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR window_surface) {
    QueueFamilyIndices indices = {0};

    uint32_t num_queue_families = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &num_queue_families, NULL);


    VkQueueFamilyProperties *queue_families = emalloc(num_queue_families * sizeof(VkQueueFamilyProperties));
    vkGetPhysicalDeviceQueueFamilyProperties(device, &num_queue_families, queue_families);

    for(uint32_t i = 0; i < num_queue_families; i++) {
        VkQueueFamilyProperties family = queue_families[i];

        if(!indices.graphics_family.present) {
            if (family.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphics_family = OptionalU32_of(i);
            }
        }

        if(!indices.presentation_family.present) {
            VkBool32 present_support = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, window_surface, &present_support);
            if(present_support) {
                indices.presentation_family = OptionalU32_of(i);
            }
        }
        
    }

    free(queue_families);

    return indices;
}

