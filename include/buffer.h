#pragma once

#include <stddef.h>

#include "line.h"
#include "terminal.h"

typedef struct {
    Line* lines;
    size_t line_count;
} Buffer;

void bufferInit(Buffer* buffer);
void freeBufferLines(Buffer* buffer);
void insertChar(Buffer* buffer, Position* cursor, const char c);
void insertString(Buffer* buffer, Position* cursor, const char* text);
void clearLine(Buffer* buffer, Position* cursor);
void deleteCharLeft(Buffer* buffer, Position* cursor, const size_t count);
void deleteCharRight(Buffer* buffer, Position* cursor, const size_t count);
void replaceChar(Buffer* buffer, Position* cursor, const size_t count, const char c);
void appendLine(Buffer* buffer, Position* cursor);
void prependLine(Buffer* buffer, Position* cursor);
void deleteLine(Buffer* buffer, Position* cursor, const size_t count);
