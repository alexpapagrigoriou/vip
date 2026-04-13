#include "normal.h"

#include <stdbool.h>

#include "error.h"
#include "input.h"
#include "movement.h"

static void fixCount(Parser* parser) {
    if (parser->cmd.count == 0) {
        parser->cmd.count = 1;
    }

    if (parser->cmd.count_after_operator == 0) {
        parser->cmd.count_after_operator = 1;
    }

    parser->cmd.count = parser->cmd.count * parser->cmd.count_after_operator;
}

static void executeRowMotion(Parser* parser, Editor* editor) {
    size_t row = getMotionRow(editor, parser->cmd.motion, parser->cmd.count);

    if (!editor->successful_motion) {
        editor->save_curosr_col = false;
        return;
    }

    switch (parser->cmd.operator) {
        case OP_NONE:
            editor->cursor.row = row;
            break;
        case OP_DELETE:
            deleteRow(&editor->buffer, &editor->cursor, row);
            break;
        case OP_CHANGE:
            bool is_last_line = editor->cursor.row == editor->buffer.line_count - 1 || row == editor->buffer.line_count - 1;
            deleteRow(&editor->buffer, &editor->cursor, row);

            if (is_last_line) {
                appendLine(&editor->buffer, &editor->cursor);
            } else {
                prependLine(&editor->buffer, &editor->cursor);
            }
            break;
        case OP_YANK:
            // TODO: implement yank row motion
            break;
        default:
            ERROR("Wrong operator");
    }
}

static void executeColLeftMotion(Parser* parser, Editor* editor) {
    size_t col = getMotionColLeft(editor, parser->cmd.motion, parser->cmd.count, parser->cmd.argument);

    if (!editor->successful_motion) {
        editor->save_curosr_col = false;
        return;
    }

    // TODO: handle the motion based on the operator
    (void)col;

    switch (parser->cmd.operator) {
        case OP_NONE:
            break;
        case OP_DELETE:
            break;
        case OP_CHANGE:
            break;
        case OP_YANK:
            break;
        default:
            ERROR("Wrong operator");
    }
}

static void executeColRightMotion(Parser* parser, Editor* editor) {
    size_t col = getMotionColRight(editor, parser->cmd.motion, parser->cmd.count, parser->cmd.argument);

    if (!editor->successful_motion) {
        editor->save_curosr_col = false;
        return;
    }

    // TODO: handle the motion based on the operator
    // !!! IMPORTANT !!!: make the closed interval to half open with col++ if the motion needs it
    (void)col;

    switch (parser->cmd.operator) {
        case OP_NONE:
            break;
        case OP_DELETE:
            break;
        case OP_CHANGE:
            break;
        case OP_YANK:
            break;
        default:
            ERROR("Wrong operator");
    }
}

static void executePositionMotion(Parser* parser, Editor* editor) {
    Position position = getMotionPosition(editor, parser->cmd.motion, parser->cmd.count);

    if (!editor->successful_motion) {
        editor->save_curosr_col = false;
        return;
    }

    // TODO: handle the motion based on the operator
    (void)position;

    switch (parser->cmd.operator) {
        case OP_NONE:
            break;
        case OP_DELETE:
            break;
        case OP_CHANGE:
            break;
        case OP_YANK:
            break;
        default:
            ERROR("Wrong operator");
    }
}

static void executeNormalMode(Parser* parser, Editor* editor) {
    fixCount(parser);

    editor->successful_motion = true;

    if (parser->cmd.operator == OP_REPLACE) {
        replaceChar(&editor->buffer, &editor->cursor, parser->cmd.count, parser->cmd.argument);
        return;
    }

    if (parser->cmd.operator == OP_NONE) {
        if (isMovementToExecute(editor, parser->cmd.motion, parser->cmd.count)) {
            return;
        }
    }

    switch (getMotionType(parser->cmd.motion)) {
        case MOT_TYPE_ROW:
            executeRowMotion(parser, editor);
            break;
        case MOT_TYPE_COL_LEFT:
            executeColLeftMotion(parser, editor);
            break;
        case MOT_TYPE_COL_RIGHT:
            executeColRightMotion(parser, editor);
            break;
        case MOT_TYPE_POSITION:
            executePositionMotion(parser, editor);
            break;
        default:
            ERROR("Wrong motion type");
    }
}

