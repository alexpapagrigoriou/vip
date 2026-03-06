#include "draw.h"

#include <stdio.h>
#include <string.h>

#include "buffer.h"
#include "colors.h"
#include "terminal.h"
#include "vip.h"

static void printCenteredLines(const char* text[], size_t n_text) {
    size_t start_row = (screen.row - n_text) / 2;
    for (size_t i = 0; i < n_text; i++) {
        size_t col = (screen.col - strlen(text[i])) / 2;
        moveCursor((Position){start_row + i, col});
        printf("%s", text[i]);
    }
    fflush(stdout);
}

static void drawTildes(size_t start_row) {
    for (size_t r = start_row; r < screen.row - 1; r++) {
        moveCursor((Position){r, 0});
        printf(BLUE "~" RESET);
    }
    fflush(stdout);
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

    for (size_t r = 0; r < screen.row - 1; r++) {
        size_t draw_row = r + text.row;
        if (draw_row >= line_count) {
            break;
        }

        moveCursor((Position){r, 0});
        printf("%s", getLine(draw_row)->chars);
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

    moveCursor(cursor);

    fflush(stdout);
}

void printLastLine(const char* text) {
    moveCursor((Position){screen.row - 1, 0});
    cleanLine();
    printf("%s", text);
    fflush(stdout);
}
