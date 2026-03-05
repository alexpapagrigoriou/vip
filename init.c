#include "terminal.h"

void initTerminal(void) {
    enableRawMode();
    autoCleanup();
    updateWindowSize();
    moveCursor(cursor_row, cursor_col);
}
