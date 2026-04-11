#pragma once

#define ERROR(msg) error(msg, __FILE__, __LINE__, __func__)

void error(const char* msg, const char* file, int line, const char* func);
