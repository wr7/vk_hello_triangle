#include <stdint.h>
#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>

#include <vulkan/vk_enum_string_helper.h>
#include <vulkan/vulkan_core.h>

#include "GLFW/glfw3.h"
#include "vulkan_setup.h"

#include "src/util.h"
#include "vulkan_setup/command_buffer.h"
#include "vulkan_setup/device.h"
#include "vulkan_setup/graphics_card.h"
#include "vulkan_setup/images.h"
#include "vulkan_setup/pipeline.h"
#include "vulkan_setup/queues.h"
#include "vulkan_setup/render_pass.h"
#include "vulkan_setup/swapchain.h"
#include "vulkan_setup/framebuffers.h"
#include "vulkan_setup/synchronization.h"

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

    s.swapchain = createSwapchain(&s, &s.swapchain_extent, &s.swapchain_image_format, window);
    s.swapchain_images = getSwapchainImages(&s, &s.num_swapchain_images);

    s.swapchain_image_views = createSwapchainImageViews(&s);

    s.render_pass = createRenderPass(&s);
    s.pipeline = createGraphicsPipeline(&s, &s.pipeline_layout);
    s.frame_buffers = createFramebuffers(&s);

    s.command_pool = createCommandPool(&s);

    for(uint32_t i = 0; i < ARRAY_LENGTH(s.command_buffer_infos); i++) {
        s.command_buffer_infos[i] = CommandBufferInfo_create(&s);
    }

    s.current_command_buffer_info = 0;

    return s;
}

void VulkanState_destroy(VulkanState s) {
    // Vulkan does not like it if you destroy stuff while GPU code is running //
    vkDeviceWaitIdle(s.device);

    for(uint32_t i = 0; i < ARRAY_LENGTH(s.command_buffer_infos); i++) {
        const CommandBufferInfo *const info = &s.command_buffer_infos[i];

        vkDestroySemaphore(s.device, info->image_available_semaphore, NULL);
        vkDestroySemaphore(s.device, info->render_finished_semaphore, NULL);
        vkDestroyFence(s.device, info->in_flight_fence, NULL);
    }

    vkDestroyCommandPool(s.device, s.command_pool, NULL);
    destroyFramebuffers(&s, s.frame_buffers);
    vkDestroyPipeline(s.device, s.pipeline, NULL);
    vkDestroyPipelineLayout(s.device, s.pipeline_layout, NULL);
    vkDestroyRenderPass(s.device, s.render_pass, NULL);
    destroyImageViews(&s, s.swapchain_image_views);
    free(s.swapchain_images);
    vkDestroySwapchainKHR(s.device, s.swapchain, NULL);
    vkDestroyDevice(s.device, NULL);
    vkDestroySurfaceKHR(s.instance, s.window_surface, NULL);
    vkDestroyInstance(s.instance, NULL);
}

static VkInstance createVulkanInstance(void) {
    VkApplicationInfo app_info = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,

        .pApplicationName = "Hello Triangle",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "No Engine",
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = VK_API_VERSION_1_0,
    };

    VkInstanceCreateInfo create_info = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,

        .pApplicationInfo = &app_info,

        .ppEnabledExtensionNames = glfwGetRequiredInstanceExtensions(&create_info.enabledExtensionCount),
    };

    if(ENABLE_VALIDATION_LAYERS) {
        create_info.enabledLayerCount = ARRAY_LENGTH(VALIDATION_LAYERS);
        create_info.ppEnabledLayerNames = VALIDATION_LAYERS;
    } else {
        create_info.enabledLayerCount = 0;
    }

    VkInstance instance = NULL;

    VkResult result = vkCreateInstance(&create_info, NULL, &instance);
    handleVkError("Failed to initialize vulkan", result);

    return instance;
}

static VkSurfaceKHR createWindowSurface(VkInstance instance, GLFWwindow *window) {
    VkSurfaceKHR surface;

    VkResult result = glfwCreateWindowSurface(instance, window, NULL, &surface);
    handleVkError("Failed to create window surface", result);

    return surface;
}
