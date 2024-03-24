#pragma once

#include "util.h"
#include "vulkan_setup.h"
#include <stddef.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "math_types.h"

struct MainWindow {
    GLFWwindow *window;
    VulkanState vk_state;

    bool window_resized;
};

typedef struct MainWindow MainWindow;

MainWindow *MainWindow_create(void);
void MainWindow_run(MainWindow *w);
void MainWindow_destroy(MainWindow *w);
