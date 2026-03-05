#include "terminal.h"

#include <stdlib.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

struct termios original;

void disableRawMode(void) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original);
}

void enableRawMode(void) {
    tcgetattr(STDIN_FILENO, &original);
    atexit(disableRawMode);

    struct termios raw = original;
    raw.c_lflag &= ~(ECHO | ICANON | ISIG);
    raw.c_iflag &= ~(IXON | ICRNL);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
