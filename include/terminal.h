#pragma once

#include <stddef.h>

typedef struct {
    size_t row;
    size_t col;
} Position;

extern Position max_screen;

void disableRawMode(void);
void enableRawMode(void);
void updateWindowSize(void);
