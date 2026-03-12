#include <stdbool.h>
#include <stddef.h>

#include "terminal.h"
#include "vip.h"

typedef struct {
    Position start;
    Position end;
} Range;

void moveUp(Editor* editor);
void moveDown(Editor* editor);
void moveRight(Editor* editor);
void moveLeft(Editor* editor);

Range moveToTopOfScreen(Editor* editor);
Range moveToMiddleOfScreen(Editor* editor);
Range moveToBottomOfScreen(Editor* editor);
Range jumpForwardsToStartOfWord(Editor* editor);
Range jumpForwardsToStartOfBigWord(Editor* editor);
Range jumpForwardToEndOfWord(Editor* editor);
Range jumpForwardsToEndOfBigWord(Editor* editor);
Range jumpBackwardsToStartOfWord(Editor* editor);
Range jumpBackwardsToStartOfBigWord(Editor* editor);
Range jumpBackwardToEndOfWord(Editor* editor);
Range jumpBackwardsToEndOfBigWord(Editor* editor);
Range moveToMatchingChar(Editor* editor);
Range jumpToStartOfLine(Editor* editor);
Range jumpToFirstNonBlankCharOfLine(Editor* editor);
Range jumpToEndOfLine(Editor* editor);
Range jumpToLastNonBlankCharOfLine(Editor* editor);
Range goToFirstLine(Editor* editor);
Range goToLastLine(Editor* editor);
Range goToLine(Editor* editor, size_t row);
Range jumpToNextOccurrenceOfChar(Editor* editor, const char c);
Range jumpBeforeNextOccurrenceOfChar(Editor* editor, const char c);
Range jumpToPreviousOccurrenceOfChar(Editor* editor, const char c);
Range jumpAfterPreviousOccurrenceOfChar(Editor* editor, const char c);
Range repeatPreviousSearchMovement(Editor* editor);
Range repeatPreviousSearchMovementBackwards(Editor* editor);
Range jumpToNextParagraph(Editor* editor);
Range jumpToPreviousParagraph(Editor* editor);
Range insideWord(Editor* editor);
Range insideBigWord(Editor* editor);
Range outsideWord(Editor* editor);
Range outsideBigWord(Editor* editor);

void moveDownHalfPage(Editor* editor);
void moveUpHalfPage(Editor* editor);
