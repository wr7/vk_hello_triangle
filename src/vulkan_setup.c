#include <stdint.h>
#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>

#include <vulkan/vk_enum_string_helper.h>
#include <vulkan/vulkan_core.h>

#include "vulkan_setup.h"

#include "src/util.h"
#include "vulkan_setup/device.h"
#include "vulkan_setup/graphics_card.h"
#include "vulkan_setup/queues.h"
#include "vulkan_setup/swapchain.h"

#ifdef NDEBUG
const bool ENABLE_VALIDATION_LAYERS = false;
#else
const bool ENABLE_VALIDATION_LAYERS = true;
#endif

static VkInstance createVulkanInstance(void);
static VkSurfaceKHR createWindowSurface(VkInstance instance, GLFWwindow *window);

VulkanState VulkanState_create(GLFWwindow *window) {
    VulkanState s = {0};

    s.instance = createVulkanInstance();
    s.window_surface = createWindowSurface(s.instance, window);

    s.graphics_card = selectGraphicsCard(s.instance, s.window_surface, &s.indices);
    s.device = createLogicalDevice(s.graphics_card, &s.indices);
    s.queues = Queues_create(s.device, &s.indices);

    s.swapchain = createSwapchain(&s, window);

    return s;
}

void VulkanState_destroy(VulkanState s) {
    vkDestroySwapchainKHR(s.device, s.swapchain, NULL);
    vkDestroyDevice(s.device, NULL);
    vkDestroySurfaceKHR(s.instance, s.window_surface, NULL);
    vkDestroyInstance(s.instance, NULL);
}

static VkInstance createVulkanInstance(void) {
    VkApplicationInfo appInfo = {0};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    VkInstanceCreateInfo createInfo = {0};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    
    if(ENABLE_VALIDATION_LAYERS) {
        createInfo.enabledLayerCount = ARRAY_LENGTH(VALIDATION_LAYERS);
        createInfo.ppEnabledLayerNames = VALIDATION_LAYERS;
    } else {
        createInfo.enabledLayerCount = 0;
    }

    VkInstance instance = NULL;

    VkResult result = vkCreateInstance(&createInfo, NULL, &instance);
    handleVkError("Failed to initialize vulkan", result);

    return instance;
}

static VkSurfaceKHR createWindowSurface(VkInstance instance, GLFWwindow *window) {
    VkSurfaceKHR surface;

    VkResult result = glfwCreateWindowSurface(instance, window, NULL, &surface);
    handleVkError("Failed to create window surface", result);

    return surface;
}
