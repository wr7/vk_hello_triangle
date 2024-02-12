#pragma once

#include "vulkan_setup.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

struct MainWindow {
    GLFWwindow *window;
    VulkanState vk_state;
};

typedef struct MainWindow MainWindow;

MainWindow MainWindow_create(void);
void MainWindow_run(MainWindow *w);
void MainWindow_destroy(MainWindow *w);
