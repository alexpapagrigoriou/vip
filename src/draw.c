#include "draw.h"

#include <stdio.h>
#include <string.h>

#include "buffer.h"
#include "colors.h"

bool in_start = true;

void moveCursor(Position pos) {
    printf("\033[%zu;%zuH", pos.row + 1, pos.col + 1);
}

static void resetCursor(void) {
    moveCursor(cursor);
}

void cleanScreen(void) {
    printf("\033[H\033[2J");
}

void cleanLine(void) {
    printf("\r\033[2K");
}

static void printCenteredLines(const char* text[], size_t n_text) {
    size_t start_row = (max_screen.row - n_text) / 2;
    for (size_t i = 0; i < n_text; i++) {
        size_t col = (max_screen.col - strlen(text[i])) / 2;
        moveCursor((Position){start_row + i, col});
        printf("%s", text[i]);
    }
}

static void drawTildes(size_t start_row) {
    for (size_t r = start_row; r < max_screen.row - 1; r++) {
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
    size_t line_count = getLineCount();

    for (size_t r = 0; r < max_screen.row - 1; r++) {
        size_t draw_row = r + text.row;
        if (draw_row >= line_count) {
            break;
        }

        moveCursor((Position){r, 0});
        printf("%.*s", (int)max_screen.col, getLine(draw_row)->chars + text.col);
    }

    drawTildes(line_count);
}

void drawWindow(void) {
    cleanScreen();

    if (in_start) {
        drawStart();
    } else {
        drawBuffer();
    }
}

void drawStatusLine(const char* text) {
    moveCursor((Position){max_screen.row - 1, 0});
    cleanLine();
    printf("%s", text);
}

void refreshWindow(void) {
    resetCursor();

    fflush(stdout);
}
