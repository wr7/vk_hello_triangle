#include <stdint.h>
#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>

#include <vulkan/vk_enum_string_helper.h>

#include "vulkan_setup.h"

#include "src/util.h"

const char *const VALIDATION_LAYERS[] = {
    "VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
const bool ENABLE_VALIDATION_LAYERS = false;
#else
const bool ENABLE_VALIDATION_LAYERS = true;
#endif


VkInstance createVulkanInstance(void) {
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
    if(result != VK_SUCCESS) {
        fprintf(stderr, "Failed to initialize Vulkan: %s\n", string_VkResult(result));
        exit(result);
    }

    return instance;
}
