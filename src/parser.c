#include "parser.h"

#include <stdbool.h>

#include "command.h"
#include "editor.h"
#include "insert.h"
#include "normal.h"
#include "visual.h"
#include "visual_line.h"

void parserInit(Parser* parser) {
    parser->state = STATE_NORMAL;
    parser->cmd.count = 0;
    parser->cmd.count_after_operator = 0;
    parser->cmd.operator = OP_NONE;
    parser->cmd.motion = MOT_NONE;
    parser->cmd.argument = '\0';
    parser->cmd.key_cache.chars[0] = '\0';
    parser->cmd.key_cache.length = 0;
}

void handleKey(Parser* parser, Editor* editor, int key) {
    switch (editor->mode) {
        case NORMAL:
            parseNormalMode(parser, editor, key);
            break;
        case INSERT:
            parseInsertMode(editor, key);
            break;
        case VISUAL:
            parseVisualMode(parser, editor, key);
            break;
        case VISUAL_LINE:
            parseVisualLineMode(parser, editor, key);
            break;
        case COMMAND:
            parseCommandMode(editor, key);
            break;
        case EXIT:
            break;
    }
}
