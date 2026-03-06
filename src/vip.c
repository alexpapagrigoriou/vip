#include "vip.h"

#include <stdio.h>

#include "buffer.h"
#include "colors.h"
#include "draw.h"
#include "init.h"
#include "input.h"
#include "keys.h"
#include "terminal.h"

bool in_start = true;
bool in_command = false;

void runVip(void) {
    initTerminal();

    while (1) {
        int key = readKey();
        if (key == 'q') {
            break;
        }
        in_start = false;

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
