#include "draw.h"

#include <stdio.h>
#include <string.h>

#include "colors.h"
#include "vip.h"

void moveCursor(Position pos) {
    printf("\033[%zu;%zuH", pos.row + 1, pos.col + 1);
}

void fixTextPosition(Editor* editor) {
    Position max_buffer = getMaxBuffer();

    if (editor->cursor.row - editor->text.row >= max_buffer.row) {
        editor->text.row = editor->cursor.row - max_buffer.row + 1;
    } else if (editor->cursor.row < editor->text.row) {
        editor->text.row = editor->cursor.row;
    }

    if (editor->cursor.col - editor->text.col >= max_buffer.col) {
        editor->text.col = editor->cursor.col - max_buffer.col + 1;
    } else if (editor->cursor.col < editor->text.col) {
        editor->text.col = editor->cursor.col;
    }
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

    drawTildes(line_count);
}

static void drawStatusLine(void) {
    moveCursor((Position){getMaxScreen().row - 1, 0});
    cleanLine();
    printf("%.*s", (int)getMaxScreen().col, getStatusLine()->chars);
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
