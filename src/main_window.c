#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "main_window.h"
#include "vulkan_setup.h"

const uint32_t WINDOW_WIDTH = 800;
const uint32_t WINDOW_HEIGHT = 600;

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
    }
}

void MainWindow_destroy(MainWindow *w) {
    VulkanState_destroy(w->vk_state);
    
    glfwDestroyWindow(w->window);
    glfwTerminate();
}

