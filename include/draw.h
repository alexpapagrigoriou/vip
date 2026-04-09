#pragma once

#include "terminal.h"

#define KEY_CACHE_SIZE 10
#define CURSOR_POSITION_SIZE 10

void moveCursor(Position pos);
void fixTextPosition(Position cursor, Position* text);
void cleanScreen(void);
void cleanLine(void);
void drawWindow(void);
void refreshWindow(void);
