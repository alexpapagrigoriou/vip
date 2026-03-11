#include "vip.h"

#include <stdbool.h>
#include <stdio.h>

#include "buffer.h"
#include "colors.h"
#include "draw.h"
#include "init.h"
#include "input.h"
#include "keys.h"
#include "terminal.h"

bool in_command = false;

bool save_curosr_col;
size_t prev_cursor_col = 0;

void runVip(void) {
    initTerminal();

    while (1) {
        int key = readKey();

        in_start = false;
        save_curosr_col = true;

        if (key == 'q') {
            break;
        } else if (key == TAB) {
            insertString("    ");
        } else if (key == ENTER) {
            appendLine();
        } else if (key == '1') {
            deleteChar();
        } else if (key == '2') {
            prependLine();
        } else if (key == '3') {
            deleteLine();
        } else if (isPrintable(key)) {
            insertChar(key);
        } else if (key == UP) {
            save_curosr_col = false;

            if (cursor.row > 0) {
                cursor.row--;

                size_t line_length = getLine(cursor.row)->length;
                if (prev_cursor_col < line_length) {
                    cursor.col = prev_cursor_col;
                } else {
                    cursor.col = line_length;
                }
            }
        } else if (key == DOWN) {
            save_curosr_col = false;

            if (cursor.row < getLineCount() - 1) {
                cursor.row++;

                size_t line_length = getLine(cursor.row)->length;
                if (prev_cursor_col < line_length) {
                    cursor.col = prev_cursor_col;
                } else {
                    cursor.col = line_length;
                }
            }
        } else if (key == RIGHT) {
            if (cursor.col < getLine(cursor.row)->length) {
                cursor.col++;
            } else {
                save_curosr_col = false;
            }
        } else if (key == LEFT) {
            if (cursor.col > 0) {
                cursor.col--;
            } else {
                save_curosr_col = false;
            }
        }

        drawWindow();

        if (save_curosr_col) {
            prev_cursor_col = cursor.col;
        }

        char status_line[128];
        snprintf(status_line, sizeof(status_line), BGREEN "Key code: %d" RESET "\trow: %zu, col: %zu, prev_col: %zu", key, cursor.row, cursor.col, prev_cursor_col);
        drawStatusLine(status_line);

        refreshWindow();
    }
}
