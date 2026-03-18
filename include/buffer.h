#pragma once

#include <stddef.h>

#include "line.h"
#include "terminal.h"

typedef struct {
    Line* lines;
    size_t line_count;
} Buffer;

Buffer* createBuffer(void);
void freeBuffer(Buffer* buffer);
void insertChar(Buffer* buffer, Position* cursor, const char c);
void insertString(Buffer* buffer, Position* cursor, const char* text);
void clearLine(Buffer* buffer, Position* cursor);
void deleteChar(Buffer* buffer, Position* cursor);
void replaceChar(Buffer* buffer, Position* cursor, const int count, const char c);
void appendLine(Buffer* buffer, Position* cursor);
void prependLine(Buffer* buffer, Position* cursor);
void deleteLine(Buffer* buffer, Position* cursor, const int count);
