#include "status.h"

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

Line* createStatusLine(void) {
    Line* status_line = malloc(sizeof(Line));
    lineInit(status_line);

    return status_line;
}

void freeStatusLine(Line* status_line) {
    freeLine(status_line);
}
