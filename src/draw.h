#pragma once

#include "main_window.h"

#define VERTICES_DEF {\
    {/*Front top left (0)*/\
        .pos = {-0.5, 0.5, -0.5},\
        .color = {1.0f, 1.0f, 1.0f},\
    },\
    {/*Front top right (1)*/\
        .pos = {0.5, 0.5, -0.5},\
        .color = {0.0f, 1.0f, 1.0f},\
    },\
    {/*Front bottom left (2)*/\
        .pos = {-0.5, -0.5, -0.5},\
        .color = {1.0f, 0.0f, 1.0f},\
    },\
    {/*Front bottom right (3)*/\
        .pos = {0.5, -0.5, -0.5},\
        .color = {0.0f, 0.0f, 1.0f},\
    },\
    {/*Back top left (4)*/\
        .pos = {-0.5, 0.5, 0.5},\
        .color = {1.0f, 1.0f, 0.0f},\
    },\
    {/*Back top right (5)*/\
        .pos = {0.5, 0.5, 0.5},\
        .color = {0.0f, 1.0f, 0.0f},\
    },\
    {/*Back bottom left (6)*/\
        .pos = {-0.5, -0.5, 0.5},\
        .color = {1.0f, 0.0f, 0.0f},\
    },\
    {/*Back bottom right (7)*/\
        .pos = {0.5, -0.5, 0.5},\
        .color = {0.0f, 0.0f, 0.0f},\
    },\
}

#define FRONT_TOP_LEFT 0
#define FRONT_TOP_RIGHT 1
#define FRONT_BOTTOM_LEFT 2
#define FRONT_BOTTOM_RIGHT 3
#define BACK_TOP_LEFT 4
#define BACK_TOP_RIGHT 5
#define BACK_BOTTOM_LEFT 6
#define BACK_BOTTOM_RIGHT 7

#define VERTEX_INDICES_DEF { \
        /*Front face*/ \
        1, 0, 2,  1, 2, 3, \
        /*Back face*/ \
        4, 5, 6,  6, 5, 7, \
        /*LEFT face*/ \
        0, 4, 6,  0, 6, 2, \
        /*RIGHT face*/ \
        5, 1, 3,  3, 7, 5, \
        /*TOP face*/ \
        5, 4, 0,  0, 1, 5, \
        /*BOTTOM face*/ \
        3, 2, 6,  6, 7, 3, \
    }

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
