#include "status.h"

#include <stdlib.h>

StatusLine* createStatusLine(void) {
    StatusLine* status_line = malloc(sizeof(StatusLine));
    status_line->chars[0] = '\0';

    return status_line;
}

void freeStatusLine(StatusLine* status_line) {
    free(status_line);
}
