#pragma once

#include <stddef.h>

#include "editor.h"

void movementLeft(Editor* editor, const size_t count);
void movementUp(Editor* editor, const size_t count);
void movementDown(Editor* editor, const size_t count);
void movementRight(Editor* editor, const size_t count);

void insertArrowLeft(Editor* editor);
void insertArrowUp(Editor* editor);
void insertArrowDown(Editor* editor);
void insertArrowRight(Editor* editor);

void movementLine(Editor* editor, size_t row);
void movementFirstLine(Editor* editor, const size_t count);
void movementLastLine(Editor* editor, const size_t count);

void movementEndOfLine(Editor* editor);
