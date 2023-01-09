#pragma once
#include <signal.h>

/**
 * @defgroup HandlerModule
 * @{
 * @brief Handle the interrupt signals.
 * @note SIGINT is only handled.
 */

/**
 * Used to store the interrupt signals to be handled.
 */
typedef struct
{
    /**
     * @brief Used to store the Ctrl + C signal.
     */
    volatile sig_atomic_t sigint;
} signal_int_t;

extern volatile signal_int_t _interrupt_signals;

/**
 * Handle the interrupt signals.
 * @param sig Signal to be handled.
 */
void interrupt(int sig);

/**
 * Initiate the handler.
 */
void initiate_handler();
/** @} */