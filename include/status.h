#pragma once

typedef struct {
    char chars[128];
} StatusLine;

StatusLine* createStatusLine(void);
void freeStatusLine(StatusLine* status_line);
