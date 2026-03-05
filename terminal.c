#include "terminal.h"

#include <stdio.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include "draw.h"

struct termios original;

int rows = 0, cols = 0;
int cursor_row = 1, cursor_col = 1;
int text_row = 0, text_col = 0;

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
    rows = w.ws_row;
    cols = w.ws_col;

    drawWindow();
}

void moveCursor(int row, int col) {
    printf("\033[%d;%dH", row, col);
    fflush(stdout);
}

void cleanScreen(void) {
    printf("\033[H\033[2J");
    fflush(stdout);
}

void cleanLine(void) {
    printf("\r\033[2K");
    fflush(stdout);
}
