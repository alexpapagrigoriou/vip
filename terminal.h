#pragma once

extern int rows, cols;
extern int cursor_row, cursor_col;
extern int text_row, text_col;

void disableRawMode(void);
void enableRawMode(void);
void autoCleanup(void);
void updateWindowSize(void);
void moveCursor(int row, int col);
void cleanScreen(void);
void cleanLine(void);
