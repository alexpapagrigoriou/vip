#include "vip.h"

#include <signal.h>
#include <stdio.h>

#include "colors.h"
#include "draw.h"
#include "input.h"
#include "keymap.h"
#include "motion.h"
#include "status.h"

static Editor editor;

static void editorInit(Editor* editor, char* filename) {
    editor->buffer = createBuffer();
    editor->cursor = (Position){0, 0};
    editor->save_curosr_col = true;
    editor->prev_cursor_col = 0;
    editor->text = (Position){0, 0};
    editor->status_line = createStatusLine();
    editor->status_cursor_col = 0;
    editor->in_start = filename == NULL;
    editor->pending_operator = '\0';
    editor->mode = NORMAL;
    editor->filename = filename;
}

static void cleanup(Editor* editor) {
    disableRawMode();

    freeBuffer(editor->buffer);
    freeStatusLine(editor->status_line);

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

        handleKey(&editor, key);

        drawWindow();

        if (editor.save_curosr_col) {
            editor.prev_cursor_col = editor.cursor.col;
        }

        char status_line[128];
        snprintf(status_line, sizeof(status_line), BGREEN "Key code: %d" RESET "\trow: %zu, col: %zu, prev_col: %zu, Mode: %d", key, editor.cursor.row, editor.cursor.col, editor.prev_cursor_col, editor.mode);
        drawStatusLine(status_line);

        refreshWindow();
    }

    cleanup(&editor);
}

Line* getLine(size_t row) {
    return &editor.buffer->lines[row];
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
