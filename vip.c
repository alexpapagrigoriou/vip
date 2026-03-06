#include "vip.h"

#include <stdio.h>

#include "buffer.h"
#include "colors.h"
#include "draw.h"
#include "init.h"
#include "input.h"
#include "keys.h"
#include "terminal.h"

int in_start = 1;
int in_command = 0;

void runVip(void) {
    initTerminal();

    while (1) {
        int key = readKey();
        if (key == 'q') {
            break;
        }
        in_start = 0;

        if (isPrintable(key)) {
            insertChar(key);
        } else if (key == TAB) {
            for (int i = 0; i < 4; i++) {
                insertChar(' ');
            }
        } else if (key == ENTER) {
            appendLine();
        }

        drawWindow();

        char command[128];
        snprintf(command, sizeof(command), BGREEN "Key code: %d" RESET "\t(testing... press 'q' to exit)", key);
        printLastLine(command);

        moveCursor(cursor);
    }
}
