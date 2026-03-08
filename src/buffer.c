#include "buffer.h"

#include <stdlib.h>
#include <string.h>

typedef struct {
    Line* lines;
    size_t line_count;
} Buffer;

Buffer* buffer;

Position cursor;
Position text;

void initBuffer(void) {
    buffer = malloc(sizeof(Buffer));

    buffer->lines = malloc(sizeof(Line));
    initLine(&buffer->lines[0]);

    buffer->line_count = 1;

    cursor = (Position){0, 0};
    text = (Position){0, 0};
}

Line* getLine(size_t row) {
    return &buffer->lines[row];
}

size_t getLineCount(void) {
    return buffer->line_count;
}

void insertChar(const char c) {
    Line* line = &buffer->lines[cursor.row];

    line->chars = realloc(line->chars, line->length + 2);

    memmove(&line->chars[cursor.col + 1], &line->chars[cursor.col], line->length - cursor.col + 1);

    line->chars[cursor.col] = c;
    line->length++;

    cursor.col++;
}

void insertString(const char* text) {
    size_t text_length = strlen(text);

    Line* line = &buffer->lines[cursor.row];

    line->chars = realloc(line->chars, line->length + text_length + 1);

    memmove(&line->chars[cursor.col + text_length], &line->chars[cursor.col], line->length - cursor.col + 1);

    memmove(&line->chars[cursor.col], text, text_length);
    line->length += text_length;

    cursor.col += text_length;
}

void deleteChar(void) {
    Line* line = &buffer->lines[cursor.row];

    if (cursor.col == line->length) {
        return;
    }

    memmove(&line->chars[cursor.col], &line->chars[cursor.col + 1], line->length - cursor.col);

    line->chars = realloc(line->chars, line->length - 1);

    line->length--;
}

void appendLine(void) {
    buffer->lines = realloc(buffer->lines, sizeof(Line) * (buffer->line_count + 1));

    memmove(&buffer->lines[cursor.row + 2], &buffer->lines[cursor.row + 1], sizeof(Line) * (buffer->line_count - cursor.row - 1));

    initLine(&buffer->lines[cursor.row + 1]);
    buffer->line_count++;

    cursor.row++;
    cursor.col = 0;
}

void prependLine(void) {
    buffer->lines = realloc(buffer->lines, sizeof(Line) * (buffer->line_count + 1));

    memmove(&buffer->lines[cursor.row + 1], &buffer->lines[cursor.row], sizeof(Line) * (buffer->line_count - cursor.row));

    initLine(&buffer->lines[cursor.row]);
    buffer->line_count++;

    cursor.col = 0;
}

void deleteLine(void) {
    cursor.col = 0;

    if (buffer->line_count == 1) {
        buffer->lines[cursor.row].chars[0] = '\0';
        buffer->lines[cursor.row].length = 0;
        return;
    }

    free(buffer->lines[cursor.row].chars);

    memmove(&buffer->lines[cursor.row], &buffer->lines[cursor.row + 1], sizeof(Line) * (buffer->line_count - cursor.row - 1));

    buffer->lines = realloc(buffer->lines, sizeof(Line) * (buffer->line_count - 1));
    buffer->line_count--;

    if (cursor.row == buffer->line_count) {
        cursor.row--;
    }
}

void freeBuffer(void) {
    for (size_t i = 0; i < buffer->line_count; i++) {
        freeLine(&buffer->lines[i]);
    }
    free(buffer->lines);
}
