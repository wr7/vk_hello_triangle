#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "main_window.h"
#include "util.h"
#include "vulkan/vulkan_core.h"
#include "vulkan_setup.h"
#include "vulkan_setup/command_buffer.h"

const uint32_t WINDOW_WIDTH = 800;
const uint32_t WINDOW_HEIGHT = 600;

static void draw_frame(const MainWindow *const w);

MainWindow MainWindow_create(void) {
    MainWindow w = {0};

    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    w.window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Vulkan Window", NULL, NULL);

    w.vk_state = VulkanState_create(w.window);

    return w;
}

void MainWindow_run(MainWindow *w) {
    while (!glfwWindowShouldClose(w->window)) {
        glfwPollEvents();
        draw_frame(w);
    }
}

static void draw_frame(const MainWindow *const w) {
    const VulkanState *const s = &w->vk_state;

    vkWaitForFences(s->device, 1, &s->in_flight_fence, VK_TRUE, UINT64_MAX);
    vkResetFences(s->device, 1, &s->in_flight_fence);

    uint32_t image_index;
    vkAcquireNextImageKHR(s->device, s->swapchain, UINT64_MAX,
        s->image_available_semaphore, VK_NULL_HANDLE, &image_index);

    vkResetCommandBuffer(s->command_buffer, 0);

    recordCommandBuffer(s, image_index);

    VkSubmitInfo submitInfo = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,

        .pCommandBuffers = &s->command_buffer,
        .commandBufferCount = 1,

        .pWaitDstStageMask = (const VkPipelineStageFlags[]){VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT},
        .pWaitSemaphores = &s->image_available_semaphore,
        .waitSemaphoreCount= 1,

        .pSignalSemaphores = &s->render_finished_semaphore,
        .signalSemaphoreCount = 1,
    };

    handleVkError("Failed to submit to queue",
        vkQueueSubmit(s->queues.graphics_queue, 1, &submitInfo, s->in_flight_fence)
    );

    VkPresentInfoKHR presentInfo = {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
    
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &s->render_finished_semaphore,

        .swapchainCount = 1,
        .pSwapchains = &s->swapchain,
        .pImageIndices = &image_index,
        .pResults = NULL,
    };

    handleVkError("Failed to present image", 
        vkQueuePresentKHR(s->queues.presentation_queue, &presentInfo)
    );
}

void MainWindow_destroy(MainWindow *w) {
    VulkanState_destroy(w->vk_state);
    
    glfwDestroyWindow(w->window);
    glfwTerminate();
}

