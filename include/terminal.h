#pragma once

#include <stddef.h>

typedef struct {
    size_t row;
    size_t col;
} Position;

extern Position screen;

void disableRawMode(void);
void enableRawMode(void);
void moveCursor(Position pos);
void resetCursor(void);
void cleanScreen(void);
void cleanLine(void);
void refreshWindow(void);
void updateWindowSize(void);
