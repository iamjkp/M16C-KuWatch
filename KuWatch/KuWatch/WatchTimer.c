#include "../Drivers/qsk_lcd.h"
#include "KuWatch.h"
#include "DateTime.h"
#include "WatchTimer.h"
#include <string.h>
#include <stdio.h>

static void WatchTimerView(Mode mode);

extern State state;
static Timer timer;

void InitWatchTimer() {
	memset(&timer, 0, sizeof(timer));
	//initTime(&timer.time);
}

void WatchTimerState() {
	WatchTimerView(timer.mode);
}

void WatchTimerCalc() {
	if (timer.mode == RUNNING) {
		if (!isZero(&timer.time)) {
			if (decMilliSecond(&timer.time, 5)) {
				if (decSecond(&timer.time)) {
					if (decMinute(&timer.time)) {
						decHour(&timer.time);
					}	
				}
			}
		} else {
			DISABLE_INTS
			timer.mode = NOTHING;
			changeState(false, TIMER_RING);
			ENABLE_INTS
		}
	}
}

void WatchTimerEvent(bool sw1, bool sw2, bool sw3) {
	if (sw1 == true) {
		switch (timer.mode) {
		case RUNNING:
		case NOTHING:
			state = ALARM;
		break;
		case SET_HOUR:
		case SET_MINUTE:
		case SET_SECOND:
			timer.mode = NOTHING;
		break;	
		}
	}
	
	if (sw2 == true) {
		switch (timer.mode) {
			case NOTHING:
				timer.mode = SET_HOUR;
			break;
			case SET_HOUR:
				timer.mode = SET_MINUTE;
			break;
			case SET_MINUTE:
				timer.mode = SET_SECOND;
			break;
			case SET_SECOND:
				timer.mode = SET_HOUR;
			break;	
		}
	}
	
	if (sw3 == true) {
		switch (timer.mode) {
			case NOTHING:
				if (!isZero(&timer.time)) {
					timer.mode = RUNNING;
				}
			break;
			case RUNNING:
				timer.mode = NOTHING;
			break;
			case SET_HOUR:
				incHour(&timer.time);
			break;
			case SET_MINUTE:
				incMinute(&timer.time);
			break;
			case SET_SECOND:
				incSecond(&timer.time);
			break;
		}
	}
}

static void WatchTimerView(Mode mode) {
	char timeFormat[16] = {0,};
	sprintf(timeFormat, "%02d:%02d:%02d", timer.time.hour, timer.time.minute, timer.time.second);
	
	switch (mode) {
		case RUNNING:
		case NOTHING:
			DisplayString(LCD_LINE2, timeFormat);
		break;
		case SET_HOUR:
			DisplayBlinkString(DELAY, 0, 1, LCD_LINE2, timeFormat);
		break;
		case SET_MINUTE:
			DisplayBlinkString(DELAY, 3, 4, LCD_LINE2, timeFormat);
		break;
		case SET_SECOND:
			DisplayBlinkString(DELAY, 6, 7, LCD_LINE2, timeFormat);
		break;	
	}
}