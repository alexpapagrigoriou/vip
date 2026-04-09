#include "vip.h"

#include <signal.h>
#include <stdio.h>

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

        fixTextPosition(&editor);

        if (editor.save_curosr_col) {
            editor.prev_cursor_col = editor.cursor.col;
        }

        // TODO: take parser's key cache and output it to the status line
        snprintf(editor.status_line->chars, sizeof(editor.status_line->chars), BGREEN "Key code: %d" RESET "\trow: %zu, col: %zu, prev_col: %zu, Mode: %d, max_row: %zu, max_col: %zu, t_row: %zu, t_col: %zu", key, editor.cursor.row, editor.cursor.col, editor.prev_cursor_col, editor.mode, getMaxScreen().row, getMaxScreen().col, editor.text.row, editor.text.col);

        drawWindow();

        refreshWindow();
    }

    cleanup(&editor);
}

Line* getLine(size_t row) {
    return &editor.buffer->lines[row];
}

StatusLine* getStatusLine(void) {
    return editor.status_line;
}

size_t getLineCount(void) {
    return editor.buffer->line_count;
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
