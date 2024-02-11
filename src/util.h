#pragma once

#include<stdlib.h>

#define ARRAY_LENGTH(arr) (sizeof(arr)/sizeof(arr[0]))

void *emalloc(size_t bytes);
