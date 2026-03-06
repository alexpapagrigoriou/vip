#pragma once

#include <stddef.h>

#include "terminal.h"

typedef struct {
    char* chars;
    size_t length;
} Line;

typedef struct {
    Line* lines;
    size_t line_count;
} Buffer;

extern Position cursor;
extern Position text;

void initBuffer(void);
Line* getLine(size_t row);
size_t getLineCount(void);
void insertChar(char c);
void deleteChar(void);
void appendLine(void);
void prependLine(void);
void deleteLine(void);
void freeBuffer(void);
