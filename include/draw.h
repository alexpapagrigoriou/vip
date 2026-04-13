#pragma once

#include "terminal.h"

#define KEY_CACHE_STRING_SIZE 10
#define CURSOR_POSITION_STRING_SIZE 10

void moveCursor(Position pos);
void fixTextPosition(Position cursor, Position* text, const size_t line_count);
void cleanScreen(void);
void cleanLine(void);
void drawWindow(void);
void refreshWindow(void);
