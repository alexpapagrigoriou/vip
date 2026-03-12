#pragma once

#include <stdbool.h>

#include "terminal.h"
#include "vip.h"

void moveCursor(Position pos);
void fixTextPosition(Editor* editor);
void cleanScreen(void);
void cleanLine(void);
void drawWindow(void);
void refreshWindow(void);
