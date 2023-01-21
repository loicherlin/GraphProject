#include "../include/cprintf.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>

bool _debug = false;

void deprintf(const char *format, ...)
{
    if (_debug)
    {
        va_list args;
        va_start(args, format);
        printf(ANSI_COLOR_YELLOW "\nDEBUG: ");
        vprintf(format, args);
        va_end(args);
        printf(ANSI_RESET);
    }
}

void prprintf(char *task_name, int current, int size)
{
    printf(ANSI_COLOR_CYAN "\r%s: [%d/%d] (%d/100%%)" ANSI_RESET, task_name, current, size,
           (current * 100) / size);
    fflush(stdout);
    // Print a new line at the end
    if (current == size)
        printf(ANSI_COLOR_BLUE "\n%s completed.\n" ANSI_RESET, task_name);
}

void eprintf(const char *msg, const char *file, int line)
{
    fprintf(stderr, ANSI_BOLD ANSI_COLOR_RED "ERROR: " ANSI_RESET ANSI_BOLD);
    fprintf(stderr, "at %s:%d: " ANSI_RESET, file, line);
    fprintf(stderr, "%s", msg);
    fprintf(stderr, ANSI_RESET);
}

noreturn void chprintf(int syserr, const char *file, int line, const char *info, const char *msg, ...)
{
    va_list ap;
    va_start(ap, msg);
    eprintf("", file, line);
    fprintf(stderr, "%s\n", info);
    fprintf(stderr, "\t| " ANSI_COLOR_CYAN ANSI_BOLD);
    vfprintf(stderr, msg, ap);
    fprintf(stderr, ANSI_RESET "\n");
    va_end(ap);
    if (syserr == 1)
    {
        fprintf(stderr, "\t| " ANSI_BOLD ANSI_COLOR_RED "PERROR: " ANSI_RESET);
        perror("");
        fprintf(stderr, ANSI_RESET);
    }
    fprintf(stderr, "\n");
    exit(EXIT_FAILURE);
}