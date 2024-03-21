#pragma once

#include<stdlib.h>
#include<stdint.h>
#include<stdbool.h>
#include <stdnoreturn.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

// Stuff for hiding useless clangd warnings
#ifdef __clang__
    #pragma GCC diagnostic ignored "-Wkeyword-macro"
    #define ALLOW_UNUSED_START _Pragma("GCC diagnostic push")\
        _Pragma("GCC diagnostic ignored \"-Wunneeded-internal-declaration\"")
    #define ALLOW_UNUSED_END _Pragma("GCC diagnostic pop")
#else
    #define ALLOW_UNUSED_START
    #define ALLOW_UNUSED_END
#endif

// Utility macros //

#define ARRAY_LENGTH(arr) (sizeof(arr)/sizeof(arr[0]))
#define UNUSED(val) (void) val
#define STRINGIFY(a) #a

#ifdef _MSC_EXTENSIONS
    #define align(n) __declspec( align ( n ) )
    #define typeof(n) __typeof__(n)
#elif defined(__GNUC__)
    #define align(n) __attribute__( ( aligned ( n ) ) )
    #define typeof(n) __typeof__(n)
#else
    // Align fallback (requires C11)
    #define _align1 _Alignas(char)
    #define _align2 _Alignas(short)
    #define _align4 _Alignas(int)
    #define _align8 _Alignas(long)
    #define _align16 _Alignas(long long)
    #define align(n) _align##n

    // Typeof fallback (requires C23)
    #warning Failed to find typeof alignment compiler extention. Code may fail to compile.
    // C23 has the typeof operator by default
    // #define typeof(n) typeof(n)
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
