#include "swapchain.h"
#include "util.h"

#include <vulkan/vulkan_core.h>

SwapchainSupportDetails SwapchainSupportDetails_create(const VkPhysicalDevice device, const VkSurfaceKHR surface) {
    SwapchainSupportDetails details = {0};

    handleVkError("Failed to query swapchain capabilities", 
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities)
    );

    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &details.num_formats, NULL);

    details.formats = emalloc(details.num_formats * sizeof(*details.formats));

    handleVkError("Failed to query device surface formats",
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &details.num_formats, details.formats)
    );

    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &details.num_present_modes, NULL);

    details.present_modes = emalloc(details.num_present_modes * sizeof(*details.present_modes));

    handleVkError("Failed to query device surface present modes", 
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &details.num_present_modes, details.present_modes)
    );

    return details;
}

bool SwapchainSupportDetails_is_adequate(const SwapchainSupportDetails *const details) {
    return details->num_formats != 0 && details->num_present_modes != 0;
}

void SwapchainSupportDetails_free(const SwapchainSupportDetails *const details) {
    if(details == NULL) {
        return;
    }

    free(details->formats);
    free(details->present_modes);
}
