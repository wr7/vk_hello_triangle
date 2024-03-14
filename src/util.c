#include "util.h"

#include <stdint.h>
#include <vulkan/vk_enum_string_helper.h>

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

OptionalU32 OptionalU32_empty(void) {
    OptionalU32 val = {0};
    return val;
}

OptionalU32 OptionalU32_of(uint32_t val) {
    OptionalU32 optional;
    optional.present = true;
    optional.value = val;

    return optional;
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

uint32_t uint32_t_clamp(uint32_t val, uint32_t min, uint32_t max) {
    if(val < min) {
        return min;
    } else if(val > max) {
        return max;
    } else {
        return val;
    }
}
