#include "parser.h"

#include <stdbool.h>

#include "command.h"

void parserInit(Parser* parser) {
    parser->state = STATE_NORMAL;
    parser->cmd.count = 0;
    parser->cmd.operator = OP_NONE;
    parser->cmd.motion = MOT_NONE;
    parser->cmd.search = '\0';
    parser->cmd.keyCacheLength = 0;
}

static void handleNormalMode(Parser* parser, Editor* editor, int key) {
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

    bool ready_to_execute = false;
    if (parser->cmd.motion != MOT_NONE) {
        switch (key) {
            case 'g':
                if (parser->cmd.motion == MOT_FIRST_LINE_PENDING) {
                    parser->cmd.motion = MOT_FIRST_LINE;
                    ready_to_execute = true;
                }
                break;
            case 'f':
                if (parser->cmd.motion == MOT_NEXT_OCCURRENCE_OF_CHAR_PENDING) {
                    parser->cmd.motion = MOT_NEXT_OCCURRENCE_OF_CHAR;
                    ready_to_execute = true;
                }
                break;
            case 't':
                if (parser->cmd.motion == MOT_BEFORE_NEXT_OCCURRENCE_OF_CHAR_PENDING) {
                    parser->cmd.motion = MOT_BEFORE_NEXT_OCCURRENCE_OF_CHAR;
                    ready_to_execute = true;
                }
                break;
            case 'F':
                if (parser->cmd.motion == MOT_PREVIOUS_OCCURRENCE_OF_CHAR_PENDING) {
                    parser->cmd.motion = MOT_PREVIOUS_OCCURRENCE_OF_CHAR;
                    ready_to_execute = true;
                }
                break;
            case 'T':
                if (parser->cmd.motion == MOT_AFTER_PREVIOUS_OCCURRENCE_OF_CHAR_PENDING) {
                    parser->cmd.motion = MOT_AFTER_PREVIOUS_OCCURRENCE_OF_CHAR;
                    ready_to_execute = true;
                }
                break;
        }
        if (ready_to_execute) {
            executeCommand(parser, editor);
        }
        parserInit(parser);
        return;
    }

    ready_to_execute = true;
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
        executeCommand(parser, editor);
        parserInit(parser);
        return;
    }

    ready_to_execute = false;
    switch (parser->state) {
        case STATE_NORMAL:
            switch (key) {
                case 'd':
                    parser->state = STATE_OPERATOR_PENDING;
                    parser->cmd.operator = OP_DELETE;
                    break;
                case 'c':
                    parser->state = STATE_OPERATOR_PENDING;
                    parser->cmd.operator = OP_CHANGE;
                    break;
                case 'y':
                    parser->state = STATE_OPERATOR_PENDING;
                    parser->cmd.operator = OP_YANK;
                    break;
            }
            break;
        case STATE_OPERATOR_PENDING:
            switch (key) {
                case 'd':
                    if (parser->cmd.operator == OP_DELETE) {
                        parser->cmd.motion = MOT_LINE;
                        ready_to_execute = true;
                    }
                    break;
                case 'c':
                    if (parser->cmd.operator == OP_CHANGE) {
                        parser->cmd.motion = MOT_LINE;
                        ready_to_execute = true;
                    }
                    break;
                case 'y':
                    if (parser->cmd.operator == OP_YANK) {
                        parser->cmd.motion = MOT_LINE;
                        ready_to_execute = true;
                    }
                    break;
            }
            break;
    }

    if (ready_to_execute) {
        executeCommand(parser, editor);
        parserInit(parser);
        return;
    }
}

void handleKey(Parser* parser, Editor* editor, int key) {
    switch (editor->mode) {
        case NORMAL:
            handleNormalMode(parser, editor, key);
            break;
        case INSERT:
            // handleInsertMode();
            break;
        case VISUAL:
            // handleVisualMode();
            break;
        case COMMAND:
            // handleCommandMode();
            break;
        case EXIT:
            break;
    }
}
