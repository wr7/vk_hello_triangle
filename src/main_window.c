#include "vulkan_setup/synchronization.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "main_window.h"
#include "util.h"
#include "vulkan/vulkan_core.h"
#include "vulkan_setup.h"
#include "vulkan_setup/command_buffer.h"
#include "vulkan_setup/images.h"
#include "vulkan_setup/swapchain.h"
#include "vulkan_setup/framebuffers.h"

static const uint32_t WINDOW_INIT_WIDTH = 800;
static const uint32_t WINDOW_INIT_HEIGHT = 600;

static void draw_frame(MainWindow *const w);

static void resizeHandler(GLFWwindow *window, int width, int height) {
    UNUSED(width); UNUSED(height);

    MainWindow *const w = (MainWindow *) glfwGetWindowUserPointer(window);
    w->window_resized = true;
}

MainWindow *MainWindow_create(void) {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    GLFWwindow *window = glfwCreateWindow(WINDOW_INIT_WIDTH, WINDOW_INIT_HEIGHT, "Vulkan Window", NULL, NULL);

    MainWindow *w = malloc(sizeof(*w));

    *w = (MainWindow){
        .window = window,
        .window_resized = false,
    };
    w->vk_state = VulkanState_create(w->window),

    glfwSetWindowUserPointer(w->window, w);
    glfwSetWindowSizeCallback(window, resizeHandler);

    return w;
}

void MainWindow_run(MainWindow *w) {
    while (!glfwWindowShouldClose(w->window)) {
        glfwPollEvents();
        draw_frame(w);
    }
}

static void recreateSwapchain(MainWindow *const w);

static void draw_frame(MainWindow *const w) {
    start: {};

    VulkanState *const s = &w->vk_state;
    const CommandBufferInfo *const info = &s->command_buffer_infos[s->current_command_buffer_info];

    vkWaitForFences(s->device, 1, &info->in_flight_fence, VK_TRUE, UINT64_MAX);

    uint32_t image_index;

    VkResult next_image_result = vkAcquireNextImageKHR(s->device, s->swapchain, UINT64_MAX,
        info->image_available_semaphore, VK_NULL_HANDLE, &image_index);

    if(next_image_result == VK_SUBOPTIMAL_KHR || next_image_result == VK_ERROR_OUT_OF_DATE_KHR || w->window_resized) {
        w->window_resized = false;
        recreateSwapchain(w);
        goto start;
    }

    handleVkError("Failed to acquire swapchain image", next_image_result);

    vkResetFences(s->device, 1, &info->in_flight_fence);

    handleVkError("Failed to reset command buffer",
        vkResetCommandBuffer(info->buffer, 0)
    );

    recordCommandBuffer(s, image_index);

    VkSubmitInfo submitInfo = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,

        .pCommandBuffers = &info->buffer,
        .commandBufferCount = 1,

        .pWaitDstStageMask = (const VkPipelineStageFlags[]){VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT},
        .pWaitSemaphores = &info->image_available_semaphore,
        .waitSemaphoreCount= 1,

        .pSignalSemaphores = &info->render_finished_semaphore,
        .signalSemaphoreCount = 1,
    };

    handleVkError("Failed to submit to queue",
        vkQueueSubmit(s->queues.graphics_queue, 1, &submitInfo, info->in_flight_fence)
    );

    VkPresentInfoKHR presentInfo = {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
    
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &info->render_finished_semaphore,

        .swapchainCount = 1,
        .pSwapchains = &s->swapchain,
        .pImageIndices = &image_index,
        .pResults = NULL,
    };

    handleVkError("Failed to present image", 
        vkQueuePresentKHR(s->queues.presentation_queue, &presentInfo)
    );

    s->current_command_buffer_info += 1;
    s->current_command_buffer_info %= ARRAY_LENGTH(s->command_buffer_infos);
}

static void recreateSwapchain(MainWindow *const w) {
    VulkanState *const s = &w->vk_state;
    CommandBufferInfo *const info = &s->command_buffer_infos[s->current_command_buffer_info];

    int width, height = 0;
    glfwGetFramebufferSize(w->window, &width, &height);
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(w->window, &width, &height);
        glfwWaitEvents();
    }    vkDeviceWaitIdle(s->device);

    // Destroy previous swapchain //
    destroyFramebuffers(s, s->frame_buffers);
    destroyImageViews(s, s->swapchain_image_views);
    free(s->swapchain_images);
    vkDestroySwapchainKHR(s->device, s->swapchain, NULL);
    vkDestroySemaphore(s->device, info->image_available_semaphore, NULL);
    
    // Create new swapchain and all of its dependents //
    s->swapchain = createSwapchain(s, &s->swapchain_extent, &s->swapchain_image_format, w->window);
    s->swapchain_images = getSwapchainImages(s, &s->num_swapchain_images);
    s->swapchain_image_views = createSwapchainImageViews(s);
    s->frame_buffers = createFramebuffers(s);

    info->image_available_semaphore = createSemaphore(s);
}

void MainWindow_destroy(MainWindow *w) {
    VulkanState_destroy(w->vk_state);
    glfwDestroyWindow(w->window);

    glfwTerminate();
    free(w);
}

