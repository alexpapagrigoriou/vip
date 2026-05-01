#include "vip.h"

#include <signal.h>
#include <stdio.h>
#include <string.h>

#include "colors.h"
#include "draw.h"
#include "editor.h"
#include "input.h"
#include "parser.h"

static Editor editor;
static Parser parser;

static void cleanup(Editor* editor) {
    disableRawMode();

    freeEditor(editor);

    cleanScreen();
}

void runVip(char* filename) {
    enableRawMode();

    editorInit(&editor, filename);

    signal(SIGWINCH, handleResize);
    updateWindowSize();

    while (editor.mode != EXIT) {
        int key = readKey();

        editor.in_start = false;
        editor.save_curosr_col = true;

        handleKey(&parser, &editor, key);

        fixTextPosition(editor.cursor, &editor.text, editor.buffer.line_count);

        if (editor.save_curosr_col) {
            editor.prev_cursor_col = editor.cursor.col;
        }

        snprintf(editor.status_line, sizeof(editor.status_line), BGREEN "Key code: %d" RESET "\trow: %zu, col: %zu, prev_col: %zu, Mode: %d, l_c: %zu, l_cap: %zu, t_row: %zu, t_col: %zu", key, editor.cursor.row, editor.cursor.col, editor.prev_cursor_col, editor.mode, editor.buffer.line_count, editor.buffer.capacity, editor.text.row, editor.text.col);

        drawWindow();

        refreshWindow();
    }

    cleanup(&editor);
}

Line* getLine(size_t row) {
    return &editor.buffer.lines[row];
}

char* getStatusLine(void) {
    return editor.status_line;
}

char* getKeyCacheString(void) {
    char tmp[64];
    if (parser.cmd.count == 0) {
        if (parser.cmd.count_after_operator == 0) {
            snprintf(tmp, sizeof(tmp), "%s", parser.cmd.key_cache.chars);
        } else {
            snprintf(tmp, sizeof(tmp), "%c%zu%s", parser.cmd.key_cache.chars[0], parser.cmd.count_after_operator, parser.cmd.key_cache.chars + 1);
        }
    } else {
        if (parser.cmd.count_after_operator == 0) {
            snprintf(tmp, sizeof(tmp), "%zu%s", parser.cmd.count, parser.cmd.key_cache.chars);
        } else {
            snprintf(tmp, sizeof(tmp), "%zu%c%zu%s", parser.cmd.count, parser.cmd.key_cache.chars[0], parser.cmd.count_after_operator, parser.cmd.key_cache.chars + 1);
        }
    }

    memmove(editor.key_cache_string, tmp, KEY_CACHE_STRING_SIZE);
    editor.key_cache_string[KEY_CACHE_STRING_SIZE] = '\0';

    return editor.key_cache_string;
}

char* getCursorPositionString(void) {
    if (editor.cursor.col == 0 && getLine(editor.cursor.row)->chars[0] == '\0') {
        snprintf(editor.cursor_position_string, sizeof(editor.cursor_position_string), "%zu,0-1", editor.cursor.row + 1);
    } else {
        snprintf(editor.cursor_position_string, sizeof(editor.cursor_position_string), "%zu,%zu", editor.cursor.row + 1, editor.cursor.col + 1);
    }

    return editor.cursor_position_string;
}

size_t getLineCount(void) {
    return editor.buffer.line_count;
}

Position getCursorPosition(void) {
    return editor.cursor;
}

Position getTextPosition(void) {
    return editor.text;
}

bool isInStart(void) {
    return editor.in_start;
}
