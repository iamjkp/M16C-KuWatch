#include "../Drivers/qsk_lcd.h"
#include "KuWatch.h"
#include "DateTime.h"
#include "Alarm.h"
#include <string.h>
#include <stdio.h>

static void AlarmView(void);

extern State state;
extern DateTime datetime;
static Alarm alarm[ALARM_CNT];
static Mode mode;
static int curr_alarm_idx = 0;


void InitAlarm(void) {
	memset(alarm, 0, sizeof(Alarm)*ALARM_CNT);
}

void AlarmState(void) {
	AlarmView();
}

void AlarmChecker(void) {
	int i = 0;
	for (i = 0 ; i < ALARM_CNT ; i++) {
		Alarm *a = &alarm[i];
		
		if (a->isOn == true) {
			if (isEqualTime(a->time, datetime.time)) {
				a->isOn = false;
				DISABLE_INTS
				changeState(false, ALARM_RING);
				ENABLE_INTS
			}
		}	
	}
}

void AlarmEvent(bool sw1, bool sw2, bool sw3) {
	if (sw1 == true) {
		switch (mode) {
			case NOTHING:
				state = DATETIME;
			break;	
			case SET_HOUR:
			case SET_MINUTE:
			case SET_SECOND:
			case SET_ONOFF:
				mode = NOTHING;
			break;
		}
	}
	
	if (sw2 == true) {
		DisplayClear();
		switch (mode) {
			case NOTHING:	
				mode = SET_HOUR;
			break;
			case SET_HOUR:
				mode = SET_MINUTE;
			break;
			case SET_MINUTE:
				mode = SET_SECOND;
			break;
			case SET_SECOND:
				mode = SET_ONOFF;
			break;
			case SET_ONOFF:
				mode = SET_HOUR;
			break;
		}
	}
	
	if (sw3 == true) {
		switch (mode) {
			case NOTHING:
				// Scroll Down
				if (curr_alarm_idx < ALARM_CNT-1) curr_alarm_idx++;
				else curr_alarm_idx = 0;
			break;
			case SET_HOUR:
				incHour(&alarm[curr_alarm_idx].time);
			break;
			case SET_MINUTE:
				incMinute(&alarm[curr_alarm_idx].time);
			break;
			case SET_SECOND:
				incSecond(&alarm[curr_alarm_idx].time);
			break;
			case SET_ONOFF:
				DisplayClear();
				alarm[curr_alarm_idx].isOn = !alarm[curr_alarm_idx].isOn;
			break;
		}
	}
}

static void ScrollView(void) {
	int i = 0;
	char buf[16] = {0, };
	
	// Draw Two Lines
	for (i = 0 ; i < 2 ; i++) {
		char bar = ' ';
		char sel = ' ';
		Alarm a = alarm[(curr_alarm_idx/2)*2 + i];
	
		memset(buf, 0, sizeof(buf));
		
		if (a.isOn == true) {
			bar = '$';
		}	
		
		if (curr_alarm_idx%2 == i) {
			sel = 0xFF;
		} else {
			sel = bar;
		}
		
		sprintf(buf, "ALARM %d", (curr_alarm_idx/2)*2 + (i + 1));
		
		if (i == 0) {
			DisplayBlinkChar(DELAY, 0, LCD_LINE1, bar, sel, buf);
		} else {
			DisplayBlinkChar(DELAY, 0, LCD_LINE2, bar, sel, buf);
		}
	}	
}

static void CurrentAlarmView(Mode mode) {
	char alarmFormat[16] = {0, };
	char timeFormat[16] = {0, };
	char str[16] = {0,};
	
	Alarm a = alarm[curr_alarm_idx];
	
	sprintf(timeFormat, "%02d:%02d:%02d", a.time.hour, a.time.minute, a.time.second);
	sprintf(alarmFormat, "ALARM %d", curr_alarm_idx + 1);
	
	switch (mode) {
		case SET_HOUR:
			DisplayString(LCD_LINE1, alarmFormat);
			DisplayBlinkString(DELAY, 0, 1, LCD_LINE2, timeFormat);
		break;
		case SET_MINUTE:
			DisplayString(LCD_LINE1, alarmFormat);
			DisplayBlinkString(DELAY, 3, 4, LCD_LINE2, timeFormat);
		break;
		case SET_SECOND:
			DisplayString(LCD_LINE1, alarmFormat);
			DisplayBlinkString(DELAY, 6, 7, LCD_LINE2, timeFormat);
		break;
		case SET_ONOFF:
			if (a.isOn == true) sprintf(str, "CURR:ON");
			else sprintf(str, "CURR:OFF");
			DisplayBlinkString(DELAY, 5, 7, LCD_LINE1, str);
			DisplayString(LCD_LINE2, timeFormat);
		break;	
	}
}

static void AlarmView(void) {
	switch (mode) {
		case NOTHING:
			ScrollView();
		break;
		case SET_HOUR:
			CurrentAlarmView(mode);
		break;
		case SET_MINUTE:
			CurrentAlarmView(mode);
		break;
		case SET_SECOND:
			CurrentAlarmView(mode);
		break;
		case SET_ONOFF:
			CurrentAlarmView(mode);
		break;	
	}
}