#include "buffer.h"

#include <stdlib.h>
#include <string.h>

#include "error.h"

#define BUFFER_INITIAL_CAPACITY 8

void bufferInit(Buffer* buffer) {
    buffer->lines = malloc(sizeof(Line) * BUFFER_INITIAL_CAPACITY);
    lineInit(&buffer->lines[0]);
    buffer->line_count = 1;
    buffer->capacity = BUFFER_INITIAL_CAPACITY;
}

void freeBuffer(Buffer* buffer) {
    for (size_t i = 0; i < buffer->line_count; i++) {
        freeLine(&buffer->lines[i]);
    }
    free(buffer->lines);
}

static void bufferEnsure(Buffer* buffer, size_t needed) {
    if (needed <= buffer->capacity) {
        return;
    }

    size_t cap = buffer->capacity ? buffer->capacity : BUFFER_INITIAL_CAPACITY;
    while (cap < needed) {
        cap += cap / 2;
    }

    Line* tmp = realloc(buffer->lines, sizeof(Line) * cap);
    if (!tmp) {
        ERROR("Out of memory");
    }

    buffer->lines = tmp;
    buffer->capacity = cap;
}

static void bufferCompact(Buffer* buffer) {
    if (buffer->capacity <= BUFFER_INITIAL_CAPACITY) {
        return;
    }

    if (buffer->line_count >= buffer->capacity / 2) {
        return;
    }

    size_t new_cap = buffer->line_count < BUFFER_INITIAL_CAPACITY ? BUFFER_INITIAL_CAPACITY : buffer->line_count;
    Line* tmp = realloc(buffer->lines, sizeof(Line) * new_cap);
    if (!tmp) {
        return;
    }

    buffer->lines = tmp;
    buffer->capacity = new_cap;
}

void insertChar(Buffer* buffer, Position* cursor, const char c) {
    Line* line = &buffer->lines[cursor->row];

    lineEnsure(line, line->length + 2);

    memmove(&line->chars[cursor->col + 1], &line->chars[cursor->col], line->length - cursor->col + 1);

    line->chars[cursor->col] = c;
    line->length++;

    cursor->col++;
}

void insertString(Buffer* buffer, Position* cursor, const char* text) {
    size_t text_length = strlen(text);

    Line* line = &buffer->lines[cursor->row];

    lineEnsure(line, line->length + text_length + 1);

    memmove(&line->chars[cursor->col + text_length], &line->chars[cursor->col], line->length - cursor->col + 1);
    memmove(&line->chars[cursor->col], text, text_length);

    line->length += text_length;

    cursor->col += text_length;
}

void replaceChar(Buffer* buffer, Position* cursor, const size_t count, const char c) {
    Line* line = &buffer->lines[cursor->row];

    if (cursor->col == line->length) {
        return;
    }

    for (size_t i = 0; i < count; i++) {
        line->chars[cursor->col] = c;
        cursor->col++;

        if (cursor->col == line->length) {
            break;
        }
    }

    cursor->col--;
}

void appendLine(Buffer* buffer, Position* cursor) {
    bufferEnsure(buffer, buffer->line_count + 1);

    memmove(&buffer->lines[cursor->row + 2], &buffer->lines[cursor->row + 1], sizeof(Line) * (buffer->line_count - cursor->row - 1));

    lineInit(&buffer->lines[cursor->row + 1]);
    buffer->line_count++;

    cursor->row++;
    cursor->col = 0;
}

void prependLine(Buffer* buffer, Position* cursor) {
    bufferEnsure(buffer, buffer->line_count + 1);

    memmove(&buffer->lines[cursor->row + 1], &buffer->lines[cursor->row], sizeof(Line) * (buffer->line_count - cursor->row));

    lineInit(&buffer->lines[cursor->row]);
    buffer->line_count++;

    cursor->col = 0;
}

void deleteRow(Buffer* buffer, Position* cursor, size_t row) {
    cursor->col = 0;

    if (cursor->row > row) {
        size_t tmp = cursor->row;
        cursor->row = row;
        row = tmp;
    }

    size_t count = row - cursor->row + 1;

    if (count > buffer->line_count) {
        ERROR("Row out of bounds");
    }

    if (count == buffer->line_count) {
        cursor->row = 0;

        freeBuffer(buffer);
        bufferInit(buffer);
        return;
    }

    if (row == buffer->line_count - 1) {
        cursor->row--;
    } else {
        memmove(&buffer->lines[cursor->row], &buffer->lines[cursor->row + count], sizeof(Line) * (buffer->line_count - cursor->row - count));
    }

    buffer->line_count -= count;
    bufferCompact(buffer);
}

static void deleteCol(Buffer* buffer, Position* cursor, size_t col) {
    Line* line = &buffer->lines[cursor->row];

    size_t count = col - cursor->col + 1;

    if (count > line->length) {
        ERROR("Column out of bounds");
    }

    if (count == line->length) {
        cursor->col = 0;

        freeLine(line);
        lineInit(line);
        return;
    }

    if (col == line->length - 1) {
        line->chars[cursor->col] = '\0';
        cursor->col--;
    } else {
        memmove(&line->chars[cursor->col], &line->chars[cursor->col + count], line->length - cursor->col - count + 1);
    }

    line->length -= count;
    lineCompact(line);
}

void deleteColLeft(Buffer* buffer, Position* cursor, size_t col) {
    cursor->col--;

    size_t tmp = cursor->col;
    cursor->col = col;
    col = tmp;

    deleteCol(buffer, cursor, col);
}

void deleteColRight(Buffer* buffer, Position* cursor, size_t col) {
    deleteCol(buffer, cursor, col);
}

void joinLines(Buffer* buffer, Position* cursor, size_t row) {
    if (cursor->row == row) {
        return;
    }

    // TODO: join lines from cursor.row to row
    //       set the cursor.col between the last joined line and the previous line

    deleteRow(buffer, &(Position){cursor->row + 1, 0}, row);
}
