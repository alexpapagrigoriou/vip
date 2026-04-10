#include "movement.h"

#include <stdint.h>

#include "motion.h"
#include "vip.h"

static void fixCursorCol(Editor* editor) {
    size_t line_length = getLine(editor->cursor.row)->length;
    if (editor->mode != INSERT && line_length > 0) {
        line_length--;
    }

    if (editor->prev_cursor_col >= line_length) {
        editor->cursor.col = line_length;
        return;
    }

    editor->cursor.col = editor->prev_cursor_col;
}

void movementLeft(Editor* editor, const size_t count) {
    if (editor->cursor.col == 0) {
        editor->save_curosr_col = false;
        return;
    }

    editor->cursor.col = moveLeft(editor, count);
}

void movementUp(Editor* editor, const size_t count) {
    editor->save_curosr_col = false;

    size_t new_row = moveUp(editor, count);
    if (new_row == editor->cursor.row) {
        return;
    }

    editor->cursor.row = new_row;

    fixCursorCol(editor);
}

void movementDown(Editor* editor, const size_t count) {
    editor->save_curosr_col = false;

    size_t new_row = moveDown(editor, count);
    if (new_row == editor->cursor.row) {
        return;
    }

    editor->cursor.row = new_row;

    fixCursorCol(editor);
}

void movementRight(Editor* editor, const size_t count) {
    if (editor->mode == INSERT) {
        if (editor->cursor.col == getLine(editor->cursor.row)->length) {
            editor->save_curosr_col = false;
            return;
        }

        editor->cursor.col++;
        return;
    }

    size_t new_col = moveRight(editor, count);
    if (new_col == editor->cursor.col) {
        editor->save_curosr_col = false;
        return;
    }

    editor->cursor.col = new_col;
}

void insertArrowLeft(Editor* editor) {
    movementLeft(editor, 1);
}

void insertArrowUp(Editor* editor) {
    movementUp(editor, 1);
}

void insertArrowDown(Editor* editor) {
    movementDown(editor, 1);
}

void insertArrowRight(Editor* editor) {
    movementRight(editor, 1);
}

void movementLine(Editor* editor, size_t row) {
    editor->save_curosr_col = false;

    editor->cursor.row = goToLine(row);

    fixCursorCol(editor);
}

void movementFirstLine(Editor* editor, const size_t count) {
    editor->save_curosr_col = false;

    editor->cursor.row = goToFirstLine(count);

    fixCursorCol(editor);
}

void movementLastLine(Editor* editor, const size_t count) {
    editor->save_curosr_col = false;

    editor->cursor.row = goToLastLine(count);

    fixCursorCol(editor);
}

void movementEndOfLine(Editor* editor) {
    editor->save_curosr_col = false;
    editor->prev_cursor_col = SIZE_MAX;

    editor->cursor.col = jumpToEndOfLine(editor);
}
