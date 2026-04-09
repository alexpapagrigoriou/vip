#pragma once

#include <stdbool.h>

#include "line.h"
#include "status.h"
#include "terminal.h"

void runVip(char* filename);
Line* getLine(size_t row);
StatusLine* getStatusLine();
size_t getLineCount(void);
Position getCursorPosition(void);
Position getTextPosition(void);
bool isInStart(void);
