#include <signal.h>
#include <stdlib.h>

#include "buffer.h"
#include "input.h"
#include "terminal.h"

void initTerminal(void) {
    enableRawMode();
    atexit(disableRawMode);

    signal(SIGWINCH, handleResize);
    updateWindowSize();
    atexit(cleanScreen);

    initBuffer();
    atexit(freeBuffer);

    moveCursor(cursor);
}
