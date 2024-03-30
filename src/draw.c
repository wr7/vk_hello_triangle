#include <math.h>
#include <time.h>

#include "draw.h"

#include "math_types.h"
#include "util.h"
#include "vulkan_setup.h"
#include "vulkan_setup/command_buffer.h"
#include "vulkan_setup/framebuffers.h"
#include "vulkan_setup/images.h"
#include "vulkan_setup/swapchain.h"
#include "vulkan_setup/synchronization.h"

static void recreateSwapchain(MainWindow *const w);

static void updateUniformBuffer(const VulkanState *const s, UniformBufferObject *const ubo);

void draw_frame(MainWindow *const w) {
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

    updateUniformBuffer(s,  &(*s->mapped_uniform_buffer_memory)[s->current_command_buffer_info]);

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

static void updateUniformBuffer(const VulkanState *const s, UniformBufferObject *const ubo) {
    const clock_t raw_time = clock();

    if(raw_time < 0) {
        error("Failed to get time");
    }

    // Get time in deciseconds
    const uint64_t time = raw_time / ( CLOCKS_PER_SEC/100 );

    const double rotm = 1/2.0;
    const double model_yaw = fmodf(time / (rotm*100.0), 2.0*PI);
    const double model_pitch = fmodf(time / (rotm*130.0), 2.0*PI);
    const double model_roll = fmodf(time / (rotm*145.0), 2.0*PI);
    const double model_y_offset = sin(time/32.0)*0.3;

    ubo->model = Mat4_yaw_pitch_roll_translate(model_yaw, model_pitch, model_roll, (Vec3) {{0, model_y_offset, 2}});
    ubo->view = Mat4_identity();
    ubo->proj = Mat4_perspective(radians(45), ((float) s->swapchain_extent.width)/s->swapchain_extent.height, 0.1f, 10.0f);
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

