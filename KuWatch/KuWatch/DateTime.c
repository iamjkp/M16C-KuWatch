#include "KuWatch.h"
#include "DateTime.h"
#include "../Drivers/qsk_lcd.h"
#include "../sfr62p.h"
#include <stdio.h>
#include <string.h>

// Prototypes
static void DateTimeView(Mode mode);

// Extern Variables
DateTime datetime;
extern State state;
extern unsigned long timerCount;

void InitDateTime(int year, int month, int day, int hour, int minute, int second) {
	// Set DateTime
	memset(&datetime, 0, sizeof(datetime));
	datetime.date.year = year;
	datetime.date.month = month;
	datetime.date.day = day;
	datetime.time.hour = hour;
	datetime.time.minute = minute;
	datetime.time.second = second;
}

// Called from ISR
void DateTimeCalc(void) {
	if (datetime.mode == NOTHING || datetime.mode == DAYOFWEEK) {
		if (incMilliSecond(&datetime.time, 5)) {
			if (incSecond(&datetime.time)) {
				if (incMinute(&datetime.time)) {
					if (incHour(&datetime.time)) {
						if (incDay(&datetime.date)) {
							if (incMonth(&datetime.date)) {
								incYear(&datetime.date);	
							}	
						}
					}
				}
			}
		}
	}
}

void DateTimeEvent(bool sw1, bool sw2, bool sw3) {
	if (state == DATETIME) {
		if (sw1 == true) {
			DisplayClear();
			
			switch(datetime.mode) {
				case SET_YEAR:
				case SET_MONTH:
				case SET_DAY:
				case SET_HOUR:
				case SET_MINUTE:
				case SET_SECOND:
					datetime.mode = NOTHING;
				break;
				case NOTHING:
					state = STOPWATCH;
				break;
			}
		}
	 
		if (sw2 == true) {
			switch (datetime.mode) {
				case NOTHING:
					datetime.mode = SET_YEAR;
				break;
				case SET_YEAR:
					datetime.mode = SET_MONTH;
				break;
				case SET_MONTH:
					datetime.mode = SET_DAY;
				break;
				case SET_DAY:
					datetime.mode = SET_HOUR;
				break;
				case SET_HOUR:
					datetime.mode = SET_MINUTE;
				break;
				case SET_MINUTE:
					datetime.mode = SET_SECOND;
				break;
				case SET_SECOND:
					datetime.mode = SET_YEAR;
				break;
				default:
				break;	
			}
		}
 
		if (sw3 == true) {
			switch (datetime.mode) {
				case NOTHING:
					DisplayClear();
					datetime.mode = DAYOFWEEK;
				break;
				case DAYOFWEEK:
					DisplayClear();
					datetime.mode = NOTHING;
				break;
				case SET_YEAR:
					incYear(&datetime.date);
				break;
				case SET_MONTH:
					incMonth(&datetime.date);
				break;
				case SET_DAY:
					incDay(&datetime.date);
				break;
				case SET_HOUR:;
					incHour(&datetime.time);
				break;
				case SET_MINUTE:
					incMinute(&datetime.time);
				break;
				case SET_SECOND:
					incSecond(&datetime.time);
				break;
				default:
				break;	
			}
		}
	}
}

void DateTimeState() {
	DateTimeView(datetime.mode);
}

static void DateTimeView(Mode mode) {
	char dateFormat[16] = {0,};
	char timeFormat[16] = {0,};
	char day_of_week[16] = {0,};
	
	sprintf(dateFormat, "%02d/%02d/%02d", datetime.date.year % 2000, datetime.date.month, datetime.date.day);
	sprintf(timeFormat, "%02d:%02d:%02d", datetime.time.hour, datetime.time.minute, datetime.time.second);
	
	switch(mode) {
		case NOTHING:
			DisplayString(LCD_LINE1, dateFormat);
			DisplayString(LCD_LINE2, timeFormat);
		break;
		case DAYOFWEEK:			
			DisplayString(LCD_LINE1, "Today Is");
			switch (datetime.date.day_of_week) {
				case SUNDAY:
					strcpy(day_of_week, "SUN");
				break;
				case MONDAY:
					strcpy(day_of_week, "MON");
				break;
				case TUESDAY:
					strcpy(day_of_week, "TUES");
				break;
				case WEDNESDAY:
					strcpy(day_of_week, "WED");
				break;
				case THURSDAY:
					strcpy(day_of_week, "THURS");
				break;
				case FRIDAY:
					strcpy(day_of_week, "FRI");
				break;
				case SATURDAY:
					strcpy(day_of_week, "SAT");
				break;
			}
			DisplayString(LCD_LINE2, day_of_week);
		break;
		case SET_YEAR:
			DisplayBlinkString(DELAY, 0, 1, LCD_LINE1, dateFormat);
			DisplayString(LCD_LINE2, timeFormat);
		break;
		case SET_MONTH:
			DisplayBlinkString(DELAY, 3, 4, LCD_LINE1, dateFormat);
			DisplayString(LCD_LINE2, timeFormat);
		break;
		case SET_DAY:
			DisplayBlinkString(DELAY, 6, 7, LCD_LINE1, dateFormat);
			DisplayString(LCD_LINE2, timeFormat);
		break;
		case SET_HOUR:
			DisplayString(LCD_LINE1, dateFormat);
			DisplayBlinkString(DELAY, 0, 1, LCD_LINE2, timeFormat);
		break;
		case SET_MINUTE:
			DisplayString(LCD_LINE1, dateFormat);
			DisplayBlinkString(DELAY, 3, 4, LCD_LINE2, timeFormat);
		break;
		case SET_SECOND:
			DisplayString(LCD_LINE1, dateFormat);
			DisplayBlinkString(DELAY, 6, 7, LCD_LINE2, timeFormat);
		break;
		default:
			DisplayString(LCD_LINE1, "ERROR!");
			DisplayString(LCD_LINE2, "!MODE");
		break;
	}
}

