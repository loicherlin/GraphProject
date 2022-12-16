#pragma once
#include <signal.h>
extern volatile sig_atomic_t interrupt_sigint;

void interrupt(int sig);