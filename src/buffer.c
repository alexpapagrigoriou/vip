#include "buffer.h"

#include <stdlib.h>
#include <string.h>

#include "error.h"

#define BUFFER_INITIAL_CAPACITY 8

void buffer_init(Buffer* buffer) {
    buffer->lines = malloc(sizeof(Line) * BUFFER_INITIAL_CAPACITY);
    line_init(&buffer->lines[0]);
    buffer->line_count = 1;
    buffer->capacity = BUFFER_INITIAL_CAPACITY;
}

void free_buffer(Buffer* buffer) {
    for (size_t i = 0; i < buffer->line_count; i++) {
        free_line(&buffer->lines[i]);
    }
    free(buffer->lines);
}

static void buffer_ensure(Buffer* buffer, size_t needed) {
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

static void buffer_compact(Buffer* buffer) {
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

void insert_char(Buffer* buffer, Position* cursor, const char c) {
    Line* line = &buffer->lines[cursor->row];

    line_ensure(line, line->length + 2);

    memmove(&line->chars[cursor->col + 1], &line->chars[cursor->col], line->length - cursor->col + 1);

    line->chars[cursor->col] = c;
    line->length++;

    cursor->col++;
}

void insert_string(Buffer* buffer, Position* cursor, const char* text) {
    size_t text_length = strlen(text);

    Line* line = &buffer->lines[cursor->row];

    line_ensure(line, line->length + text_length + 1);

    memmove(&line->chars[cursor->col + text_length], &line->chars[cursor->col], line->length - cursor->col + 1);
    memmove(&line->chars[cursor->col], text, text_length);

    line->length += text_length;

    cursor->col += text_length;
}

void replace_char(Buffer* buffer, Position* cursor, const size_t count, const char c) {
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

void append_line(Buffer* buffer, Position* cursor) {
    buffer_ensure(buffer, buffer->line_count + 1);

    memmove(&buffer->lines[cursor->row + 2], &buffer->lines[cursor->row + 1], sizeof(Line) * (buffer->line_count - cursor->row - 1));

    line_init(&buffer->lines[cursor->row + 1]);
    buffer->line_count++;

    cursor->row++;
    cursor->col = 0;
}

void prepend_line(Buffer* buffer, Position* cursor) {
    buffer_ensure(buffer, buffer->line_count + 1);

    memmove(&buffer->lines[cursor->row + 1], &buffer->lines[cursor->row], sizeof(Line) * (buffer->line_count - cursor->row));

    line_init(&buffer->lines[cursor->row]);
    buffer->line_count++;

    cursor->col = 0;
}

void delete_row(Buffer* buffer, Position* cursor, size_t row) {
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

        free_buffer(buffer);
        buffer_init(buffer);
        return;
    }

    if (row == buffer->line_count - 1) {
        cursor->row--;
    } else {
        memmove(&buffer->lines[cursor->row], &buffer->lines[cursor->row + count], sizeof(Line) * (buffer->line_count - cursor->row - count));
    }

    buffer->line_count -= count;
    buffer_compact(buffer);
}

static void delete_col(Buffer* buffer, Position* cursor, size_t col) {
    Line* line = &buffer->lines[cursor->row];

    size_t count = col - cursor->col + 1;

    if (count > line->length) {
        ERROR("Column out of bounds");
    }

    if (count == line->length) {
        cursor->col = 0;

        free_line(line);
        line_init(line);
        return;
    }

    if (col == line->length - 1) {
        line->chars[cursor->col] = '\0';
        cursor->col--;
    } else {
        memmove(&line->chars[cursor->col], &line->chars[cursor->col + count], line->length - cursor->col - count + 1);
    }

    line->length -= count;
    line_compact(line);
}

void delete_col_left(Buffer* buffer, Position* cursor, size_t col) {
    cursor->col--;

    size_t tmp = cursor->col;
    cursor->col = col;
    col = tmp;

    delete_col(buffer, cursor, col);
}

void delete_col_right(Buffer* buffer, Position* cursor, size_t col) {
    delete_col(buffer, cursor, col);
}

void join_lines(Buffer* buffer, Position* cursor, size_t row) {
    if (cursor->row == row) {
        return;
    }

    // TODO: join lines from cursor.row to row
    //       set the cursor.col between the last joined line and the previous line

    delete_row(buffer, &(Position){cursor->row + 1, 0}, row);
}
