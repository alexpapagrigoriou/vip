#include "line.h"

#include <stdlib.h>

void initLine(Line* line) {
    line->chars = malloc(1);
    line->chars[0] = '\0';

    line->length = 0;
}

void freeLine(Line* line) {
    free(line->chars);
}
