#pragma once

#include <stdbool.h>
#include <stddef.h>

#include "editor.h"
#include "motion.h"

bool isMovementToExecute(Editor* editor, Motion motion, const size_t count);

void movementLine(Editor* editor, size_t row);

void insertArrowLeft(Editor* editor);
void insertArrowUp(Editor* editor);
void insertArrowDown(Editor* editor);
void insertArrowRight(Editor* editor);
