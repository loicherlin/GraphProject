#include "../include/cprintf.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>

bool _debug = false;

void deprintf(const char *format, ...) {
    if (_debug) {
        va_list args;
        va_start(args, format);
        printf(ANSI_COLOR_YELLOW "\nDEBUG: ");
        vprintf(format, args);
        va_end(args);
        printf(ANSI_COLOR_RESET);
    }
}

void prprintf(char *task_name, int current, int size) {
    printf(ANSI_COLOR_CYAN "\r%s: [%d/%d] (%d/100%%)" ANSI_COLOR_RESET,
           task_name, current, size, (current * 100) / size);
    fflush(stdout);
    // Print a new line at the end
    if (current == size)
        printf(ANSI_COLOR_BLUE "\n%s completed.\n" ANSI_COLOR_RESET, task_name);
}

void eprintf(const char *format, ...) {
    va_list args;
    va_start(args, format);
    fprintf(stderr, ANSI_COLOR_RED "ERROR: ");
    vfprintf(stderr, format, args);
    va_end(args);
    printf(ANSI_COLOR_RESET);
}