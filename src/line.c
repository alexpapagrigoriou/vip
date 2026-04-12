#include "line.h"

#include <stdlib.h>

#include "error.h"

#define LINE_INITIAL_CAPACITY 16

void lineInit(Line* line) {
    line->chars = malloc(LINE_INITIAL_CAPACITY);
    line->chars[0] = '\0';
    line->length = 0;
    line->capacity = LINE_INITIAL_CAPACITY;
}

void freeLine(Line* line) {
    free(line->chars);
}

void lineEnsure(Line* line, size_t needed) {
    if (needed <= line->capacity) {
        return;
    }

    size_t cap = line->capacity ? line->capacity : LINE_INITIAL_CAPACITY;
    while (cap < needed) {
        cap += cap / 2;
    }

    char* tmp = realloc(line->chars, cap);
    if (!tmp) {
        ERROR("Out of memory");
    }

    line->chars = tmp;
    line->capacity = cap;
}

void lineCompact(Line* line) {
    if (line->capacity <= LINE_INITIAL_CAPACITY) {
        return;
    }

    size_t needed = line->length + 1;
    size_t new_cap = needed < LINE_INITIAL_CAPACITY ? LINE_INITIAL_CAPACITY : needed;
    char* tmp = realloc(line->chars, new_cap);
    if (!tmp) {
        ERROR("Out of memory");
    }

    line->chars = tmp;
    line->capacity = new_cap;
}
