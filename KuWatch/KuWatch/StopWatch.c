#include "KuWatch.h"
#include "DateTime.h"
#include "StopWatch.h"
#include "../Drivers/qsk_lcd.h"
#include <stdio.h>
#include <string.h>

static void StopWatchView(Mode mode);

extern State state;
static StopWatch stopwatch;

void InitStopWatch() {
	memset(&stopwatch, 0, sizeof(stopwatch));
	initTime(&stopwatch.time);
	initTime(&stopwatch.lap);
}

void StopWatchCalc() {
	if (stopwatch.mode == RUNNING) {
		Time *time = &stopwatch.time;
		
		if (incMilliSecond(time, 5)) {
			if (incSecond(time)) {
				if (incMinute(time)) {
					incHour(time);
				}	
			}	
		}	
	}
}

void StopWatchState() {
	StopWatchView(stopwatch.mode);
}

void StopWatchEvent(bool sw1, bool sw2, bool sw3) {
	// Switch1 Event Handler
	if (sw1 == true) {
		DisplayClear();
		
		switch(stopwatch.mode) {
			case NOTHING:
			case RUNNING:
				state = TIMER;
			break;
		}
	}
 
 	// Switch2 Event Handler
	if (sw2 == true) {
		switch(stopwatch.mode) {
			case NOTHING:
				stopwatch.mode = RUNNING;
			break;
			case RUNNING:
				stopwatch.mode = NOTHING;
			break;
		}
	}

	// Switch3 Event Handler
	if (sw3 == true) {
		switch(stopwatch.mode) {
			case NOTHING:
				// Reset
				initTime(&stopwatch.time);
				initTime(&stopwatch.lap);
			break;
			case RUNNING:
				// Lap
				memcpy(&stopwatch.lap, &stopwatch.time, sizeof(stopwatch.time));
			break;
		}
	}
}

static void StopWatchView(Mode mode) {
	char lapFormat[16] = {0,};
	char timeFormat[16] = {0,};
	
	Time time = stopwatch.time;
	Time lap = stopwatch.lap;
	
	sprintf(timeFormat, "%02d:%02d.%02d", time.minute, time.second, time.milli_second / 10);
	sprintf(lapFormat, "%02d:%02d.%02d", lap.minute, lap.second, lap.milli_second / 10);
	
	switch (mode) {
		case NOTHING:
		case RUNNING:
		DisplayString(LCD_LINE1, lapFormat);
		DisplayString(LCD_LINE2, timeFormat);
		break;	
	}
}

