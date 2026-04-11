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

static void movementLeft(Editor* editor, Motion motion, const size_t count) {
    if (editor->cursor.col == 0) {
        editor->save_curosr_col = false;
        return;
    }

    editor->cursor.col = getMotionColLeft(editor, motion, count, '\0');
}

static void movementUp(Editor* editor, Motion motion, const size_t count) {
    editor->save_curosr_col = false;

    size_t new_row = getMotionRow(editor, motion, count);
    if (new_row == editor->cursor.row) {
        return;
    }

    editor->cursor.row = new_row;

    fixCursorCol(editor);
}

static void movementDown(Editor* editor, Motion motion, const size_t count) {
    editor->save_curosr_col = false;

    size_t new_row = getMotionRow(editor, motion, count);
    if (new_row == editor->cursor.row) {
        return;
    }

    editor->cursor.row = new_row;

    fixCursorCol(editor);
}

static void movementRight(Editor* editor, Motion motion, const size_t count) {
    if (editor->mode == INSERT) {
        if (editor->cursor.col == getLine(editor->cursor.row)->length) {
            editor->save_curosr_col = false;
            return;
        }

        editor->cursor.col++;
        return;
    }

    size_t new_col = getMotionColRight(editor, motion, count, '\0');
    if (new_col == editor->cursor.col) {
        editor->save_curosr_col = false;
        return;
    }

    editor->cursor.col = new_col;
}

static void movementFirstLine(Editor* editor, Motion motion, const size_t count) {
    editor->save_curosr_col = false;

    editor->cursor.row = getMotionRow(editor, motion, count);

    fixCursorCol(editor);
}

static void movementLastLine(Editor* editor, Motion motion, const size_t count) {
    editor->save_curosr_col = false;

    editor->cursor.row = getMotionRow(editor, motion, count);

    fixCursorCol(editor);
}

static void movementEndOfLine(Editor* editor, Motion motion) {
    editor->save_curosr_col = false;
    editor->prev_cursor_col = SIZE_MAX;

    editor->cursor.col = getMotionColRight(editor, motion, 1, '\0');
}

bool isMovementToExecute(Editor* editor, Motion motion, const size_t count) {
    switch (motion) {
        case MOT_LEFT:
            movementLeft(editor, motion, count);
            break;
        case MOT_UP:
            movementUp(editor, motion, count);
            break;
        case MOT_DOWN:
            movementDown(editor, motion, count);
            break;
        case MOT_RIGHT:
            movementRight(editor, motion, count);
            break;
        case MOT_FIRST_LINE:
            movementFirstLine(editor, motion, count);
            break;
        case MOT_LAST_LINE:
            movementLastLine(editor, motion, count);
            break;
        case MOT_END_OF_LINE:
            movementEndOfLine(editor, motion);
            break;
        default:
            return false;
    }

    return true;
}

void movementLine(Editor* editor, size_t row) {
    editor->save_curosr_col = false;

    editor->cursor.row = getMotionRow(editor, MOT_LINE, row);

    fixCursorCol(editor);
}

void insertArrowLeft(Editor* editor) {
    movementLeft(editor, MOT_LEFT, 1);
}

void insertArrowUp(Editor* editor) {
    movementUp(editor, MOT_UP, 1);
}

void insertArrowDown(Editor* editor) {
    movementDown(editor, MOT_DOWN, 1);
}

void insertArrowRight(Editor* editor) {
    movementRight(editor, MOT_RIGHT, 1);
}
