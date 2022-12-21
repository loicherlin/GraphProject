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

/**
 * Print progression of a task
 * @param task_name The name of the task
 * @param current The current progression
 * @param size The total size of the task
 */
void prprintf(char* task_name, int current, int size);