#include "draw.h"

#include <stdio.h>
#include <string.h>

#include "colors.h"
#include "vip.h"

#define SCROLL_OFF 8

void move_cursor(Position pos) {
    printf("\033[%zu;%zuH", pos.row + 1, pos.col + 1);
}

static void fix_text_row(Position cursor, Position* text, const size_t line_count, const size_t max_row) {
    if (cursor.row < text->row + SCROLL_OFF) {
        if (cursor.row < SCROLL_OFF) {
            text->row = 0;
        } else {
            text->row = cursor.row - SCROLL_OFF;
        }
        return;
    }

    if (line_count - text->row <= max_row) {
        return;
    }

    if (cursor.row - text->row + SCROLL_OFF > max_row - 1) {
        if (cursor.row + SCROLL_OFF > line_count - 1) {
            text->row = line_count - max_row;
        } else {
            text->row = cursor.row + SCROLL_OFF - max_row + 1;
        }
        return;
    }
}

static void fix_text_col(Position cursor, Position* text, const size_t max_col) {
    if (cursor.col < text->col) {
        text->col = cursor.col;
    } else if (cursor.col - text->col >= max_col) {
        text->col = cursor.col - max_col + 1;
    }
}

void fix_text_position(Position cursor, Position* text, const size_t line_count) {
    Position max_buffer = get_max_buffer();

    fix_text_row(cursor, text, line_count, max_buffer.row);
    fix_text_col(cursor, text, max_buffer.col);
}

void clean_screen(void) {
    printf("\033[H\033[2J");
}

void clean_line(void) {
    printf("\r\033[2K");
}

static void print_centered_lines(const char* text[], size_t text_length[], size_t n_text) {
    Position max_buffer = get_max_buffer();

    size_t start_row = (max_buffer.row - n_text) / 2;
    for (size_t i = 0; i < n_text; i++) {
        size_t col = (max_buffer.col - text_length[i]) / 2;
        move_cursor((Position){start_row + i, col});
        printf("%s", text[i]);
    }
}

static void draw_tildes(size_t start_row) {
    for (size_t r = start_row; r < get_max_buffer().row; r++) {
        move_cursor((Position){r, 0});
        printf(BLUE "~" RESET);
    }
}

static void draw_start(void) {
    const char* text[] = {
        "VIP - Vi rIPoff",
        "",
        "version idk",
        "by Alexandros Papagrigoriou.",
        "",
        "Vip is open source and freely distributable",
        "https://github.com/AlexPapagre/vip",
        "",
        "type  :q" BLUE "<Enter>" RESET "  to exit"};

    size_t n_text = sizeof(text) / sizeof(text[0]);

    size_t text_length[n_text];
    for (size_t i = 0; i < n_text; i++) {
        text_length[i] = strlen(text[i]);
    }
    text_length[8] -= COLOR_SIZE + RESET_COLOR_SIZE;

    print_centered_lines(text, text_length, n_text);
    draw_tildes(1);
}

static void draw_buffer(void) {
    Position max_buffer = get_max_buffer();
    size_t line_count = get_line_count();

    for (size_t r = 0; r < max_buffer.row; r++) {
        size_t draw_row = r + get_text_position().row;
        if (draw_row >= line_count) {
            break;
        }

        Line* line = get_line(draw_row);
        if (line->length <= get_text_position().col) {
            continue;
        }

        move_cursor((Position){r, 0});
        printf("%.*s", (int)max_buffer.col, line->chars + get_text_position().col);
    }

    draw_tildes(line_count - get_text_position().row);
}

static void draw_status_line(void) {
    size_t max_row = get_max_screen().row - 1;

    move_cursor((Position){max_row, 0});
    clean_line();

    int max_col = (int)get_max_screen().col - (KEY_CACHE_STRING_SIZE + CURSOR_POSITION_STRING_SIZE + 3);
    printf("%.*s", max_col, get_status_line());

    max_col++;

    move_cursor((Position){max_row, max_col});
    printf("%.*s", KEY_CACHE_STRING_SIZE, get_key_cache_string());

    max_col += KEY_CACHE_STRING_SIZE + 1;

    move_cursor((Position){max_row, max_col});
    printf("%.*s", CURSOR_POSITION_STRING_SIZE, get_cursor_position_string());
}

void draw_window(void) {
    clean_screen();

    if (is_in_start()) {
        draw_start();
    } else {
        draw_buffer();
    }

    draw_status_line();
}

void refresh_window(void) {
    Position buffer_pos = (Position){get_cursor_position().row - get_text_position().row, get_cursor_position().col - get_text_position().col};
    move_cursor(buffer_pos);

    fflush(stdout);
}
