#pragma once

#include<stdlib.h>
#include<stdint.h>
#include<stdbool.h>
#include <stdnoreturn.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define ARRAY_LENGTH(arr) (sizeof(arr)/sizeof(arr[0]))
#define UNUSED(val) (void) val
#define STRINGIFY(a) #a

#ifdef _MSC_EXTENSIONS
    #define align(n) _declspec( align ( n ) )
#elif defined(__GNUC__)
    #define align(n) __attribute__( ( aligned ( n ) ) )
#else
    #warning Failed to find variable alignment compiler extention. Code may fail to compile.
    #define align(n) _Pragma( STRINGIFY(align##(##n##) ) )
#endif

typedef struct {
    uint32_t value;
    bool present;
} OptionalU32;

OptionalU32 OptionalU32_empty(void);
OptionalU32 OptionalU32_of(uint32_t val);

void handleVkError(const char *const msg, VkResult result);

/// Tries to allocate `bytes` bytes. On error, prints to stderr and terminates the program.
void *emalloc(size_t bytes);
noreturn void error(const char *const msg);

uint32_t uint32_t_clamp(uint32_t min, uint32_t val, uint32_t max);
