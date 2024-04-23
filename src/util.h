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

#define PI 3.14159265358979323846

#define ARRAY_LENGTH(arr) (sizeof(arr)/sizeof((arr)[0]))
#define UNUSED(val) (void) val
#define STRINGIFY(a) #a

#ifndef __has_attribute
    #define __has_attribute(x) 0
#endif

#ifdef _MSC_EXTENSIONS
    #define align(n) __declspec( align ( n ) )
    #define typeof(n) __typeof__(n)
    #define pure
    #define always_inline inline __forceinline
#elif defined(__GNUC__)
    #if ( __has_attribute ( align ) )
        #define align(n) __attribute__( ( aligned ( n ) ) )
    #endif

    #define typeof(n) __typeof__(n)

    #if ( __has_attribute ( pure ) )
        #define pure __attribute__( ( pure ) )
    #else
        #define pure
    #endif

    #if ( __has_attribute ( always_inline ) )
        #define always_inline inline __attribute__( ( always_inline ) ) 
    #else
        #define always_inline inline
    #endif
#endif

#if (__STDC_VERSION__ >= 201112L)
    #undef align
    #define align(x) _Alignas(x)
#elif ( !defined(align) )
    #error "Failed to find proper alignas implementation"
#endif

#if (__STDC_VERSION__ <= 201710L)
    #ifndef typeof
        #error "Failed to find proper typeof implementation; C23 or compiler extention required"
    #endif
#else
    #undef typeof
#endif

#ifndef pure
    #define pure
    #define always_inline inline
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
pure float radians(const float degrees);

/**
 * Gets the time since the process started in nanoseconds
 */
uint64_t get_time_nanos();
