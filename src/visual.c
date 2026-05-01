#include "visual.h"

static void fix_count(Parser* parser) {
    if (parser->cmd.count == 0) {
        parser->cmd.count = 1;
    }
}

static void execute_visual_mode(Parser* parser, Editor* editor) {
    fix_count(parser);

    editor->successful_motion = true;

    // TODO: implement visual movements and execute them or update the visual selection
    (void)editor;
}

static void execute_and_init(Parser* parser, Editor* editor) {
    execute_visual_mode(parser, editor);
    parser_init(parser);
}

void parse_visual_mode(Parser* parser, Editor* editor, int key) {
    // TODO: implement visual mode parser
    (void)parser;
    (void)editor;
    (void)key;
    execute_and_init(parser, editor);
}
