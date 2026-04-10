#include "normal.h"

#include "movement.h"

static void handleOperatorNone(Parser* parser, Editor* editor) {
    switch (parser->cmd.motion) {
        case MOT_LEFT:
            movementLeft(editor, parser->cmd.count);
            break;
        case MOT_UP:
            movementUp(editor, parser->cmd.count);
            break;
        case MOT_DOWN:
            movementDown(editor, parser->cmd.count);
            break;
        case MOT_RIGHT:
            movementRight(editor, parser->cmd.count);
            break;
        case MOT_WORD:
            break;
        case MOT_BIG_WORD:
            break;
        case MOT_END_WORD:
            break;
        case MOT_END_BIG_WORD:
            break;
        case MOT_BACKWARD_WORD:
            break;
        case MOT_BACKWARD_BIG_WORD:
            break;
        case MOT_START_OF_LINE:
            break;
        case MOT_FIRST_NON_BLANK_CHAR_OF_LINE:
            break;
        case MOT_END_OF_LINE:
            break;
        case MOT_LAST_LINE:
            break;
        case MOT_NEXT_PARAGRAPH:
            break;
        case MOT_PREVIOUS_PARAGRAPH:
            break;
        case MOT_FIRST_LINE:
            break;
        case MOT_NEXT_OCCURRENCE_OF_CHAR:
            break;
        case MOT_BEFORE_NEXT_OCCURRENCE_OF_CHAR:
            break;
        case MOT_PREVIOUS_OCCURRENCE_OF_CHAR:
            break;
        case MOT_AFTER_PREVIOUS_OCCURRENCE_OF_CHAR:
            break;
        default:
            break;
    }
}

static void handleOperatorDelete(Parser* parser, Editor* editor) {
    switch (parser->cmd.motion) {
        case MOT_LEFT:
            deleteCharLeft(&editor->buffer, &editor->cursor, parser->cmd.count);
            break;
        case MOT_UP:
            break;
        case MOT_DOWN:
            break;
        case MOT_RIGHT:
            deleteCharRight(&editor->buffer, &editor->cursor, parser->cmd.count);
            break;
        case MOT_LINE:
            deleteLine(&editor->buffer, &editor->cursor, parser->cmd.count);
            break;
        case MOT_WORD:
            break;
        case MOT_BIG_WORD:
            break;
        case MOT_END_WORD:
            break;
        case MOT_END_BIG_WORD:
            break;
        case MOT_BACKWARD_WORD:
            break;
        case MOT_BACKWARD_BIG_WORD:
            break;
        case MOT_START_OF_LINE:
            break;
        case MOT_FIRST_NON_BLANK_CHAR_OF_LINE:
            break;
        case MOT_END_OF_LINE:
            break;
        case MOT_FIRST_LINE:
            break;
        case MOT_LAST_LINE:
            break;
        case MOT_NEXT_PARAGRAPH:
            break;
        case MOT_PREVIOUS_PARAGRAPH:
            break;
        case MOT_NEXT_OCCURRENCE_OF_CHAR:
            break;
        case MOT_BEFORE_NEXT_OCCURRENCE_OF_CHAR:
            break;
        case MOT_PREVIOUS_OCCURRENCE_OF_CHAR:
            break;
        case MOT_AFTER_PREVIOUS_OCCURRENCE_OF_CHAR:
            break;
        default:
            break;
    }
}

static void handleOperatorChange(Parser* parser, Editor* editor) {
    handleOperatorDelete(parser, editor);
    editor->mode = INSERT;
}

static void handleOperatorYank(Parser* parser, Editor* editor) {
    switch (parser->cmd.motion) {
        case MOT_LINE:
            (void)editor;
            break;
        default:
            break;
    }
}

static void handleOperatorReplace(Parser* parser, Editor* editor) {
    replaceChar(&editor->buffer, &editor->cursor, parser->cmd.count, parser->cmd.argument);
}

static void fixCount(Parser* parser) {
    if (parser->cmd.count == 0) {
        parser->cmd.count = 1;
    }

    if (parser->cmd.count_after_operator == 0) {
        parser->cmd.count_after_operator = 1;
    }

    parser->cmd.count = parser->cmd.count * parser->cmd.count_after_operator;
}

static void executeNormalMode(Parser* parser, Editor* editor) {
    fixCount(parser);

    editor->successfu_motion = true;

    if (parser->cmd.operator == OP_NONE) {
        switch (parser->cmd.motion) {
            case MOT_LEFT:
                movementLeft(editor, parser->cmd.count);
                return;
            case MOT_UP:
                movementUp(editor, parser->cmd.count);
                return;
            case MOT_DOWN:
                movementDown(editor, parser->cmd.count);
                return;
            case MOT_RIGHT:
                movementRight(editor, parser->cmd.count);
                return;
            case MOT_FIRST_LINE:
                movementFirstLine(editor, parser->cmd.count);
                return;
            case MOT_LAST_LINE:
                movementLastLine(editor, parser->cmd.count);
                return;
            case MOT_END_OF_LINE:
                movementEndOfLine(editor);
                return;
            default:
                break;
        }
    }

    // TODO: replace the buffer functions with deleteRow and deleteCol
    //       find the size_t row or col and execute based on the MotionType
    //       make the change operator to be the delete + insert

    switch (parser->cmd.operator) {
        case OP_NONE:
            handleOperatorNone(parser, editor);
            break;
        case OP_DELETE:
            handleOperatorDelete(parser, editor);
            break;
        case OP_CHANGE:
            handleOperatorChange(parser, editor);
            break;
        case OP_YANK:
            handleOperatorYank(parser, editor);
            break;
        case OP_REPLACE:
            handleOperatorReplace(parser, editor);
            break;
    }
}

