#pragma once

#include "editor.h"
#include "motion.h"
#include "operator.h"

typedef struct {
    int count;
    Operator operator;
    Motion motion;
    char search;
    char keyCache[3];
    int keyCacheLength;
} Command;

typedef enum {
    STATE_NORMAL,
    STATE_OPERATOR_PENDING
} ParserState;

typedef struct {
    ParserState state;
    Command cmd;
} Parser;

void parserInit(Parser* parser);
void handleKey(Parser* parser, Editor* editor, int key);
