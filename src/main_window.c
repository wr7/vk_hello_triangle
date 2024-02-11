#include "main_window.h"
#include "util.h"

#include <vulkan/vk_enum_string_helper.h>

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <vulkan/vulkan_core.h>

const uint32_t WINDOW_WIDTH = 800;
const uint32_t WINDOW_HEIGHT = 600;

const char *const VALIDATION_LAYERS[] = {
    "VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool ENABLE_VALIDATION_LAYERS = true;
#endif

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
    if(result != VK_SUCCESS) {
        fprintf(stderr, "Failed to initialize Vulkan: %s\n", string_VkResult(result));
        exit(result);
    }

    return instance;
}

static void MainWindow_selectGraphicsCard(MainWindow *w) {
    uint32_t num_devices = 0;
    VkResult result = vkEnumeratePhysicalDevices(w->instance, &num_devices, NULL);
    if(result != VK_SUCCESS) {
        fprintf(stderr, "Failed to query graphics cards: %s\n", string_VkResult(result));
        exit(result);
    }

    if(num_devices == 0) {
        fprintf(stderr, "Failed to find suitable Vulkan graphics card.\n");
        exit(1);
    }

    VkPhysicalDevice *availableDevices = emalloc(num_devices * sizeof(VkPhysicalDevice));
    vkEnumeratePhysicalDevices(w->instance, &num_devices, availableDevices);

    w->graphics_card = availableDevices[0];

    free(availableDevices);
}

static void MainWindow_initVulkan(MainWindow *w) {
    w->instance = createVulkanInstance();
    MainWindow_selectGraphicsCard(w);
}

MainWindow MainWindow_create(void) {
    MainWindow w = {0};

    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    w.window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Vulkan Window", NULL, NULL);

    MainWindow_initVulkan(&w);

    return w;
}

void MainWindow_run(MainWindow *w) {
    while (!glfwWindowShouldClose(w->window)) {
        glfwPollEvents();
    }
}

void MainWindow_destroy(MainWindow *w) {
    vkDestroyInstance(w->instance, NULL);
    
    glfwDestroyWindow(w->window);
    glfwTerminate();
}

