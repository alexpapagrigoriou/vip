#include "error.h"

#include <stdio.h>
#include <stdlib.h>

#include "draw.h"

void error(const char* msg, const char* file, int line, const char* func) {
    clean_screen();
    fflush(stdout);
#ifdef NDEBUG
    (void)file;
    (void)line;
    (void)func;
    fprintf(stderr, "Error: %s\n", msg);
#else
    fprintf(stderr, "Error: %s (%s:%d, %s)\n", msg, file, line, func);
#endif
    fflush(stderr);

    exit(EXIT_FAILURE);
}
