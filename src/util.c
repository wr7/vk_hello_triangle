#include "util.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

OptionalU32 OptionalU32_of(uint32_t val) {
    OptionalU32 optional;
    optional.present = true;
    optional.value = val;

    return optional;
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
