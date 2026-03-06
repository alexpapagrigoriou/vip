#include "input.h"

#include <sys/select.h>
#include <unistd.h>

#include "terminal.h"

int readKey(void) {
    char c;
    if (read(STDIN_FILENO, &c, 1) == 1) {
        if (c == 27) {
            char seq[2];
            struct timeval tv = {0, 10000};
            fd_set fds;
            FD_ZERO(&fds);
            FD_SET(STDIN_FILENO, &fds);

            if (select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) == 0)
                return 27;
            if (read(STDIN_FILENO, &seq[0], 1) == 0)
                return 27;
            if (read(STDIN_FILENO, &seq[1], 1) == 0)
                return '\x1b';

            if (seq[0] == '[') {
                switch (seq[1]) {
                    case 'A':
                        return 1000;
                    case 'B':
                        return 1001;
                    case 'C':
                        return 1002;
                    case 'D':
                        return 1003;
                }
            }
            return 27;
        }
        return c;
    }
    return -1;
}

int isEnter(int c) {
    return c == 13;
}

int isPrintable(int c) {
    return c >= 32 && c <= 126;
}

void handleResize(int sig) {
    (void)sig;
    updateWindowSize();
}
