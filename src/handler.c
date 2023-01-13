#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "../include/handler.h"


volatile signal_int_t _interrupt_signals = {0};

void interrupt(int sig)
{   
    switch(sig){
        // Ctrl + C
        case SIGINT:
            _interrupt_signals.sigint = 1;
            break;
        default:
            break;
    }
}

void initiate_handler()
{
    struct sigaction action;
    // Our handler
    action.sa_handler = interrupt;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    // Register Ctrl + C handler
    sigaction(SIGINT, &action, NULL);
}