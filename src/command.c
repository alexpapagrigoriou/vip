#include "command.h"

static void handleOperatorNone(Parser* parser, Editor* editor) {
    switch (parser->cmd.motion) {
        case MOT_LEFT:
            break;
        case MOT_DOWN:
            break;
        case MOT_UP:
            break;
        case MOT_RIGHT:
            break;
        case MOT_LINE:
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
}

static void handleOperatorChange(Parser* parser, Editor* editor) {
}

static void handleOperatorYank(Parser* parser, Editor* editor) {
}

void executeCommand(Parser* parser, Editor* editor) {
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
    }
}
