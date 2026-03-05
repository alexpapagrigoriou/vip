#include <signal.h>
#include <stdlib.h>

#include "input.h"
#include "terminal.h"

void initTerminal(void) {
    enableRawMode();

    signal(SIGWINCH, handleResize);
    updateWindowSize();

    atexit(disableRawMode);
    atexit(cleanScreen);

    moveCursor(cursor);
}
