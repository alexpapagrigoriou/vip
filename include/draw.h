#pragma once

#include "editor.h"
#include "terminal.h"

#define KEY_CACHE_SIZE 10
#define CURSOR_POSITION_SIZE 10

void moveCursor(Position pos);
void fixTextPosition(Editor* editor);
void cleanScreen(void);
void cleanLine(void);
void drawWindow(void);
void refreshWindow(void);
