#pragma once

#include<stdlib.h>
#include<stdint.h>
#include<stdbool.h>

#define ARRAY_LENGTH(arr) (sizeof(arr)/sizeof(arr[0]))

typedef struct {
    uint32_t value;
    bool present;
} OptionalU32;

OptionalU32 OptionalU32_of(uint32_t val);

/// Tries to allocate `bytes` bytes. On error, prints to stderr and terminates the program.
void *emalloc(size_t bytes);
