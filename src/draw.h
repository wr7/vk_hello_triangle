#pragma once

#include "main_window.h"

#define VERTICES_DEF {\
    {\
        .pos = {-0.5f, 0.5f},\
        .color = {0.0f, 1.0f, 0.0f},\
    },\
    {\
        .pos = {-0.5f, -0.5f},\
        .color = {1.0f, 0.0f, 0.0f},\
    },\
    {\
        .pos = {0.5f, 0.5f},\
        .color = {1.0f, 1.0f, 0.0f},\
    },\
    {\
        .pos = {0.5f, -0.5f},\
        .color = {0.0f, 0.0f, 1.0f},\
    },\
}

#define VERTEX_INDICES_DEF {0, 1, 2, 3}

ALLOW_UNUSED_START

// Cursed
static const Vertex VERTICES[] = VERTICES_DEF;
static const uint16_t VERTEX_INDICES[] = VERTEX_INDICES_DEF;

// Ensure that the vertex and index data are next to eachother in the compiled program //
static const struct {
    typeof(VERTICES) vertices;
    typeof(VERTEX_INDICES) indices;
} VERTICES_AND_INDICES = {VERTICES_DEF, VERTEX_INDICES_DEF,};

ALLOW_UNUSED_END

static const VkDeviceSize VERTEX_BUFFER_OFFSET = offsetof(typeof(VERTICES_AND_INDICES), vertices);
static const VkDeviceSize INDEX_BUFFER_OFFSET = offsetof(typeof(VERTICES_AND_INDICES), indices);

void draw_frame(MainWindow *const w);
