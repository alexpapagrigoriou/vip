#pragma once

#include <stddef.h>

typedef struct {
    size_t row;
    size_t col;
} Position;

void disableRawMode(void);
void enableRawMode(void);
void updateWindowSize(void);
Position getMaxScreen(void);
Position getMaxBuffer(void);
