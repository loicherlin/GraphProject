#include "../include/handler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

volatile sig_atomic_t interrupt_sigint = 0;
void interrupt(int sig)
{   
    switch(sig){
        // Ctrl + C
        case SIGINT:
            interrupt_sigint = 1;
            break;
        default:
            break;
    }
}