#include "main_window.h"

#include <stdint.h>

const uint32_t WINDOW_WIDTH = 800;
const uint32_t WINDOW_HEIGHT = 600;

static void initVulkan(MainWindow *w) {
    
}

MainWindow MainWindow_create(void) {
    MainWindow w;

    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    w.window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Vulkan Window", NULL, NULL);

    initVulkan(&w);

    return w;
}

void MainWindow_run(MainWindow *w) {
    while (!glfwWindowShouldClose(w->window)) {
        glfwPollEvents();
    }
}

void MainWindow_destroy(MainWindow *w) {
    glfwDestroyWindow(w->window);

    glfwTerminate();
}

