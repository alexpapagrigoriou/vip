#include "movement.h"

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
    size_t new_col = moveRight(editor, count);
    if (editor->mode == INSERT) {
        if (new_col < editor->cursor.col) {
            editor->save_curosr_col = false;
            return;
        }

        editor->cursor.col++;
        return;
    }

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
