#pragma once

#include <stddef.h>

#include "editor.h"

void arrowLeft(Editor* editor, const size_t count);
void arrowUp(Editor* editor, const size_t count);
void arrowDown(Editor* editor, const size_t count);
void arrowRight(Editor* editor, const size_t count);

void insertArrowLeft(Editor* editor);
void insertArrowUp(Editor* editor);
void insertArrowDown(Editor* editor);
void insertArrowRight(Editor* editor);
