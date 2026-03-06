#include "vip.h"

#include <stdio.h>

#include "buffer.h"
#include "colors.h"
#include "draw.h"
#include "init.h"
#include "input.h"
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

        if (isPrintable(key)) {
            in_start = 0;

            insertChar(key);
        } else if (isEnter(key)) {
            appendLine();
        }

        drawWindow();

        char command[128];
        snprintf(command, sizeof(command), BGREEN "Key code: %d" RESET "\t(testing... press 'q' to exit)", key);
        printLastLine(command);

        moveCursor(cursor);
    }
}
