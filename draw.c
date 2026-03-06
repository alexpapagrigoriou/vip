#include "draw.h"

#include <stdio.h>
#include <string.h>

#include "buffer.h"
#include "colors.h"
#include "terminal.h"
#include "vip.h"

static void printCenteredLines(const char* text[], int n_text) {
    int start_row = (screen.row - n_text) / 2;
    for (int i = 0; i < n_text; i++) {
        int col = (screen.col - strlen(text[i])) / 2;
        if (col < 0) {
            col = 0;
        }

        moveCursor((Position){start_row + i, col});
        printf("%s", text[i]);
    }
    fflush(stdout);
}

static void drawTildes(int start_row) {
    for (int r = start_row; r < screen.row - 1; r++) {
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
    int n_text = sizeof(text) / sizeof(text[0]);

    printCenteredLines(text, n_text);
    drawTildes(1);
}

static void drawBuffer(void) {
    int line_count = getLineCount();

    for (int r = 0; r < screen.row - 1; r++) {
        int draw_row = r + text.row;
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

    fflush(stdout);
}

void printLastLine(const char* text) {
    moveCursor((Position){screen.row - 1, 0});
    cleanLine();
    printf("%s", text);
    fflush(stdout);
}
