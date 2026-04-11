#include "visual_line.h"

static void fixCount(Parser* parser) {
    if (parser->cmd.count == 0) {
        parser->cmd.count = 1;
    }
}

static void executeVisualLineMode(Parser* parser, Editor* editor) {
    fixCount(parser);

    editor->successful_motion = true;

    // TODO: implement visual line movements and execute them or update the visual selection
    (void)editor;
}

static void executeAndInit(Parser* parser, Editor* editor) {
    executeVisualLineMode(parser, editor);
    parserInit(parser);
}

void parseVisualLineMode(Parser* parser, Editor* editor, int key) {
    // TODO: implement visual line mode parser
    (void)parser;
    (void)editor;
    (void)key;
    executeAndInit(parser, editor);
}
