#include "draw.h"

#include <stdio.h>
#include <string.h>

#include "colors.h"
#include "terminal.h"

int in_start = 1;

static void printCenteredLines(const char* text[], int n_text) {
    int start_row = (rows - n_text) / 2;
    for (int i = 0; i < n_text; i++) {
        int col = (cols - strlen(text[i])) / 2;
        if (col < 0) {
            col = 0;
        }

        moveCursor(start_row + i + 1, col + 1);
        printf("%s", text[i]);
    }
    fflush(stdout);
}

static void drawTildes(int start_row) {
    for (int r = start_row; r < rows; r++) {
        moveCursor(r, 1);
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
    drawTildes(2);
}

static void drawBuffer(void) {
    // TODO: print text_buffer if you start editing
    // drawTildes(n_lines);
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
    moveCursor(rows, 1);
    cleanLine();
    printf("%s", text);
    fflush(stdout);
}
