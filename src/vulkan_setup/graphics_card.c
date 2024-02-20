#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include<stdbool.h>

#include <string.h>
#include <vulkan/vk_enum_string_helper.h>
#include <vulkan/vulkan_core.h>

#include "vulkan_setup/graphics_card.h"

#include "util.h"
#include "vulkan_setup/queues.h"

const char *const REQUIRED_EXTENTIONS[] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

const size_t NUM_REQUIRED_EXTENTIONS = ARRAY_LENGTH(REQUIRED_EXTENTIONS);


static bool hasRequiredExtentions(VkPhysicalDevice device) {
    uint32_t num_extentions = 0;
    vkEnumerateDeviceExtensionProperties(device, NULL, &num_extentions, NULL);

    VkExtensionProperties *extentions = malloc(num_extentions * sizeof(VkExtensionProperties));

    VkResult result = vkEnumerateDeviceExtensionProperties(device, NULL, &num_extentions, extentions);
    handleVkError("Failed to query device properties", result);

    bool found_all_extentions = true;

    for(size_t r = 0; r < NUM_REQUIRED_EXTENTIONS; r++) {
        bool found_extention = false;
        for(size_t i = 0; i < num_extentions; i++) {
            if(strcmp(REQUIRED_EXTENTIONS[r], extentions[i].extensionName) == 0) {
                found_extention = true;
                break;
            }
        }

        if(!found_extention) {
            found_all_extentions = false;
            break;
        }
    }

    free(extentions);

    return found_all_extentions;
}

static bool isDeviceSuitable(const VkPhysicalDevice device, const QueueFamilyIndices *const indices) {
    if(!QueueFamilyIndices_has_minimum_requirements(indices)) {
        return false;
    } else if(!hasRequiredExtentions(device)) {
        return false;
    } else {
        return true;
    }
}

VkPhysicalDevice selectGraphicsCard(VkInstance instance, VkSurfaceKHR window_surface, QueueFamilyIndices *out_queue_indices) {
    uint32_t num_devices = 0;
    vkEnumeratePhysicalDevices(instance, &num_devices, NULL);

    VkPhysicalDevice *available_devices = emalloc(num_devices * sizeof(VkPhysicalDevice));

    VkResult result = vkEnumeratePhysicalDevices(instance, &num_devices, available_devices);
    handleVkError("Failed to query graphics card", result);

    for(uint32_t card_num = 0; card_num < num_devices; card_num++) {
        VkPhysicalDevice device = available_devices[card_num];

        QueueFamilyIndices families = findQueueFamilies(device, window_surface);

        if(isDeviceSuitable(device, &families)) {
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