static void executeAndInit(Parser* parser, Editor* editor) {
    executeNormalMode(parser, editor);
    parserInit(parser);
}

void parseNormalMode(Parser* parser, Editor* editor, int key) {
    // TODO: remove exiting with 'q' after handling commmand mode
    if (key == 'q' && parser->state == STATE_NORMAL) {
        editor->mode = EXIT;
        return;
    }

    if (parser->cmd.operator == OP_REPLACE) {
        parser->cmd.argument = key;
        executeAndInit(parser, editor);
        return;
    }

    if (key >= '0' && key <= '9') {
        if (parser->state == STATE_NORMAL) {
            if (!(key == '0' && parser->cmd.count == 0)) {
                if (parser->cmd.motion == MOT_NONE) {
                    parser->cmd.count = parser->cmd.count * 10 + (key - '0');
                } else {
                    parserInit(parser);
                }
                return;
            }
        } else {
            if (!(key == '0' && parser->cmd.count_after_operator == 0)) {
                if (parser->cmd.motion == MOT_NONE) {
                    parser->cmd.count_after_operator = parser->cmd.count_after_operator * 10 + (key - '0');
                } else {
                    parserInit(parser);
                }
                return;
            }
        }
    }

    parser->cmd.key_cache.chars[parser->cmd.key_cache.length++] = key;
    parser->cmd.key_cache.chars[parser->cmd.key_cache.length] = '\0';

    if (parser->cmd.motion != MOT_NONE) {
        switch (key) {
            case 'g':
                if (parser->cmd.motion == MOT_FIRST_LINE_PENDING) {
                    parser->cmd.motion = MOT_FIRST_LINE;
                    executeAndInit(parser, editor);
                } else {
                    parserInit(parser);
                }
                return;
            case 'f':
                if (parser->cmd.motion == MOT_NEXT_OCCURRENCE_OF_CHAR_PENDING) {
                    parser->cmd.motion = MOT_NEXT_OCCURRENCE_OF_CHAR;
                    executeAndInit(parser, editor);
                } else {
                    parserInit(parser);
                }
                return;
            case 't':
                if (parser->cmd.motion == MOT_BEFORE_NEXT_OCCURRENCE_OF_CHAR_PENDING) {
                    parser->cmd.motion = MOT_BEFORE_NEXT_OCCURRENCE_OF_CHAR;
                    executeAndInit(parser, editor);
                } else {
                    parserInit(parser);
                }
                return;
            case 'F':
                if (parser->cmd.motion == MOT_PREVIOUS_OCCURRENCE_OF_CHAR_PENDING) {
                    parser->cmd.motion = MOT_PREVIOUS_OCCURRENCE_OF_CHAR;
                    executeAndInit(parser, editor);
                } else {
                    parserInit(parser);
                }
                return;
            case 'T':
                if (parser->cmd.motion == MOT_AFTER_PREVIOUS_OCCURRENCE_OF_CHAR_PENDING) {
                    parser->cmd.motion = MOT_AFTER_PREVIOUS_OCCURRENCE_OF_CHAR;
                    executeAndInit(parser, editor);
                } else {
                    parserInit(parser);
                }
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

        case 'g':
            parser->cmd.motion = MOT_FIRST_LINE_PENDING;
            return;
        case 'f':
            parser->cmd.motion = MOT_NEXT_OCCURRENCE_OF_CHAR_PENDING;
            return;
        case 't':
            parser->cmd.motion = MOT_BEFORE_NEXT_OCCURRENCE_OF_CHAR_PENDING;
            return;
        case 'F':
            parser->cmd.motion = MOT_PREVIOUS_OCCURRENCE_OF_CHAR_PENDING;
            return;
        case 'T':
            parser->cmd.motion = MOT_AFTER_PREVIOUS_OCCURRENCE_OF_CHAR_PENDING;
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
                    return;
                case 'c':
                    parser->state = STATE_OPERATOR_PENDING;
                    parser->cmd.operator = OP_CHANGE;
                    return;
                case 'y':
                    parser->state = STATE_OPERATOR_PENDING;
                    parser->cmd.operator = OP_YANK;
                    return;
                case 'r':
                    parser->state = STATE_OPERATOR_PENDING;
                    parser->cmd.operator = OP_REPLACE;
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
                        parserInit(parser);
                    }
                    return;
                case 'c':
                    if (parser->cmd.operator == OP_CHANGE) {
                        parser->cmd.motion = MOT_LINE;
                        executeAndInit(parser, editor);
                    } else {
                        parserInit(parser);
                    }
                    return;
                case 'y':
                    if (parser->cmd.operator == OP_YANK) {
                        parser->cmd.motion = MOT_LINE;
                        executeAndInit(parser, editor);
                    } else {
                        parserInit(parser);
                    }
                    return;
                default:
                    parserInit(parser);
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
            editor->cursor.col = jumpToFirstNonBlankCharOfLine(editor);
            parserInit(parser);
            return;
        case 'A':
            editor->mode = INSERT;
            editor->cursor.col = jumpToEndOfLine(editor);
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
    }

    parserInit(parser);
}
