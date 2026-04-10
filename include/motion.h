#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "editor.h"
#include "terminal.h"

typedef enum {
    MOT_NONE,

    MOT_UP,
    MOT_DOWN,
    MOT_LINE,
    MOT_FIRST_LINE,
    MOT_LAST_LINE,
    MOT_NEXT_PARAGRAPH,
    MOT_PREVIOUS_PARAGRAPH,

    MOT_LEFT,
    MOT_RIGHT,
    MOT_WORD,
    MOT_BIG_WORD,
    MOT_END_WORD,
    MOT_END_BIG_WORD,
    MOT_BACKWARD_WORD,
    MOT_BACKWARD_BIG_WORD,
    MOT_START_OF_LINE,
    MOT_FIRST_NON_BLANK_CHAR_OF_LINE,
    MOT_END_OF_LINE,
    MOT_NEXT_OCCURRENCE_OF_CHAR,
    MOT_BEFORE_NEXT_OCCURRENCE_OF_CHAR,
    MOT_PREVIOUS_OCCURRENCE_OF_CHAR,
    MOT_AFTER_PREVIOUS_OCCURRENCE_OF_CHAR,

    MOT_MATCHING_CHAR,

    MOT_FIRST_LINE_PENDING,
    MOT_NEXT_OCCURRENCE_OF_CHAR_PENDING,
    MOT_BEFORE_NEXT_OCCURRENCE_OF_CHAR_PENDING,
    MOT_PREVIOUS_OCCURRENCE_OF_CHAR_PENDING,
    MOT_AFTER_PREVIOUS_OCCURRENCE_OF_CHAR_PENDING
} Motion;

typedef enum {
    MOT_TYPE_NONE,
    MOT_TYPE_ROW,
    MOT_TYPE_COL,
    MOT_TYPE_POSITION
} MotionType;

MotionType getMotionType(Motion motion);

size_t moveUp(Editor* editor, const size_t count);
size_t moveDown(Editor* editor, const size_t count);
size_t goToLine(size_t row);
size_t goToFirstLine(const size_t count);
size_t goToLastLine(const size_t count);
size_t jumpToNextParagraph(Editor* editor, const size_t count);
size_t jumpToPreviousParagraph(Editor* editor, const size_t count);

size_t moveLeft(Editor* editor, const size_t count);
size_t moveRight(Editor* editor, const size_t count);
size_t jumpForwardsToStartOfWord(Editor* editor, const size_t count);
size_t jumpForwardsToStartOfBigWord(Editor* editor, const size_t count);
size_t jumpForwardToEndOfWord(Editor* editor, const size_t count);
size_t jumpForwardsToEndOfBigWord(Editor* editor, const size_t count);
size_t jumpBackwardsToStartOfWord(Editor* editor, const size_t count);
size_t jumpBackwardsToStartOfBigWord(Editor* editor, const size_t count);
size_t jumpToStartOfLine();
size_t jumpToFirstNonBlankCharOfLine(Editor* editor);
size_t jumpToEndOfLine(Editor* editor);
size_t jumpToNextOccurrenceOfChar(Editor* editor, const size_t count, const char c);
size_t jumpBeforeNextOccurrenceOfChar(Editor* editor, const size_t count, const char c);
size_t jumpToPreviousOccurrenceOfChar(Editor* editor, const size_t count, const char c);
size_t jumpAfterPreviousOccurrenceOfChar(Editor* editor, const size_t count, const char c);

Position jumpToMatchingChar(Editor* editor);
