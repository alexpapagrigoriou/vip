#include "mode.h"

void enterNormalMode(Mode* mode) {
    *mode = NORMAL;
}

void enterInsertMode(Mode* mode) {
    *mode = INSERT;
}

void enterVisualMode(Mode* mode) {
    *mode = VISUAL;
}

void enterCommandMode(Mode* mode) {
    *mode = COMMAND;
}

void enterExitMode(Mode* mode) {
    *mode = EXIT;
}
