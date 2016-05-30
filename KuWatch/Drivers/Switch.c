#include "../qsk_bsp.h"
#include "Switch.h"

#define SWITCH1_LEVEL 0x1
#define SWITCH2_LEVEL 0x1
#define SWITCH3	p8_1
#define SWITCH3_DIRECTION pd8_1

#pragma INTERRUPT Switch1_Handler
#pragma INTERRUPT Switch2_Handler

static switch_handler switch1;
static switch_handler switch2;
static switch_handler switch3;
static int switch3_state = 0;

void InitSwitch() {
	_asm("fset i");
	int0ic = SWITCH1_LEVEL;
	int1ic = SWITCH2_LEVEL;
	SWITCH3_DIRECTION = 0;
}

void Switch1_Handler(void) {
	_asm("fset i");
	if (switch1 != 0x0) {
		switch1();	
	}
}

void Switch2_Handler(void) {
	_asm("fset i");
	if (switch2 != 0x0) {
		switch2();	
	}
}

void Switch3_Polling(void) {	
	if (switch3_state == 0 && !SWITCH3) {
		switch3_state = 1;
		
		if (switch3 != 0x0) {
			switch3();	
		}		
	} else if (switch3_state == 1 && SWITCH3) {
		switch3_state = 0;
	}
}

void SetSwitch1Handler(switch_handler handler) {
	switch1 = handler;
}

void SetSwitch2Handler(switch_handler handler) {
	switch2 = handler;
}

void SetSwitch3Handler(switch_handler handler) {
	switch3 = handler;
}