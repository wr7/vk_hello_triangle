#define _POSIX_C_SOURCE 200809L

#ifdef _MSC_VER
    #include <windows.h>
#endif

#include "util.h"

#include <stdint.h>
#include <vulkan/vk_enum_string_helper.h>

#include <math.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>

#include <time.h>

OptionalU32 OptionalU32_empty(void) {
    return (OptionalU32) {.present = false};
}

OptionalU32 OptionalU32_of(uint32_t val) {
    return (OptionalU32) {.value = val, .present = true};
}

void handleVkError(const char *const msg, VkResult result) {
    // Positive VkResult values are still successful
    if(result < VK_SUCCESS) {
        fprintf(stderr, "%s: %s\n", msg, string_VkResult(result));
        exit(result);
    }
    
}

/// Tries to allocate `bytes` bytes. On error, prints to stderr and terminates the program.
void *emalloc(size_t bytes) {
    void *buf = malloc(bytes);

    if(buf == NULL) {
        perror("Failed to allocate memory");
        exit(errno);
    }

    return buf;
}

noreturn void error(const char *const msg) {
    fprintf(stderr, "%s", msg);
    exit(1);
}

uint32_t uint32_t_clamp(uint32_t val, uint32_t min, uint32_t max) {
    if(val < min) {
        return min;
    } else if(val > max) {
        return max;
    } else {
        return val;
    }
}

pure float radians(const float degrees) {
    return PI * degrees / 180.0;
}

#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
// POSIX Implementation of get_time_nanos

static uint64_t start_time;

__attribute__( ( constructor ) )
static void _set_start_time() {
    start_time = 0;
    start_time = get_time_nanos();
}

/**
 * Gets the time since the process started in nanoseconds
 */
uint64_t get_time_nanos() {
    struct timespec ts;

    clock_gettime(CLOCK_MONOTONIC, &ts);

    return (((uint64_t) ts.tv_sec) * 1e+9 + (uint64_t) ts.tv_nsec) - start_time;
}

#elif defined (_MSC_VER)
// Windows Implementation of get_time_nanos

static uint64_t start_time;

static int __cdecl _set_start_time(void) {
    start_time = get_time_nanos();
    return 0;
}

// Windows jank to run _set_start_time before main() //
#pragma section(".CRT$XIB__SET_START_TIME", read)

__declspec(allocate(".CRT$XIB__SET_START_TIME"))
static int (* __cdecl _pset_start_time)(void) = _set_start_time;

/**
 * Gets the time since the process started in nanoseconds
 */
uint64_t get_time_nanos() {
    uint64_t micros = GetTickCount64();
    return micros * 1000;
}
#else
#error "No get_time_nanos function implemented for platform"
#endif
