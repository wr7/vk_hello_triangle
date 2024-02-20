#pragma once

#include<stdlib.h>
#include<stdint.h>
#include<stdbool.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define ARRAY_LENGTH(arr) (sizeof(arr)/sizeof(arr[0]))

typedef struct {
    uint32_t value;
    bool present;
} OptionalU32;

OptionalU32 OptionalU32_empty(void);
OptionalU32 OptionalU32_of(uint32_t val);

void handleVkError(const char *const msg, VkResult result);

/// Tries to allocate `bytes` bytes. On error, prints to stderr and terminates the program.
void *emalloc(size_t bytes);
