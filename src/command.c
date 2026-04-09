#include "command.h"

#include "buffer.h"

static void handleOperatorNone(Parser* parser, Editor* editor) {
    // TODO: add count in commands
    switch (parser->cmd.motion) {
        case MOT_LEFT:
            moveLeft(editor);
            break;
        case MOT_DOWN:
            moveDown(editor);
            break;
        case MOT_UP:
            moveUp(editor);
            break;
        case MOT_RIGHT:
            moveRight(editor);
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
    // TODO: add count in commands
    switch (parser->cmd.motion) {
        case MOT_LEFT:
            if (editor->cursor.col != 0) {
                moveLeft(editor);
                deleteChar(&editor->buffer, &editor->cursor);
            }
            break;
        case MOT_DOWN:
            break;
        case MOT_UP:
            break;
        case MOT_RIGHT:
            deleteChar(&editor->buffer, &editor->cursor);
            break;
        case MOT_WORD:
            break;
        case MOT_LINE:
            deleteLine(&editor->buffer, &editor->cursor, parser->cmd.count);
            break;
        default:
            break;
    }
}

static void handleOperatorChange(Parser* parser, Editor* editor) {
    // TODO: add count in commands
    switch (parser->cmd.motion) {
        case MOT_LINE:
            deleteLine(&editor->buffer, &editor->cursor, parser->cmd.count);
            editor->mode = INSERT;
            break;
        default:
            break;
    }
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

void executeCommand(Parser* parser, Editor* editor) {
    if (parser->cmd.count == 0) {
        parser->cmd.count = 1;
    }

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
