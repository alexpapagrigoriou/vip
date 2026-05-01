#include "error.h"

#include <stdio.h>
#include <stdlib.h>

#include "draw.h"

void error(const char* msg, const char* file, int line, const char* func) {
    clean_screen();
    fflush(stdout);

    fprintf(stderr, "Error: %s (%s:%d, %s)\n", msg, file, line, func);
    fflush(stderr);

    exit(EXIT_FAILURE);
}
