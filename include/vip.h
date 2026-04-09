#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "line.h"
#include "terminal.h"

void runVip(char* filename);
Line* getLine(size_t row);
char* getStatusLine(void);
char* getKeyCacheString(void);
char* getCursorPositionString(void);
size_t getLineCount(void);
Position getCursorPosition(void);
Position getTextPosition(void);
bool isInStart(void);
