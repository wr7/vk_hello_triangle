#pragma once

#include "vulkan/vulkan_core.h"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include<stddef.h>
#include "util.h"

typedef struct {
    float entries[4];
} Vec4;

typedef struct {
    float entries[3];
} Vec3;

typedef struct {
    Vec4 columns[4];
} Mat4;

typedef struct {
    Mat4 model;
    Mat4 view;
    Mat4 proj;
} UniformBufferObject;

typedef struct {
    float pos[2];
    float color[3];
} Vertex;

static const VkVertexInputBindingDescription Vertex_BINDING_DESCRIPTION = {
    .binding = 0,
    .stride = sizeof(Vertex),
    .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
};

static const VkVertexInputAttributeDescription Vertex_ATTRIBUTE_DESCRIPTIONS[] = {
    {
        .binding = 0,
        .location = 0,
        .format = VK_FORMAT_R32G32_SFLOAT,
        .offset = offsetof(Vertex, pos),
    },
    {
        .binding = 0,
        .location = 1,
        .format = VK_FORMAT_R32G32B32_SFLOAT,
        .offset = offsetof(Vertex, color),
    },
};

pure Mat4 Mat4_identity();

pure Vec4 Mat4_apply(const Mat4 matrix, const Vec4 vector);

pure Mat4 Mat4_perspective(const float fov, const float aspect_ratio, const float znear, const float zfar);

pure Mat4 Mat4_yaw_pitch_roll(const float yaw, const float pitch, const float roll);
pure Mat4 Mat4_inverse_yaw_pitch_roll(const float yaw, const float pitch, const float roll);

pure Mat4 Mat4_yaw_pitch_roll_translate(const float yaw, const float pitch, const float roll, const Vec3 translation);
pure Mat4 Mat4_inverse_yaw_pitch_roll_translate(const float yaw, const float pitch, const float roll, const Vec3 translation);
