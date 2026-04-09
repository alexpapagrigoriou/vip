#pragma once

#include "editor.h"
#include "motion.h"
#include "operator.h"

#define KEY_CACHE_SIZE 3

typedef struct {
    char chars[KEY_CACHE_SIZE + 1];
    int length;
} KeyCache;

typedef struct {
    int count;
    Operator operator;
    Motion motion;
    char argument;
    KeyCache key_cache;
} Command;

typedef enum {
    STATE_NORMAL,
    STATE_OPERATOR_PENDING
} ParserState;

typedef struct {
    ParserState state;
    Command cmd;
} Parser;

void handleKey(Parser* parser, Editor* editor, int key);
