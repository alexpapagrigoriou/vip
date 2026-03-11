#include <signal.h>
#include <stdlib.h>

#include "buffer.h"
#include "draw.h"
#include "input.h"
#include "status.h"
#include "terminal.h"

void initTerminal(void) {
    enableRawMode();
    atexit(disableRawMode);

    initBuffer();
    atexit(freeBuffer);

    initStatusLine();
    atexit(freeStatusLine);

    signal(SIGWINCH, handleResize);
    updateWindowSize();

    atexit(cleanScreen);
}
