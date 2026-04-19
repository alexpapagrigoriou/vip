#pragma once

#include <stddef.h>

#include "line.h"
#include "terminal.h"

typedef struct {
    Line* lines;
    size_t line_count;
    size_t capacity;
} Buffer;

void bufferInit(Buffer* buffer);
void freeBuffer(Buffer* buffer);

void insertChar(Buffer* buffer, Position* cursor, const char c);
void insertString(Buffer* buffer, Position* cursor, const char* text);

void replaceChar(Buffer* buffer, Position* cursor, const size_t count, const char c);

void appendLine(Buffer* buffer, Position* cursor);
void prependLine(Buffer* buffer, Position* cursor);

void deleteRow(Buffer* buffer, Position* cursor, size_t row);
void deleteColLeft(Buffer* buffer, Position* cursor, size_t col);
void deleteColRight(Buffer* buffer, Position* cursor, size_t col);

void joinLines(Buffer* buffer, Position* cursor, size_t row);