static void executeAndInit(Parser* parser, Editor* editor) {
    executeNormalMode(parser, editor);
    parserInit(parser);
}

static void wrongInput(Parser* parser, Editor* editor) {
    editor->save_curosr_col = false;
    parserInit(parser);
}

void parseNormalMode(Parser* parser, Editor* editor, int key) {
    // TODO: remove exiting with 'q' after handling commmand mode
    if (key == 'q' && parser->state == STATE_NORMAL) {
        editor->mode = EXIT;
        return;
    }

    if (parser->cmd.operator == OP_REPLACE) {
        if (!isPrintable(key)) {
            wrongInput(parser, editor);
            return;
        }

        parser->cmd.argument = key;
        executeAndInit(parser, editor);
        return;
    }

    if (key >= '0' && key <= '9') {
        if (parser->state == STATE_NORMAL) {
            if (!(key == '0' && parser->cmd.count == 0)) {
                if (parser->cmd.motion != MOT_NONE) {
                    wrongInput(parser, editor);
                    return;
                }

                parser->cmd.count = parser->cmd.count * 10 + (key - '0');
                editor->save_curosr_col = false;
                return;
            }
        } else {
            if (!(key == '0' && parser->cmd.count_after_operator == 0)) {
                if (parser->cmd.motion != MOT_NONE) {
                    wrongInput(parser, editor);
                    return;
                }

                parser->cmd.count_after_operator = parser->cmd.count_after_operator * 10 + (key - '0');
                editor->save_curosr_col = false;
                return;
            }
        }
    }

    parser->cmd.key_cache.chars[parser->cmd.key_cache.length++] = key;
    parser->cmd.key_cache.chars[parser->cmd.key_cache.length] = '\0';

    if (parser->cmd.motion != MOT_NONE) {
        switch (key) {
            case 'g':
                if (parser->cmd.motion != MOT_FIRST_LINE) {
                    wrongInput(parser, editor);
                    return;
                }

                executeAndInit(parser, editor);
                return;

            default:
                if (!isPrintable(key)) {
                    wrongInput(parser, editor);
                    return;
                }

                parser->cmd.argument = key;
                executeAndInit(parser, editor);
                return;
        }
    }

    bool ready_to_execute = true;
    switch (key) {
        case 'h':
            parser->cmd.motion = MOT_LEFT;
            break;
        case 'j':
            parser->cmd.motion = MOT_DOWN;
            break;
        case 'k':
            parser->cmd.motion = MOT_UP;
            break;
        case 'l':
            parser->cmd.motion = MOT_RIGHT;
            break;
        case 'w':
            parser->cmd.motion = MOT_WORD;
            break;
        case 'W':
            parser->cmd.motion = MOT_BIG_WORD;
            break;
        case 'e':
            parser->cmd.motion = MOT_END_WORD;
            break;
        case 'E':
            parser->cmd.motion = MOT_END_BIG_WORD;
            break;
        case 'b':
            parser->cmd.motion = MOT_BACKWARD_WORD;
            break;
        case 'B':
            parser->cmd.motion = MOT_BACKWARD_BIG_WORD;
            break;
        case '0':
            parser->cmd.motion = MOT_START_OF_LINE;
            break;
        case '^':
            parser->cmd.motion = MOT_FIRST_NON_BLANK_CHAR_OF_LINE;
            break;
        case '$':
            parser->cmd.motion = MOT_END_OF_LINE;
            break;
        case 'G':
            parser->cmd.motion = MOT_LAST_LINE;
            break;
        case '}':
            parser->cmd.motion = MOT_NEXT_PARAGRAPH;
            break;
        case '{':
            parser->cmd.motion = MOT_PREVIOUS_PARAGRAPH;
            break;
        case '%':
            parser->cmd.motion = MOT_MATCHING_CHAR;
            break;

        case 'g':
            parser->cmd.motion = MOT_FIRST_LINE;
            return;
        case 'f':
            parser->cmd.motion = MOT_NEXT_OCCURRENCE_OF_CHAR;
            return;
        case 't':
            parser->cmd.motion = MOT_BEFORE_NEXT_OCCURRENCE_OF_CHAR;
            return;
        case 'F':
            parser->cmd.motion = MOT_PREVIOUS_OCCURRENCE_OF_CHAR;
            return;
        case 'T':
            parser->cmd.motion = MOT_AFTER_PREVIOUS_OCCURRENCE_OF_CHAR;
            return;

        default:
            ready_to_execute = false;
            break;
    }

    if (ready_to_execute) {
        executeAndInit(parser, editor);
        return;
    }

    switch (parser->state) {
        case STATE_NORMAL:
            switch (key) {
                case 'd':
                    parser->state = STATE_OPERATOR_PENDING;
                    parser->cmd.operator = OP_DELETE;
                    editor->save_curosr_col = false;
                    return;
                case 'c':
                    parser->state = STATE_OPERATOR_PENDING;
                    parser->cmd.operator = OP_CHANGE;
                    editor->save_curosr_col = false;
                    return;
                case 'y':
                    parser->state = STATE_OPERATOR_PENDING;
                    parser->cmd.operator = OP_YANK;
                    editor->save_curosr_col = false;
                    return;
                case 'r':
                    parser->state = STATE_OPERATOR_PENDING;
                    parser->cmd.operator = OP_REPLACE;
                    editor->save_curosr_col = false;
                    return;
            }
            break;
        case STATE_OPERATOR_PENDING:
            switch (key) {
                case 'd':
                    if (parser->cmd.operator == OP_DELETE) {
                        parser->cmd.motion = MOT_LINE;
                        executeAndInit(parser, editor);
                    } else {
                        wrongInput(parser, editor);
                    }
                    return;
                case 'c':
                    if (parser->cmd.operator == OP_CHANGE) {
                        parser->cmd.motion = MOT_LINE;
                        executeAndInit(parser, editor);
                    } else {
                        wrongInput(parser, editor);
                    }
                    return;
                case 'y':
                    if (parser->cmd.operator == OP_YANK) {
                        parser->cmd.motion = MOT_LINE;
                        executeAndInit(parser, editor);
                    } else {
                        wrongInput(parser, editor);
                    }
                    return;
                default:
                    wrongInput(parser, editor);
                    return;
            }
            break;
    }

    switch (key) {
        case 'i':
            editor->mode = INSERT;
            parserInit(parser);
            return;
        case 'a':
            editor->mode = INSERT;
            insertArrowRight(editor);
            parserInit(parser);
            return;
        case 'I':
            editor->mode = INSERT;
            editor->cursor.col = getMotionCol(editor, MOT_FIRST_NON_BLANK_CHAR_OF_LINE);
            parserInit(parser);
            return;
        case 'A':
            editor->mode = INSERT;
            editor->cursor.col = getMotionCol(editor, MOT_END_OF_LINE);
            insertArrowRight(editor);
            parserInit(parser);
            return;
        case 'o':
            editor->mode = INSERT;
            appendLine(&editor->buffer, &editor->cursor);
            parserInit(parser);
            return;
        case 'O':
            editor->mode = INSERT;
            prependLine(&editor->buffer, &editor->cursor);
            parserInit(parser);
            return;
        case 'x':
            parser->cmd.operator = OP_DELETE;
            parser->cmd.motion = MOT_RIGHT;
            executeAndInit(parser, editor);
            return;
        case 'X':
            parser->cmd.operator = OP_DELETE;
            parser->cmd.motion = MOT_LEFT;
            executeAndInit(parser, editor);
            return;
        case 'J':
            joinLines(&editor->buffer, &editor->cursor, getMotionRow(editor, MOT_DOWN, parser->cmd.count ? parser->cmd.count : 1));
            parserInit(parser);
            return;
    }

    wrongInput(parser, editor);
}
