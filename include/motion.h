#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "editor.h"
#include "terminal.h"

typedef enum {
    MOT_NONE,
    MOT_LEFT,
    MOT_DOWN,
    MOT_UP,
    MOT_RIGHT,
    MOT_LINE,
    MOT_WORD,
    MOT_BIG_WORD,
    MOT_END_WORD,
    MOT_END_BIG_WORD,
    MOT_BACKWARD_WORD,
    MOT_BACKWARD_BIG_WORD,
    MOT_START_OF_LINE,
    MOT_FIRST_NON_BLANK_CHAR_OF_LINE,
    MOT_END_OF_LINE,
    MOT_LAST_LINE,
    MOT_NEXT_PARAGRAPH,
    MOT_PREVIOUS_PARAGRAPH,

    MOT_FIRST_LINE_PENDING,
    MOT_NEXT_OCCURRENCE_OF_CHAR_PENDING,
    MOT_BEFORE_NEXT_OCCURRENCE_OF_CHAR_PENDING,
    MOT_PREVIOUS_OCCURRENCE_OF_CHAR_PENDING,
    MOT_AFTER_PREVIOUS_OCCURRENCE_OF_CHAR_PENDING,

    MOT_FIRST_LINE,
    MOT_NEXT_OCCURRENCE_OF_CHAR,
    MOT_BEFORE_NEXT_OCCURRENCE_OF_CHAR,
    MOT_PREVIOUS_OCCURRENCE_OF_CHAR,
    MOT_AFTER_PREVIOUS_OCCURRENCE_OF_CHAR
} Motion;

typedef struct {
    Position start;
    Position end;
} Range;

void moveLeft(Editor* editor);
void moveUp(Editor* editor);
void moveDown(Editor* editor);
void moveRight(Editor* editor);

Range jumpForwardsToStartOfWord(Editor* editor);
Range jumpForwardsToStartOfBigWord(Editor* editor);
Range jumpForwardToEndOfWord(Editor* editor);
Range jumpForwardsToEndOfBigWord(Editor* editor);
Range jumpBackwardsToStartOfWord(Editor* editor);
Range jumpBackwardsToStartOfBigWord(Editor* editor);
Range jumpToStartOfLine(Editor* editor);
Range jumpToFirstNonBlankCharOfLine(Editor* editor);
Range jumpToEndOfLine(Editor* editor);
Range jumpToLastNonBlankCharOfLine(Editor* editor);
Range goToLastLine(Editor* editor);
Range goToLine(Editor* editor, size_t row);
Range jumpToNextParagraph(Editor* editor);
Range jumpToPreviousParagraph(Editor* editor);
Range goToFirstLine(Editor* editor);
Range jumpToNextOccurrenceOfChar(Editor* editor, const char c);
Range jumpBeforeNextOccurrenceOfChar(Editor* editor, const char c);
Range jumpToPreviousOccurrenceOfChar(Editor* editor, const char c);
Range jumpAfterPreviousOccurrenceOfChar(Editor* editor, const char c);
