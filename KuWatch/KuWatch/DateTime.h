#ifndef DATETIME_H
#define DATETIME_H

// Define Date Type
typedef struct _DATE {
	unsigned int year;
	unsigned int month;
	unsigned int day;
	Day day_of_week;
} Date;

// Define Time Type
typedef struct _TIME {
	unsigned int hour;
	unsigned int minute;
	unsigned int second;
	unsigned int milli_second;
} Time;

// Define Data Structure
typedef struct _DateTime {
	Date date;
	Time time;
	Mode mode;
} DateTime;

// Prototypes
void InitDateTime(int year, int month, int day, int hour, int minute, int second);
void DateTimeCalc(void);
void DateTimeEvent(bool sw1, bool sw2, bool sw3);
void DateTimeState(void);

// Date Function
int incDay(Date*);
int incMonth(Date*);
int incYear(Date*);

// Time Function Handler
void initTime(Time*);
int incMilliSecond(Time*, int);
int incSecond(Time*);
int incMinute(Time*);
int incHour(Time*);
int decMilliSecond(Time*, int);
int decSecond(Time*);
int decMinute(Time*);
int decHour(Time*);

// DateTime Utilities
int zeller(int y, int m, int d);
bool isZero(Time *time);
bool isEqualTime(Time t1, Time t2);
#endif