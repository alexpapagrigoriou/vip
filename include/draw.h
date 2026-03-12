#pragma once

#include <stdbool.h>

#include "terminal.h"

void moveCursor(Position pos);
void cleanScreen(void);
void cleanLine(void);
void drawWindow(void);
void drawStatusLine(const char* text);
void refreshWindow(void);
