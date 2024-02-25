#include "swapchain.h"
#include "util.h"
#include "vulkan_setup.h"
#include "vulkan_setup/queues.h"
#include <vulkan/vulkan_core.h>

static VkExtent2D selectSwapExtent(const SwapchainSupportDetails *const details, GLFWwindow *const window);
static VkSurfaceFormatKHR selectSwapSurfaceFormat(const SwapchainSupportDetails *const details);
static VkPresentModeKHR selectSwapPresentMode(const SwapchainSupportDetails *const details);

VkSwapchainKHR createSwapchain(const VulkanState *const s, GLFWwindow *window) {
    const SwapchainSupportDetails details = SwapchainSupportDetails_create(s->graphics_card, s->window_surface);

    VkSurfaceFormatKHR surfaceFormat = selectSwapSurfaceFormat(&details);
    VkPresentModeKHR presentMode = selectSwapPresentMode(&details);
    VkExtent2D extent = selectSwapExtent(&details, window);

    uint32_t image_count = details.capabilities.minImageCount + 1;

    if (details.capabilities.maxImageCount > 0 && image_count > details.capabilities.maxImageCount) {
        image_count = details.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {0};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = s->window_surface;

    createInfo.minImageCount = image_count;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    uint32_t needed_indices[] = {s->indices.graphics_family.value, s->indices.presentation_family.value};

    if (s->indices.graphics_family.value != s->indices.presentation_family.value) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = needed_indices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0; // Optional
        createInfo.pQueueFamilyIndices = NULL; // Optional
    }

    createInfo.preTransform = details.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    VkSwapchainKHR swapchain;
    handleVkError("Failed to create swapchain", vkCreateSwapchainKHR(s->device, &createInfo, NULL, &swapchain));

    SwapchainSupportDetails_free(&details);

    return swapchain;
}

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

static VkSurfaceFormatKHR selectSwapSurfaceFormat(const SwapchainSupportDetails *const details) {
    for(size_t i = 0; i < details->num_formats; i++) {
        VkSurfaceFormatKHR format  = details->formats[i];
        if(format.format == VK_FORMAT_B8G8R8A8_SRGB && format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return format;
        }
    }

    return details->formats[0];
}

static VkPresentModeKHR selectSwapPresentMode(const SwapchainSupportDetails *const details) {
    for(size_t i = 0; i < details->num_present_modes; i++) {
        VkPresentModeKHR mode = details->present_modes[i];
        if(mode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return VK_PRESENT_MODE_MAILBOX_KHR;
        }
    }

    // Fallback Value //

    return VK_PRESENT_MODE_FIFO_KHR;
}

static VkExtent2D selectSwapExtent(const SwapchainSupportDetails *const details, GLFWwindow *const window) {
    const VkSurfaceCapabilitiesKHR capabilities = details->capabilities;

    if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    }
    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    VkExtent2D actual_extent = {width,height};

    actual_extent.width = uint32_t_clamp(actual_extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    actual_extent.height = uint32_t_clamp(actual_extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

    return actual_extent;
}
