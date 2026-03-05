#pragma once

typedef struct {
    char* chars;
    int length;
} Line;

typedef struct {
    Line* lines;
    int line_count;
} Buffer;
