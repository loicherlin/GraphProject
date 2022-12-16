#pragma once
#include <stdbool.h>
#include <stdarg.h>

// Debug mode
extern bool _debug;

/**
 * Print a debug message
 * @param format The format string
 * @param ... The arguments
 */
void deprintf(const char *format, ...);