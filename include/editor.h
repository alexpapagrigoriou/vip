#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "buffer.h"
#include "draw.h"

#define STATUS_LINE_SIZE 256

typedef enum {
    NORMAL,
    INSERT,
    VISUAL,
    VISUAL_LINE,
    COMMAND,
    EXIT
} Mode;

typedef struct {
    Buffer buffer;
    Position cursor;
    bool successful_motion;
    bool save_curosr_col;
    size_t prev_cursor_col;
    Position text;
    char status_line[STATUS_LINE_SIZE];
    char key_cache_string[KEY_CACHE_STRING_SIZE + 1];
    char cursor_position_string[CURSOR_POSITION_STRING_SIZE + 1];
    size_t status_cursor_col;
    bool in_start;
    Mode mode;
    char* filename;
} Editor;

void editorInit(Editor* editor, char* filename);
void freeEditor(Editor* editor);
