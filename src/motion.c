#include "motion.h"

void moveUp(Editor* editor) {
    editor->save_curosr_col = false;

    if (editor->cursor.row > 0) {
        editor->cursor.row--;

        size_t line_length = getLine(editor->cursor.row)->length;
        if (editor->mode != INSERT && line_length > 0) {
            line_length--;
        }

        if (editor->prev_cursor_col < line_length) {
            editor->cursor.col = editor->prev_cursor_col;
        } else {
            editor->cursor.col = line_length;
        }
    }
}

void moveDown(Editor* editor) {
    editor->save_curosr_col = false;

    if (editor->cursor.row < getLineCount() - 1) {
        editor->cursor.row++;

        size_t line_length = getLine(editor->cursor.row)->length;
        if (editor->mode != INSERT && line_length > 0) {
            line_length--;
        }

        if (editor->prev_cursor_col < line_length) {
            editor->cursor.col = editor->prev_cursor_col;
        } else {
            editor->cursor.col = line_length;
        }
    }
}

void moveRight(Editor* editor) {
    size_t line_length = getLine(editor->cursor.row)->length;
    if (editor->mode != INSERT && line_length > 0) {
        line_length--;
    }

    if (editor->cursor.col < line_length) {
        editor->cursor.col++;
    } else {
        editor->save_curosr_col = false;
    }
}

void moveLeft(Editor* editor) {
    if (editor->cursor.col > 0) {
        editor->cursor.col--;
    } else {
        editor->save_curosr_col = false;
    }
}

Range jumpForwardsToStartOfWord(Editor* editor) {
    (void)editor;
    return (Range){(Position){0, 0}, (Position){0, 0}};
}

Range jumpForwardsToStartOfBigWord(Editor* editor) {
    (void)editor;
    return (Range){(Position){0, 0}, (Position){0, 0}};
}

Range jumpForwardToEndOfWord(Editor* editor) {
    (void)editor;
    return (Range){(Position){0, 0}, (Position){0, 0}};
}

Range jumpForwardsToEndOfBigWord(Editor* editor) {
    (void)editor;
    return (Range){(Position){0, 0}, (Position){0, 0}};
}

Range jumpBackwardsToStartOfWord(Editor* editor) {
    (void)editor;
    return (Range){(Position){0, 0}, (Position){0, 0}};
}

Range jumpBackwardsToStartOfBigWord(Editor* editor) {
    (void)editor;
    return (Range){(Position){0, 0}, (Position){0, 0}};
}

Range jumpBackwardToEndOfWord(Editor* editor) {
    (void)editor;
    return (Range){(Position){0, 0}, (Position){0, 0}};
}

Range jumpBackwardsToEndOfBigWord(Editor* editor) {
    (void)editor;
    return (Range){(Position){0, 0}, (Position){0, 0}};
}

Range moveToMatchingChar(Editor* editor) {
    (void)editor;
    return (Range){(Position){0, 0}, (Position){0, 0}};
}

Range jumpToStartOfLine(Editor* editor) {
    (void)editor;
    return (Range){(Position){0, 0}, (Position){0, 0}};
}

Range jumpToFirstNonBlankCharOfLine(Editor* editor) {
    (void)editor;
    return (Range){(Position){0, 0}, (Position){0, 0}};
}

Range jumpToEndOfLine(Editor* editor) {
    (void)editor;
    return (Range){(Position){0, 0}, (Position){0, 0}};
}

Range jumpToLastNonBlankCharOfLine(Editor* editor) {
    (void)editor;
    return (Range){(Position){0, 0}, (Position){0, 0}};
}

Range goToFirstLine(Editor* editor) {
    (void)editor;
    return (Range){(Position){0, 0}, (Position){0, 0}};
}

Range goToLastLine(Editor* editor) {
    (void)editor;
    return (Range){(Position){0, 0}, (Position){0, 0}};
}

Range goToLine(Editor* editor, size_t row) {
    (void)editor;
    (void)row;
    return (Range){(Position){0, 0}, (Position){0, 0}};
}

Range jumpToNextOccurrenceOfChar(Editor* editor, const char c) {
    (void)editor;
    (void)c;
    return (Range){(Position){0, 0}, (Position){0, 0}};
}

Range jumpBeforeNextOccurrenceOfChar(Editor* editor, const char c) {
    (void)editor;
    (void)c;
    return (Range){(Position){0, 0}, (Position){0, 0}};
}

Range jumpToPreviousOccurrenceOfChar(Editor* editor, const char c) {
    (void)editor;
    (void)c;
    return (Range){(Position){0, 0}, (Position){0, 0}};
}

Range jumpAfterPreviousOccurrenceOfChar(Editor* editor, const char c) {
    (void)editor;
    (void)c;
    return (Range){(Position){0, 0}, (Position){0, 0}};
}

Range repeatPreviousSearchMovement(Editor* editor) {
    (void)editor;
    return (Range){(Position){0, 0}, (Position){0, 0}};
}

Range repeatPreviousSearchMovementBackwards(Editor* editor) {
    (void)editor;
    return (Range){(Position){0, 0}, (Position){0, 0}};
}

Range jumpToNextParagraph(Editor* editor) {
    (void)editor;
    return (Range){(Position){0, 0}, (Position){0, 0}};
}

Range jumpToPreviousParagraph(Editor* editor) {
    (void)editor;
    return (Range){(Position){0, 0}, (Position){0, 0}};
}

Range insideWord(Editor* editor) {
    (void)editor;
    return (Range){(Position){0, 0}, (Position){0, 0}};
}

Range insideBigWord(Editor* editor) {
    (void)editor;
    return (Range){(Position){0, 0}, (Position){0, 0}};
}

Range outsideWord(Editor* editor) {
    (void)editor;
    return (Range){(Position){0, 0}, (Position){0, 0}};
}

Range outsideBigWord(Editor* editor) {
    (void)editor;
    return (Range){(Position){0, 0}, (Position){0, 0}};
}

void moveDownHalfPage(Editor* editor) {
    (void)editor;
}

void moveUpHalfPage(Editor* editor) {
    (void)editor;
}
