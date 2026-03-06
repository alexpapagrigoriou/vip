#include "input.h"

#include <sys/select.h>
#include <unistd.h>

#include "keys.h"
#include "terminal.h"

int readKey(void) {
    char c;
    if (read(STDIN_FILENO, &c, 1) == 1) {
        if (c == ESC) {
            char seq[2];
            struct timeval tv = {0, 10000};
            fd_set fds;
            FD_ZERO(&fds);
            FD_SET(STDIN_FILENO, &fds);

            if (select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) == 0)
                return ESC;
            if (read(STDIN_FILENO, &seq[0], 1) == 0)
                return ESC;
            if (read(STDIN_FILENO, &seq[1], 1) == 0)
                return ESC;

            if (seq[0] == '[') {
                switch (seq[1]) {
                    case 'A':
                        return UP_ARROW;
                    case 'B':
                        return DOWN_ARROW;
                    case 'C':
                        return RIGHT_ARROW;
                    case 'D':
                        return LEFT_ARROW;
                }
            }
            return ESC;
        }
        return c;
    }
    return -1;
}

int isPrintable(int c) {
    return c >= FIRST_PRINTABLE && c <= LAST_PRINTABLE;
}

void handleResize(int sig) {
    (void)sig;
    updateWindowSize();
}
