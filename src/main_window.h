#pragma once

#include "vulkan_setup.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "math_types.h"

struct MainWindow {
    GLFWwindow *window;
    VulkanState vk_state;

    bool window_resized;
};

static const Vertex VERTICES[] = {
    {
        .pos = {0.0f, -0.5f},
        .color = {1.0f, 0.0f, 0.0f},
    },
    {
        .pos = {0.5f, 0.5f},
        .color = {0.0f, 1.0f, 0.0f},
    },
    {
        .pos = {-0.5f, 0.5f},
        .color = {0.0f, 0.0f, 1.0f},
    },
};

typedef struct MainWindow MainWindow;

MainWindow *MainWindow_create(void);
void MainWindow_run(MainWindow *w);
void MainWindow_destroy(MainWindow *w);
