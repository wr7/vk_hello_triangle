#include "util.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

/// Tries to allocate `bytes` bytes. On error, prints to stderr and terminates the program.
void *emalloc(size_t bytes) {
  void *buf = malloc(bytes);

  if(buf == NULL) {
    perror("Failed to allocate memory");
    exit(errno);
  }

  return buf;
}
