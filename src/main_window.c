#include "draw.h"
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

void MainWindow_destroy(MainWindow *w) {
    VulkanState_destroy(w->vk_state);
    glfwDestroyWindow(w->window);

    glfwTerminate();
    free(w);
}

