#pragma once

#include <stddef.h>

typedef struct {
    char* chars;
    size_t length;
} Line;

void initLine(Line* line);
void freeLine(Line* line);
