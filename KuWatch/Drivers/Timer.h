#ifndef TIMER_H
#define TIMER_H

// Typedefine for Interrupt Handler Function
typedef void (*interrupt_handler)(void);

// Prototypes
void InitTimerA0(interrupt_handler);

#endif