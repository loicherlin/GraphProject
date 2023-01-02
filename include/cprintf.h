#pragma once
#include <stdbool.h>
#include <stdarg.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

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

/**
 * Print an error message
 * @param format The format string
 * @param ... The arguments
 */
void eprintf(const char *format, ...);