#pragma once

#include <stdbool.h>

#include "line.h"
#include "parser.h"
#include "terminal.h"

void runVip(char* filename);
Line* getLine(size_t row);
char* getStatusLine(void);
KeyCache getKeyCache(void);
size_t getLineCount(void);
Position getCursorPosition(void);
Position getTextPosition(void);
bool isInStart(void);
