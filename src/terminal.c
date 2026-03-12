#include "terminal.h"

#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include "draw.h"

struct termios original;

Position max_screen;

void disableRawMode(void) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original);
}

void enableRawMode(void) {
    tcgetattr(STDIN_FILENO, &original);

    struct termios raw = original;
    raw.c_lflag &= ~(ECHO | ICANON | ISIG);
    raw.c_iflag &= ~(IXON | ICRNL);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void updateWindowSize(void) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    max_screen.row = w.ws_row;
    max_screen.col = w.ws_col;

    drawWindow();

    refreshWindow();
}

Position getMaxScreen(void) {
    return max_screen;
}
