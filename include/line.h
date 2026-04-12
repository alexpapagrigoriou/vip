#pragma once

#include <stddef.h>

typedef struct {
    char* chars;
    size_t length;
    size_t capacity;
} Line;

void lineInit(Line* line);
void freeLine(Line* line);
void lineEnsure(Line* line, size_t needed);
void lineCompact(Line* line);
