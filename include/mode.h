#pragma once

typedef enum {
    NORMAL,
    INSERT,
    VISUAL,
    COMMAND,
    EXIT
} Mode;

void enterNormalMode(Mode* mode);
void enterInsertMode(Mode* mode);
void enterVisualMode(Mode* mode);
void enterCommandMode(Mode* mode);
void enterExitMode(Mode* mode);
