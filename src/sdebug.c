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
        printf("DEBUG: ");
        vprintf(format, args);
        va_end(args);
    }
}
