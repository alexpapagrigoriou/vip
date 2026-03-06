#include "buffer.h"

#include <stdlib.h>
#include <string.h>

Buffer* buffer;

Position cursor;
Position text;

static void initLine(Line* line) {
    line->chars = malloc(1);
    line->chars[0] = '\0';

    line->length = 0;
}

void initBuffer(void) {
    buffer = malloc(sizeof(Buffer));

    buffer->lines = malloc(sizeof(Line));
    initLine(&buffer->lines[0]);

    buffer->line_count = 1;

    cursor = (Position){0, 0};
    text = (Position){0, 0};
}

Line* getLine(int row) {
    return &buffer->lines[row];
}

int getLineCount(void) {
    return buffer->line_count;
}

void insertChar(char c) {
    Line* line = &buffer->lines[cursor.row];

    line->chars = realloc(line->chars, line->length + 2);

    memmove(&line->chars[cursor.col + 1], &line->chars[cursor.col], line->length - cursor.col + 1);

    line->chars[cursor.col] = c;
    line->length++;

    cursor.col++;
}

void deleteChar(void) {
    Line* line = &buffer->lines[cursor.row];

    if (line->length == 0) {
        return;
    }

    line->chars = realloc(line->chars, line->length);

    memmove(&line->chars[cursor.col], &line->chars[cursor.col + 1], line->length - cursor.col - 1);

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
    free(buffer->lines[cursor.row].chars);

    memmove(&buffer->lines[cursor.row], &buffer->lines[cursor.row + 1], sizeof(Line) * (buffer->line_count - cursor.row - 1));

    buffer->lines = realloc(buffer->lines, sizeof(Line) * (buffer->line_count - 1));
    buffer->line_count--;

    cursor.col = 0;
}

void freeBuffer(void) {
    for (int i = 0; i < buffer->line_count; i++) {
        free(buffer->lines[i].chars);
    }
    free(buffer->lines);
}
