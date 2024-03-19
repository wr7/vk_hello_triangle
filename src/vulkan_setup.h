#pragma once

#include "vulkan/vulkan_core.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "vulkan_setup/queues.h"
#include <stdbool.h>

typedef struct {
    VkCommandBuffer buffer;

    VkSemaphore image_available_semaphore;
    VkSemaphore render_finished_semaphore;
    VkFence in_flight_fence;
} CommandBufferInfo;


typedef struct {
    VkInstance instance;
    VkSurfaceKHR window_surface;
    VkPhysicalDevice graphics_card;
    VkDevice device;
    VkSwapchainKHR swapchain;

    Queues queues;
    QueueFamilyIndices indices;

    VkRenderPass render_pass;

    VkPipeline pipeline;
    VkPipelineLayout pipeline_layout;

    VkFramebuffer *frame_buffers;

    VkBuffer vertex_buffer;
    VkDeviceMemory vertex_buffer_memory;

    // Swapchain stuff //

    uint32_t num_swapchain_images;
    VkImage *swapchain_images;
    VkImageView *swapchain_image_views;

    VkExtent2D swapchain_extent;
    VkFormat swapchain_image_format;

    // Command buffer stuff //

    VkCommandPool command_pool;
    CommandBufferInfo command_buffer_infos[2];

    uint32_t current_command_buffer_info;
} VulkanState;

static const char *const VALIDATION_LAYERS[] = {
    "VK_LAYER_KHRONOS_validation"
};
extern const bool ENABLE_VALIDATION_LAYERS;

VulkanState VulkanState_create(GLFWwindow *window);
void VulkanState_destroy(VulkanState s);
