#include "motion.h"

#include "editor.h"
#include "vip.h"

MotionType getMotionType(Motion motion) {
    switch (motion) {
        case MOT_UP:
        case MOT_DOWN:
        case MOT_LINE:
        case MOT_FIRST_LINE:
        case MOT_LAST_LINE:
        case MOT_NEXT_PARAGRAPH:
        case MOT_PREVIOUS_PARAGRAPH:
            return MOT_TYPE_ROW;
        case MOT_LEFT:
        case MOT_RIGHT:
        case MOT_WORD:
        case MOT_BIG_WORD:
        case MOT_END_WORD:
        case MOT_END_BIG_WORD:
        case MOT_BACKWARD_WORD:
        case MOT_BACKWARD_BIG_WORD:
        case MOT_START_OF_LINE:
        case MOT_FIRST_NON_BLANK_CHAR_OF_LINE:
        case MOT_END_OF_LINE:
        case MOT_NEXT_OCCURRENCE_OF_CHAR:
        case MOT_BEFORE_NEXT_OCCURRENCE_OF_CHAR:
        case MOT_PREVIOUS_OCCURRENCE_OF_CHAR:
        case MOT_AFTER_PREVIOUS_OCCURRENCE_OF_CHAR:
            return MOT_TYPE_COL;
        case MOT_MATCHING_CHAR:
            return MOT_TYPE_POSITION;
        default:
            return MOT_TYPE_NONE;
    }
}

// TODO: set editor->successfu_motion to false when the motion can't be executed
//       ex. find next a. (fa) -> there is not a in this line

size_t moveUp(Editor* editor, const size_t count) {
    if (count >= editor->cursor.row) {
        return 0;
    }

    return editor->cursor.row - count;
}

size_t moveDown(Editor* editor, const size_t count) {
    if (editor->cursor.row + count >= getLineCount() - 1) {
        return getLineCount() - 1;
    }

    return editor->cursor.row + count;
}

size_t goToLine(size_t row) {
    row--;

    if (row > getLineCount() - 1) {
        return getLineCount() - 1;
    }

    return row;
}

size_t goToFirstLine(const size_t count) {
    if (count > 1) {
        return goToLine(count);
    }

    return 0;
}

size_t goToLastLine(const size_t count) {
    if (count > 1) {
        return goToLine(count);
    }

    return getLineCount() - 1;
}

size_t jumpToNextParagraph(Editor* editor, const size_t count) {
    // TODO: implement
    (void)editor;
    (void)count;

    return 0;
}

size_t jumpToPreviousParagraph(Editor* editor, const size_t count) {
    // TODO: implement
    (void)editor;
    (void)count;

    return 0;
}

size_t moveLeft(Editor* editor, const size_t count) {
    if (count >= editor->cursor.col) {
        return 0;
    }

    return editor->cursor.col - count;
}

size_t moveRight(Editor* editor, const size_t count) {
    if (getLine(editor->cursor.row)->length == 0) {
        return 0;
    }

    if (editor->cursor.col + count >= getLine(editor->cursor.row)->length - 1) {
        return getLine(editor->cursor.row)->length - 1;
    }

    return editor->cursor.col + count;
}

size_t jumpForwardsToStartOfWord(Editor* editor, const size_t count) {
    // TODO: implement
    (void)editor;
    (void)count;

    return 0;
}

size_t jumpForwardsToStartOfBigWord(Editor* editor, const size_t count) {
    // TODO: implement
    (void)editor;
    (void)count;

    return 0;
}

size_t jumpForwardToEndOfWord(Editor* editor, const size_t count) {
    // TODO: implement
    (void)editor;
    (void)count;

    return 0;
}

size_t jumpForwardsToEndOfBigWord(Editor* editor, const size_t count) {
    // TODO: implement
    (void)editor;
    (void)count;

    return 0;
}

size_t jumpBackwardsToStartOfWord(Editor* editor, const size_t count) {
    // TODO: implement
    (void)editor;
    (void)count;

    return 0;
}

size_t jumpBackwardsToStartOfBigWord(Editor* editor, const size_t count) {
    // TODO: implement
    (void)editor;
    (void)count;

    return 0;
}

size_t jumpToStartOfLine() {
    return 0;
}

size_t jumpToFirstNonBlankCharOfLine(Editor* editor) {
    // TODO: implement
    (void)editor;

    return 0;
}

size_t jumpToEndOfLine(Editor* editor) {
    return getLine(editor->cursor.row)->length - 1;
}

size_t jumpToNextOccurrenceOfChar(Editor* editor, const size_t count, const char c) {
    // TODO: implement
    (void)editor;
    (void)count;
    (void)c;

    return 0;
}

size_t jumpBeforeNextOccurrenceOfChar(Editor* editor, const size_t count, const char c) {
    // TODO: implement
    (void)editor;
    (void)count;
    (void)c;

    return 0;
}

size_t jumpToPreviousOccurrenceOfChar(Editor* editor, const size_t count, const char c) {
    // TODO: implement
    (void)editor;
    (void)count;
    (void)c;

    return 0;
}

size_t jumpAfterPreviousOccurrenceOfChar(Editor* editor, const size_t count, const char c) {
    // TODO: implement
    (void)editor;
    (void)count;
    (void)c;

    return 0;
}

Position jumpToMatchingChar(Editor* editor) {
    // TODO: implement
    (void)editor;

    return (Position){0, 0};
}
