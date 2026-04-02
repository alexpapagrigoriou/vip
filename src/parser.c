#include "parser.h"

#include <stdbool.h>

#include "command.h"
#include "input.h"
#include "keys.h"
#include "motion.h"

static void executeAndInit(Parser* parser, Editor* editor) {
    executeCommand(parser, editor);
    parserInit(parser);
}

void moveCursorToStartOfRange(Editor* editor, Range range) {
    editor->cursor = range.start;
}

void moveCursorToEndOfRange(Editor* editor, Range range) {
    editor->cursor = range.end;
}

void parserInit(Parser* parser) {
    parser->state = STATE_NORMAL;
    parser->cmd.count = 0;
    parser->cmd.operator = OP_NONE;
    parser->cmd.motion = MOT_NONE;
    parser->cmd.argument = '\0';
    parser->cmd.keyCacheLength = 0;
}

static void handleNormalMode(Parser* parser, Editor* editor, int key) {
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
        if (!(key == '0' && parser->cmd.count == 0)) {
            if (parser->cmd.operator == OP_NONE && parser->cmd.motion == MOT_NONE) {
                parser->cmd.count = parser->cmd.count * 10 + (key - '0');
            } else {
                parserInit(parser);
            }
            return;
        }
    }

    parser->cmd.keyCache[parser->cmd.keyCacheLength++] = key;

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
            return;
        case 'a':
            moveRight(editor);
            editor->mode = INSERT;
            return;
        case 'I':
            moveCursorToStartOfRange(editor, jumpToFirstNonBlankCharOfLine(editor));
            editor->mode = INSERT;
            return;
        case 'A':
            moveCursorToEndOfRange(editor, jumpToEndOfLine(editor));
            editor->mode = INSERT;
            return;
        case 'o':
            appendLine(editor->buffer, &editor->cursor);
            editor->mode = INSERT;
            return;
        case 'O':
            prependLine(editor->buffer, &editor->cursor);
            editor->mode = INSERT;
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

static void handleInsertMode(Editor* editor, int key) {
    switch (key) {
        case ESC:
        case CTRL_F:
            moveLeft(editor);
            editor->mode = NORMAL;
            break;
        case TAB:
            insertString(editor->buffer, &editor->cursor, "    ");
            break;
        case ENTER:
            appendLine(editor->buffer, &editor->cursor);
            break;
        case UP:
            moveUp(editor);
            break;
        case DOWN:
            moveDown(editor);
            break;
        case RIGHT:
            moveRight(editor);
            break;
        case LEFT:
            moveLeft(editor);
            break;
        default:
            if (isPrintable(key)) {
                insertChar(editor->buffer, &editor->cursor, key);
            }
            break;
    }
}

static void handleVisualMode(Editor* editor, int key) {
    (void)editor;
    (void)key;
}

static void handleCommandMode(Editor* editor, int key) {
    (void)editor;
    (void)key;
}

void handleKey(Parser* parser, Editor* editor, int key) {
    switch (editor->mode) {
        case NORMAL:
            handleNormalMode(parser, editor, key);
            break;
        case INSERT:
            handleInsertMode(editor, key);
            break;
        case VISUAL:
            handleVisualMode(editor, key);
            break;
        case COMMAND:
            handleCommandMode(editor, key);
            break;
        case EXIT:
            break;
    }
}
