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
void freeBufferLines(Buffer* buffer);

// TODO: remove these functions after merging them into deleteRow, deleteColLeft and deleteColRight functions
void insertChar(Buffer* buffer, Position* cursor, const char c);
void insertString(Buffer* buffer, Position* cursor, const char* text);
void clearLine(Buffer* buffer, Position* cursor);
void deleteCharLeft(Buffer* buffer, Position* cursor, const size_t count);
void deleteCharRight(Buffer* buffer, Position* cursor, const size_t count);
void deleteStartOfLine(Buffer* buffer, Position* cursor);
void deleteEndOfLine(Buffer* buffer, Position* cursor);
void replaceChar(Buffer* buffer, Position* cursor, const size_t count, const char c);
void appendLine(Buffer* buffer, Position* cursor);
void prependLine(Buffer* buffer, Position* cursor);
void deleteLine(Buffer* buffer, Position* cursor, const size_t count);

void deleteRow(Buffer* buffer, Position* cursor, const size_t row);
void deleteColLeft(Buffer* buffer, Position* cursor, const size_t col);
void deleteColRight(Buffer* buffer, Position* cursor, const size_t col);
void joinLines(Buffer* buffer, Position* cursor, const size_t row);
