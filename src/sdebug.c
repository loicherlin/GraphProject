#include "../include/sdebug.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdbool.h>

bool _debug = false;

void deprintf(const char *format, ...)
{
    if(_debug){
        va_list args;
        va_start(args, format);
        printf("\nDEBUG: ");
        vprintf(format, args);
        va_end(args);
    }
}

void prprintf(char* task_name, int current, int size)
{
    printf("\r%s: [%d/%d]", task_name, current, size);
    fflush(stdout);
    // Print a new line at the end
    if(current == size){ printf("\n%s completed.\n", task_name); }
}
