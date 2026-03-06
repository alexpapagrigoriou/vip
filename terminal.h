#pragma once

typedef struct {
    int row;
    int col;
} Position;

extern Position screen;

void disableRawMode(void);
void enableRawMode(void);
void updateWindowSize(void);
void moveCursor(Position pos);
void cleanScreen(void);
void cleanLine(void);
