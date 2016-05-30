#include "../sfr62p.h"
#include "../Drivers/LED.h"
#include "../Drivers/Switch.h"
#include "../Drivers/Timer.h"
#include "../Drivers/qsk_lcd.h"
#include <string.h>
#include "KuWatch.h"
#include "DateTime.h"
#include "StopWatch.h"
#include "WatchTimer.h"
#include "Alarm.h"

State state = INIT;
unsigned long timerCount;
static bool switch1_event = false;
static bool switch2_event = false;
static bool switch3_event = false;

static void TimerInterruptHandler() {
	timerCount += 5;
	
	DateTimeCalc();	
	StopWatchCalc();
	AlarmChecker();
	WatchTimerCalc();
}

static void Switch1_Handler() {
	switch1_event = true;
}

static void Switch2_Handler() {
	switch2_event = true;
}

static void Switch3_Handler() {
	switch3_event = true;
}

void KuWatch(void) {
	// Initialize
	InitDateTime(2015, 11, 8, 9, 41, 0);
	InitStopWatch();
	InitWatchTimer();
	InitAlarm();
	InitLED();
	InitSwitch();
	InitDisplay();
	InitTimerA0(&TimerInterruptHandler);
	
	// Register Switch Handler
	SetSwitch1Handler(&Switch1_Handler);
	SetSwitch2Handler(&Switch2_Handler);
	SetSwitch3Handler(&Switch3_Handler);
	
	// Change to Default State
	state = DATETIME;
	
	while (true) {
		// Polling Switch 3 
		Switch3_Polling();	
		
		switch (state) {
			case DATETIME:
				DateTimeState();
			break;
			case STOPWATCH:
				StopWatchState();
			break;
			case TIMER:
				WatchTimerState();
			break;
			case ALARM:
				AlarmState();
			break;
			
			case ALARM_RING:
				OnAllLED();
				DisplayBlinkString(DELAY, 0, 7, LCD_LINE1, "ALARM!!!");
				DisplayBlinkString(DELAY, 0, 7, LCD_LINE2, "RINGRING");
			break;
			case TIMER_RING:
				OnAllLED();
				DisplayBlinkString(DELAY, 0, 7, LCD_LINE1, "TIMER!!!");
				DisplayBlinkString(DELAY, 0, 7, LCD_LINE2, "RINGRING");
			break;
			default:
			break;
		}
		
		// Watching Switch Events
		if (switch1_event || switch2_event || switch3_event) {
			switch (state) {
				case DATETIME:
					DateTimeEvent(switch1_event, switch2_event, switch3_event);
				break;
				case STOPWATCH:
					StopWatchEvent(switch1_event, switch2_event, switch3_event);
				break;	
				case TIMER:
					WatchTimerEvent(switch1_event, switch2_event, switch3_event);
				break;
				case ALARM:
					AlarmEvent(switch1_event, switch2_event, switch3_event);
				break;
				case ALARM_RING:
				case TIMER_RING:
					if (switch3_event == true) {
						OffAllLED();
						DisplayClear();
						changeState(true, INIT);
					}
				break;
			}
			
			// Disable Switch Events
			switch1_event = false;
			switch2_event = false;
			switch3_event = false;
		}
	}
}

static State before = INIT;
void changeState(bool restore, State st) {
	if (restore == true) {
		state = before;
		before = st;
	} else {
		before = state;
		state = st;
	}	
}