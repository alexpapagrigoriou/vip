#include "motion.h"

#include "editor.h"
#include "error.h"
#include "keys.h"
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
        case MOT_BACKWARD_WORD:
        case MOT_BACKWARD_BIG_WORD:
        case MOT_START_OF_LINE:
        case MOT_FIRST_NON_BLANK_CHAR_OF_LINE:
        case MOT_PREVIOUS_OCCURRENCE_OF_CHAR:
        case MOT_AFTER_PREVIOUS_OCCURRENCE_OF_CHAR:
            return MOT_TYPE_COL_LEFT;
        case MOT_RIGHT:
        case MOT_WORD:
        case MOT_BIG_WORD:
        case MOT_END_WORD:
        case MOT_END_BIG_WORD:
        case MOT_END_OF_LINE:
        case MOT_NEXT_OCCURRENCE_OF_CHAR:
        case MOT_BEFORE_NEXT_OCCURRENCE_OF_CHAR:
            return MOT_TYPE_COL_RIGHT;
        case MOT_MATCHING_CHAR:
            return MOT_TYPE_POSITION;
        default:
            ERROR("Wrong motion");
            return MOT_TYPE_NONE;
    }
}

bool isWordSplit(char c) {
    return !((IS_DIGIT(c)) || (IS_UPPER_CASE_CHAR(c)) || (IS_LOWER_CASE_CHAR(c)));
}

bool isBigWordSplit(char c) {
    return c == SPACE;
}

// TODO: set editor->successful_motion to false when the motion can't be executed
//       ex. find next a. (fa) -> there is not a in this line

static size_t motionUp(Editor* editor, const size_t count) {
    if (editor->cursor.row == 0) {
        editor->successful_motion = false;
        return 0;
    }

    if (count >= editor->cursor.row) {
        return 0;
    }

    return editor->cursor.row - count;
}

static size_t motionDown(Editor* editor, const size_t count) {
    if (editor->cursor.row == getLineCount() - 1) {
        editor->successful_motion = false;
        return getLineCount() - 1;
    }

    if (editor->cursor.row + count >= getLineCount() - 1) {
        return getLineCount() - 1;
    }

    return editor->cursor.row + count;
}

static size_t motionLine(size_t row) {
    row--;

    if (row > getLineCount() - 1) {
        return getLineCount() - 1;
    }

    return row;
}

static size_t motionFirstLine(const size_t count) {
    if (count > 1) {
        return motionLine(count);
    }

    return 0;
}

static size_t motionLastLine(const size_t count) {
    if (count > 1) {
        return motionLine(count);
    }

    return getLineCount() - 1;
}

static size_t motionNextParagraph(Editor* editor, const size_t count) {
    editor->cursor.col = 0;

    size_t line_count = editor->buffer.line_count;

    if (editor->cursor.row == line_count - 1) {
        editor->successful_motion = false;
        return line_count - 1;
    }

    size_t found = 0;

    for (size_t i = editor->cursor.row + 1; i < line_count - 1; i++) {
        if (getLine(i)->length == 0) {
            found++;

            if (found == count) {
                return i;
            }
        }
    }

    return line_count - 1;
}

static size_t motionPreviousParagraph(Editor* editor, const size_t count) {
    editor->cursor.col = 0;

    if (editor->cursor.row == 0) {
        editor->successful_motion = false;
        return 0;
    }

    size_t found = 0;

    for (size_t i = editor->cursor.row - 1; i > 0; i--) {
        if (getLine(i)->length == 0) {
            found++;

            if (found == count) {
                return i;
            }
        }
    }

    return 0;
}

static size_t motionLeft(Editor* editor, const size_t count) {
    if (editor->cursor.col == 0) {
        editor->successful_motion = false;
        return 0;
    }

    if (count >= editor->cursor.col) {
        return 0;
    }

    return editor->cursor.col - count;
}

static size_t motionBackwardWord(Editor* editor, const size_t count) {
    if (editor->cursor.col == 0) {
        editor->successful_motion = false;
        return 0;
    }

    Line* line = getLine(editor->cursor.row);
    size_t found = 0;

    size_t prev_col = editor->cursor.col;
    while (found < count) {
        prev_col--;
        if (prev_col == 0) {
            return 0;
        }

        if (isWordSplit(line->chars[prev_col]) && line->chars[prev_col] != SPACE) {
            found++;
            continue;
        }

        bool foundWord = false;
        for (size_t i = prev_col - 1; i > 0; i--) {
            if (isWordSplit(line->chars[i])) {
                foundWord = true;
                prev_col = i + 1;
                found++;
                break;
            }
        }

        if (!foundWord) {
            return 0;
        }
    }

    return prev_col;
}

static size_t motionBackwardBigWord(Editor* editor, const size_t count) {
    if (editor->cursor.col == 0) {
        editor->successful_motion = false;
        return 0;
    }

    Line* line = getLine(editor->cursor.row);
    size_t found = 0;

    size_t prev_col = editor->cursor.col;
    while (found < count) {
        prev_col--;
        if (prev_col == 0) {
            return 0;
        }

        bool foundWord = false;
        for (size_t i = prev_col - 1; i > 0; i--) {
            if (isBigWordSplit(line->chars[i])) {
                foundWord = true;
                prev_col = i + 1;
                found++;
                break;
            }
        }

        if (!foundWord) {
            return 0;
        }
    }

    return prev_col;
}

static size_t motionStartOfLine(Editor* editor) {
    if (editor->cursor.col == 0) {
        editor->successful_motion = false;
        return 0;
    }

    return 0;
}

