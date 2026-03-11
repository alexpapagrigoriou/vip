#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "line.h"

Line* status_line;

size_t status_cursor_col;

void initStatusLine(void) {
    status_line = malloc(sizeof(Line));
    initLine(status_line);

    status_cursor_col = 0;
}

void freeStatusLine(void) {
    freeLine(status_line);
}
