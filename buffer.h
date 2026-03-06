#pragma once

#include "terminal.h"

typedef struct {
    char* chars;
    int length;
} Line;

typedef struct {
    Line* lines;
    int line_count;
} Buffer;

extern Position cursor;
extern Position text;

void initBuffer(void);
Line* getLine(int row);
int getLineCount(void);
void insertChar(char c);
void deleteChar(void);
void appendLine(void);
void prependLine(void);
void deleteLine(void);
void freeBuffer(void);
