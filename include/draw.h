#pragma once

#include <stdbool.h>

#include "terminal.h"

extern bool in_start;

void moveCursor(Position pos);
void cleanScreen(void);
void cleanLine(void);
void drawWindow(void);
void drawStatusLine(const char* text);
void refreshWindow(void);
