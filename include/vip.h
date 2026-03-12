#pragma once

#include <stdbool.h>

#include "buffer.h"
#include "line.h"
#include "mode.h"
#include "terminal.h"

typedef struct {
    Buffer* buffer;
    Position cursor;
    bool save_curosr_col;
    size_t prev_cursor_col;
    Position text;
    Line* status_line;
    size_t status_cursor_col;
    bool in_start;
    char pending_operator;
    Mode mode;
    char* filename;
} Editor;

void runVip(char* filename);
Line* getLine(size_t row);
size_t getLineCount(void);
Position getCursorPosition(void);
Position getTextPosition(void);
bool isInStart(void);
