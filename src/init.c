#include <signal.h>
#include <stdlib.h>

#include "buffer.h"
#include "command.h"
#include "input.h"
#include "terminal.h"

void initTerminal(void) {
    enableRawMode();
    atexit(disableRawMode);

    initBuffer();
    atexit(freeBuffer);

    initCommandLine();
    atexit(freeCommandLine);

    signal(SIGWINCH, handleResize);
    updateWindowSize();

    refreshWindow();

    atexit(cleanScreen);
}
