#include "../qsk_bsp.h"
#include "Timer.h"

// 01000000 value to load info TimerA0 
#define TIMER_CONFIG	0x40
#define TIMER_LEVEL		0x03

// Register Interrupt Handler
#pragma INTERRUPT /B TimerA0Int

static interrupt_handler timerA0_Handler;

/**
 * Initialize Tiemr A0
 * @param: interrupt handler: interrupt_handler
 */
void InitTimerA0(interrupt_handler handler) {
	if (handler != 0x0) {
		timerA0_Handler = handler;
	}
	
	// Set Period
	ta0 = 10000; // 5msec period
	
	_asm("fclr i");
	ta0ic |= TIMER_LEVEL;
	ta0mr = TIMER_CONFIG;
	_asm("fset i");
	
	// Fire Timer! 
	ta0s = 1;
}

/**
 * Timer A0 Interrupt Handler
 */
void TimerA0Int(void) {
	_asm("fset i");
	if (timerA0_Handler != 0x0) {
		timerA0_Handler();	
	}	
}