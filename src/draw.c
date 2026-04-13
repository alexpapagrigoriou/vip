#include "draw.h"

#include <stdio.h>
#include <string.h>

#include "colors.h"
#include "vip.h"

#define SCROLL_OFF 8

void moveCursor(Position pos) {
    printf("\033[%zu;%zuH", pos.row + 1, pos.col + 1);
}

static void fixTextRow(Position cursor, Position* text, const size_t line_count, const size_t max_row) {
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

static void fixTextCol(Position cursor, Position* text, const size_t max_col) {
    if (cursor.col < text->col) {
        text->col = cursor.col;
    } else if (cursor.col - text->col >= max_col) {
        text->col = cursor.col - max_col + 1;
    }
}

void fixTextPosition(Position cursor, Position* text, const size_t line_count) {
    Position max_buffer = getMaxBuffer();

    fixTextRow(cursor, text, line_count, max_buffer.row);
    fixTextCol(cursor, text, max_buffer.col);
}

void cleanScreen(void) {
    printf("\033[H\033[2J");
}

void cleanLine(void) {
    printf("\r\033[2K");
}

static void printCenteredLines(const char* text[], size_t n_text) {
    Position max_buffer = getMaxBuffer();

    size_t start_row = (max_buffer.row - n_text) / 2;
    for (size_t i = 0; i < n_text; i++) {
        size_t col = (max_buffer.col - strlen(text[i])) / 2;
        moveCursor((Position){start_row + i, col});
        printf("%s", text[i]);
    }
}

static void drawTildes(size_t start_row) {
    for (size_t r = start_row; r < getMaxBuffer().row; r++) {
        moveCursor((Position){r, 0});
        printf(BLUE "~" RESET);
    }
}

static void drawStart(void) {
    const char* text[] = {
        "VIP - Vi rIPoff",
        "",
        "version idk",
        "by Alexandros Papagrigoriou.",
        "Vip is open source and freely distributable",
        "",
        "type  :q" BLUE "<Enter>" RESET "  to exit"};
    size_t n_text = sizeof(text) / sizeof(text[0]);

    printCenteredLines(text, n_text);
    drawTildes(1);
}

static void drawBuffer(void) {
    Position max_buffer = getMaxBuffer();
    size_t line_count = getLineCount();

    for (size_t r = 0; r < max_buffer.row; r++) {
        size_t draw_row = r + getTextPosition().row;
        if (draw_row >= line_count) {
            break;
        }

        Line* line = getLine(draw_row);
        if (line->length <= getTextPosition().col) {
            continue;
        }

        moveCursor((Position){r, 0});
        printf("%.*s", (int)max_buffer.col, line->chars + getTextPosition().col);
    }

    drawTildes(line_count - getTextPosition().row);
}

static void drawStatusLine(void) {
    size_t max_row = getMaxScreen().row - 1;

    moveCursor((Position){max_row, 0});
    cleanLine();

    int max_col = (int)getMaxScreen().col - (KEY_CACHE_STRING_SIZE + CURSOR_POSITION_STRING_SIZE + 3);
    printf("%.*s", max_col, getStatusLine());

    max_col++;

    moveCursor((Position){max_row, max_col});
    printf("%.*s", KEY_CACHE_STRING_SIZE, getKeyCacheString());

    max_col += KEY_CACHE_STRING_SIZE + 1;

    moveCursor((Position){max_row, max_col});
    printf("%.*s", CURSOR_POSITION_STRING_SIZE, getCursorPositionString());
}

void drawWindow(void) {
    cleanScreen();

    if (isInStart()) {
        drawStart();
    } else {
        drawBuffer();
    }

    drawStatusLine();
}

void refreshWindow(void) {
    Position buffer_pos = (Position){getCursorPosition().row - getTextPosition().row, getCursorPosition().col - getTextPosition().col};
    moveCursor(buffer_pos);

    fflush(stdout);
}