static size_t motionFirstNonBlankCharOfLine(Editor* editor) {
    Line* line = getLine(editor->cursor.row);

    for (size_t i = 0; i < line->length - 1; i++) {
        if (line->chars[i] != SPACE) {
            return i;
        }
    }

    return line->length - 1;
}

static size_t motionPreviousOccurrenceOfChar(Editor* editor, const size_t count, const char c) {
    // TODO: implement
    (void)editor;
    (void)count;
    (void)c;

    return 0;
}

static size_t motionAfterPreviousOccurrenceOfChar(Editor* editor, const size_t count, const char c) {
    // TODO: implement
    (void)editor;
    (void)count;
    (void)c;

    return 0;
}

static size_t motionRight(Editor* editor, const size_t count) {
    if (editor->cursor.col + count >= getLine(editor->cursor.row)->length - 1) {
        return getLine(editor->cursor.row)->length - 1;
    }

    return editor->cursor.col + count;
}

static size_t motionWord(Editor* editor, const size_t count) {
    // TODO: implement
    (void)editor;
    (void)count;

    return 0;
}

static size_t motionBigWord(Editor* editor, const size_t count) {
    // TODO: implement
    (void)editor;
    (void)count;

    return 0;
}

static size_t motionEndWord(Editor* editor, const size_t count) {
    // TODO: implement
    (void)editor;
    (void)count;

    return 0;
}

static size_t motionEndBigWord(Editor* editor, const size_t count) {
    // TODO: implement
    (void)editor;
    (void)count;

    return 0;
}

static size_t motionEndOfLine(Editor* editor) {
    return getLine(editor->cursor.row)->length - 1;
}

static size_t motionNextOccurrenceOfChar(Editor* editor, const size_t count, const char c) {
    // TODO: implement
    (void)editor;
    (void)count;
    (void)c;

    return 0;
}

static size_t motionBeforeNextOccurrenceOfChar(Editor* editor, const size_t count, const char c) {
    // TODO: implement
    (void)editor;
    (void)count;
    (void)c;

    return 0;
}

static Position motionMatchingChar(Editor* editor) {
    // TODO: implement
    (void)editor;

    return (Position){0, 0};
}

size_t getMotionRow(Editor* editor, Motion motion, const size_t count) {
    switch (motion) {
        case MOT_UP:
            return motionUp(editor, count);
        case MOT_DOWN:
            return motionDown(editor, count);
        case MOT_LINE:
            return motionLine(editor->cursor.row + count);
        case MOT_FIRST_LINE:
            return motionFirstLine(count);
        case MOT_LAST_LINE:
            return motionLastLine(count);
        case MOT_NEXT_PARAGRAPH:
            return motionNextParagraph(editor, count);
        case MOT_PREVIOUS_PARAGRAPH:
            return motionPreviousParagraph(editor, count);
        default:
            ERROR("Wrong motion");
            return 0;
    }
}

size_t getMotionCol(Editor* editor, Motion motion) {
    switch (motion) {
        case MOT_START_OF_LINE:
            return motionStartOfLine(editor);
        case MOT_FIRST_NON_BLANK_CHAR_OF_LINE:
            return motionFirstNonBlankCharOfLine(editor);
        case MOT_END_OF_LINE:
            return motionEndOfLine(editor);
        default:
            ERROR("Wrong motion");
            return 0;
    }
}

size_t getMotionColLeft(Editor* editor, Motion motion, const size_t count, const char c) {
    if (editor->buffer.lines[editor->cursor.row].length == 0) {
        editor->successful_motion = false;
        return 0;
    }

    switch (motion) {
        case MOT_LEFT:
            return motionLeft(editor, count);
        case MOT_BACKWARD_WORD:
            return motionBackwardWord(editor, count);
        case MOT_BACKWARD_BIG_WORD:
            return motionBackwardBigWord(editor, count);
        case MOT_START_OF_LINE:
            return motionStartOfLine(editor);
        case MOT_FIRST_NON_BLANK_CHAR_OF_LINE:
            return motionFirstNonBlankCharOfLine(editor);
        case MOT_PREVIOUS_OCCURRENCE_OF_CHAR:
            return motionPreviousOccurrenceOfChar(editor, count, c);
        case MOT_AFTER_PREVIOUS_OCCURRENCE_OF_CHAR:
            return motionAfterPreviousOccurrenceOfChar(editor, count, c);
        default:
            ERROR("Wrong motion");
            return 0;
    }
}

size_t getMotionColRight(Editor* editor, Motion motion, const size_t count, const char c) {
    if (editor->buffer.lines[editor->cursor.row].length == 0) {
        editor->successful_motion = false;
        return 0;
    }

    switch (motion) {
        case MOT_RIGHT:
            return motionRight(editor, count);
        case MOT_WORD:
            return motionWord(editor, count);
        case MOT_BIG_WORD:
            return motionBigWord(editor, count);
        case MOT_END_WORD:
            return motionEndWord(editor, count);
        case MOT_END_BIG_WORD:
            return motionEndBigWord(editor, count);
        case MOT_END_OF_LINE:
            return motionEndOfLine(editor);
        case MOT_NEXT_OCCURRENCE_OF_CHAR:
            return motionNextOccurrenceOfChar(editor, count, c);
        case MOT_BEFORE_NEXT_OCCURRENCE_OF_CHAR:
            return motionBeforeNextOccurrenceOfChar(editor, count, c);
        default:
            ERROR("Wrong motion");
            return 0;
    }
}

Position getMotionPosition(Editor* editor, Motion motion, const size_t count) {
    (void)count;

    switch (motion) {
        case MOT_MATCHING_CHAR:
            return motionMatchingChar(editor);
        default:
            ERROR("Wrong motion");
            return (Position){0, 0};
    }
}
