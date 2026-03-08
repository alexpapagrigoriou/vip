#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "line.h"

Line* command_line;

size_t command_cursor_col;

void initCommandLine(void) {
    command_line = malloc(sizeof(Line));
    initLine(command_line);

    command_cursor_col = 0;
}

void freeCommandLine(void) {
    freeLine(command_line);
}
