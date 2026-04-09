#pragma once

#include <stdbool.h>

#include "buffer.h"

typedef enum {
    NORMAL,
    INSERT,
    VISUAL,
    COMMAND,
    EXIT
} Mode;

typedef struct {
    Buffer buffer;
    Position cursor;
    bool save_curosr_col;
    size_t prev_cursor_col;
    Position text;
    char status_line[256];
    size_t status_cursor_col;
    bool in_start;
    Mode mode;
    char* filename;
} Editor;

void editorInit(Editor* editor, char* filename);
void freeEditor(Editor* editor);
