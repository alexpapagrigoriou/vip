#include "buffer.h"

#include <stdlib.h>
#include <string.h>

void bufferInit(Buffer* buffer) {
    buffer->lines = malloc(sizeof(Line));
    lineInit(&buffer->lines[0]);

    buffer->line_count = 1;
}

void freeBufferLines(Buffer* buffer) {
    for (size_t i = 0; i < buffer->line_count; i++) {
        freeLine(&buffer->lines[i]);
    }
    free(buffer->lines);
}

void insertChar(Buffer* buffer, Position* cursor, const char c) {
    Line* line = &buffer->lines[cursor->row];

    line->chars = realloc(line->chars, line->length + 2);

    memmove(&line->chars[cursor->col + 1], &line->chars[cursor->col], line->length - cursor->col + 1);

    line->chars[cursor->col] = c;
    line->length++;

    cursor->col++;
}

void insertString(Buffer* buffer, Position* cursor, const char* text) {
    size_t text_length = strlen(text);

    Line* line = &buffer->lines[cursor->row];

    line->chars = realloc(line->chars, line->length + text_length + 1);

    memmove(&line->chars[cursor->col + text_length], &line->chars[cursor->col], line->length - cursor->col + 1);

    memmove(&line->chars[cursor->col], text, text_length);
    line->length += text_length;

    cursor->col += text_length;
}

void deleteChar(Buffer* buffer, Position* cursor) {
    Line* line = &buffer->lines[cursor->row];

    if (cursor->col == line->length) {
        return;
    }

    memmove(&line->chars[cursor->col], &line->chars[cursor->col + 1], line->length - cursor->col);

    line->chars = realloc(line->chars, line->length);

    line->length--;

    if (cursor->col == line->length && cursor->col > 0) {
        cursor->col--;
    }
}

void replaceChar(Buffer* buffer, Position* cursor, const int count, const char c) {
    Line* line = &buffer->lines[cursor->row];

    if (cursor->col == line->length) {
        return;
    }

    for (int i = 0; i < count; i++) {
        line->chars[cursor->col] = c;
        cursor->col++;

        if (cursor->col == line->length) {
            break;
        }
    }

    cursor->col--;
}

void appendLine(Buffer* buffer, Position* cursor) {
    buffer->lines = realloc(buffer->lines, sizeof(Line) * (buffer->line_count + 1));

    memmove(&buffer->lines[cursor->row + 2], &buffer->lines[cursor->row + 1], sizeof(Line) * (buffer->line_count - cursor->row - 1));

    lineInit(&buffer->lines[cursor->row + 1]);
    buffer->line_count++;

    cursor->row++;
    cursor->col = 0;
}

void prependLine(Buffer* buffer, Position* cursor) {
    buffer->lines = realloc(buffer->lines, sizeof(Line) * (buffer->line_count + 1));

    memmove(&buffer->lines[cursor->row + 1], &buffer->lines[cursor->row], sizeof(Line) * (buffer->line_count - cursor->row));

    lineInit(&buffer->lines[cursor->row]);
    buffer->line_count++;

    cursor->col = 0;
}

void clearLine(Buffer* buffer, Position* cursor) {
    buffer->lines[cursor->row].chars[0] = '\0';
    buffer->lines[cursor->row].length = 0;
}

void deleteLine(Buffer* buffer, Position* cursor, const int count) {
    cursor->col = 0;

    for (int i = 0; i < count; i++) {
        if (buffer->line_count == 1) {
            clearLine(buffer, cursor);
            return;
        }

        free(buffer->lines[cursor->row].chars);

        memmove(&buffer->lines[cursor->row], &buffer->lines[cursor->row + 1], sizeof(Line) * (buffer->line_count - cursor->row - 1));

        buffer->lines = realloc(buffer->lines, sizeof(Line) * (buffer->line_count - 1));
        buffer->line_count--;

        if (cursor->row == buffer->line_count) {
            cursor->row--;
        }
    }
}
