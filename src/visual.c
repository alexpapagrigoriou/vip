#include "visual.h"

static void fixCount(Parser* parser) {
    if (parser->cmd.count == 0) {
        parser->cmd.count = 1;
    }
}

static void executeVisualMode(Parser* parser, Editor* editor) {
    fixCount(parser);

    editor->successful_motion = true;

    // TODO: implement visual movements and execute them or update the visual selection
    (void)editor;
}

static void executeAndInit(Parser* parser, Editor* editor) {
    executeVisualMode(parser, editor);
    parserInit(parser);
}

void parseVisualMode(Parser* parser, Editor* editor, int key) {
    // TODO: implement visual mode parser
    (void)parser;
    (void)editor;
    (void)key;
    executeAndInit(parser, editor);
}
