#pragma once
#include <signal.h>

typedef struct
{
    /**
     * @brief Used to store the Ctrl + C signal.
     */
    volatile sig_atomic_t sigint;
} signal_int_t;

/**
 * @brief Used to store the interrupt signals.
 */
extern volatile signal_int_t interrupt_signals;

/**
 * @brief Handle the interrupt signals.
 * @param sig Signal to be handled.
 */
void interrupt(int sig);

/**
 * @brief Initiate the handler.
 */
void initiate_handler();