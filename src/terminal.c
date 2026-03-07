#include "terminal.h"

#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include "buffer.h"
#include "draw.h"

struct termios original;

Position screen;

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

void moveCursor(Position pos) {
    printf("\033[%zu;%zuH", pos.row + 1, pos.col + 1);
}

void resetCursor(void) {
    moveCursor(cursor);
}

void updateWindowSize(void) {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    screen.row = w.ws_row;
    screen.col = w.ws_col;

    drawWindow();
    resetCursor();
}

void cleanScreen(void) {
    printf("\033[H\033[2J");
}

void cleanLine(void) {
    printf("\r\033[2K");
}

void refreshWindow(void) {
    fflush(stdout);
}
