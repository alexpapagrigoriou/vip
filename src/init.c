#include <signal.h>
#include <stdlib.h>

#include "buffer.h"
#include "input.h"
#include "terminal.h"

void initTerminal(void) {
    enableRawMode();
    atexit(disableRawMode);

    initBuffer();
    atexit(freeBuffer);

    signal(SIGWINCH, handleResize);
    updateWindowSize();
    atexit(cleanScreen);
}
