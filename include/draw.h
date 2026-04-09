#pragma once

#include "editor.h"
#include "terminal.h"

void moveCursor(Position pos);
void fixTextPosition(Editor* editor);
void cleanScreen(void);
void cleanLine(void);
void drawWindow(void);
void refreshWindow(void);
