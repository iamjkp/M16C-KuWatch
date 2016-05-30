#ifndef ALARM_H
#define ALARM_H

#define ALARM_CNT 4

typedef struct _ALARM {
	Time time;
	bool isOn;
} Alarm;


void InitAlarm(void);
void AlarmState(void);
void AlarmChecker(void);
void AlarmEvent(bool sw1, bool sw2, bool sw3);

#endif