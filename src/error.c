#include "error.h"

#include <stdio.h>
#include <stdlib.h>

void error(const char* msg, const char* file, int line, const char* func) {
    fprintf(stderr, "Error: %s (%s:%d, %s)\n", msg, file, line, func);
    exit(EXIT_FAILURE);
}
