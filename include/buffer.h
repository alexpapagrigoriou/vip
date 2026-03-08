#pragma once

#include <stddef.h>

#include "line.h"
#include "terminal.h"

extern Position cursor;
extern Position text;

void initBuffer(void);
Line* getLine(size_t row);
size_t getLineCount(void);
void insertChar(const char c);
void insertString(const char* text);
void deleteChar(void);
void appendLine(void);
void prependLine(void);
void deleteLine(void);
void freeBuffer(void);
