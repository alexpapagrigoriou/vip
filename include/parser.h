#pragma once

#include "editor.h"

typedef enum {
    OP_NONE,
    OP_DELETE,
    OP_CHANGE,
    OP_YANK
} Operator;

typedef enum {
    MOT_NONE,
    MOT_LEFT,
    MOT_DOWN,
    MOT_UP,
    MOT_RIGHT,
    MOT_LINE,
    MOT_WORD,
    MOT_BIG_WORD,
    MOT_END_WORD,
    MOT_END_BIG_WORD,
    MOT_BACKWARD_WORD,
    MOT_BACKWARD_BIG_WORD,
    MOT_START_OF_LINE,
    MOT_FIRST_NON_BLANK_CHAR_OF_LINE,
    MOT_END_OF_LINE,
    MOT_LAST_LINE,
    MOT_NEXT_PARAGRAPH,
    MOT_PREVIOUS_PARAGRAPH,

    MOT_FIRST_LINE_PENDING,
    MOT_NEXT_OCCURRENCE_OF_CHAR_PENDING,
    MOT_BEFORE_NEXT_OCCURRENCE_OF_CHAR_PENDING,
    MOT_PREVIOUS_OCCURRENCE_OF_CHAR_PENDING,
    MOT_AFTER_PREVIOUS_OCCURRENCE_OF_CHAR_PENDING,

    MOT_FIRST_LINE,
    MOT_NEXT_OCCURRENCE_OF_CHAR,
    MOT_BEFORE_NEXT_OCCURRENCE_OF_CHAR,
    MOT_PREVIOUS_OCCURRENCE_OF_CHAR,
    MOT_AFTER_PREVIOUS_OCCURRENCE_OF_CHAR
} Motion;

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