// Ultility
int zeller(int y, int m, int d) {
    int k, j, h;
 
    if (m <= 2)
    {
        m += 12;
        y--;
    }
 
    k = y % 100; 
    j = y / 100;
    h = 21*j/4 + 5*k/4 + 13*(m+1)/5 + d - 1;
 
    if (h < 0) h += 7; 
 
    return h % 7;
}

int incDay(Date* date) {
	int max_day = 30;
	
	// Setting Max day of Month and Calculate Leap Year
	switch (date->month) {
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			max_day = 31;	
			break;
		case 4:
		case 6:
		case 9:
		case 11:
			max_day = 30;
			break;
		case 2:
			if(((date->year % 4 == 0) && (date->year % 100 != 0)) || (date->year % 400 == 0)) max_day = 29; // leap
    		else max_day = 28;
			break;
	}
	
	
	// Change
	if ((date->day += 1) > max_day) {
		date->day = 1;
		// Calculate Day of Week 
		date->day_of_week = zeller(date->year, date->month, date->day);
		return 1;	
	}
	// Calculate Day of Week 
	date->day_of_week = zeller(date->year, date->month, date->day);
	return 0;
}

int incMonth(Date* date) {
	if ((date->month += 1) > 12) {
		date->month = 1;
		// Calculate Day of Week 
		date->day_of_week = zeller(date->year, date->month, date->day);
		return 1;	
	}
	// Calculate Day of Week 
		date->day_of_week = zeller(date->year, date->month, date->day);
	return 0;
}

int incYear(Date* date) {
	if ((date->year += 1) > 2099) {
		date->year = 2000;
		// Calculate Day of Week 
		date->day_of_week = zeller(date->year, date->month, date->day);
		return 1;	
	}
	// Calculate Day of Week 
		date->day_of_week = zeller(date->year, date->month, date->day);
	return 0;
}

// Inner Time Function Handler
void initTime(Time* time) {
	memset(time, 0, sizeof(*time));	
}

int incMilliSecond(Time* time, int value) {
	if ((time->milli_second += value) > 1000) {
		time->milli_second = 0;
		return 1;	
	}
	return 0;
}
int incSecond(Time* time) {
	if ((time->second += 1) > 59) {
		time->second = 0;
		return 1;	
	}
	return 0;
}
int incMinute(Time* time) {
	if ((time->minute += 1) > 59) {
		time->minute = 0;
		return 1;	
	}
	return 0;
}
int incHour(Time* time) {
	if ((time->hour += 1) > 23) {
		time->hour = 0;
		return 1;	
	}
	return 0;
}

int decMilliSecond(Time* time, int value) {
	if ( (time->milli_second -= value) > 1000) { // Because Unsigned
		time->milli_second = 995;
		return 1;
	}
	return 0;
}

int decSecond(Time* time) {
	if ( (time->second -= 1) > 60) { // Because Unsigned
		time->second = 59;
		return 1;
	}
	return 0;	
}

int decMinute(Time* time) {
	if ((time->minute -= 1) >= 59) {
		time->minute = 59;
		return 1;
	}
	return 0;
}

int decHour(Time* time) {
	if ((time->hour -= 1) > 24) {
		time->hour = 0;
		return 1;	
	}
	return 0;
}

bool isZero(Time *time) {
	if (time->hour == 0 && time->minute == 0 && time->second == 0) {
		return true;	
	}
	return false;
}

bool isEqualTime(Time t1, Time t2) {
	if (t1.hour == t2.hour && t1.minute == t2.minute && t1.second == t2.second) {
		return true;	
	}	
	return false;
}