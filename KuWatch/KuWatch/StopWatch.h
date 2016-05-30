#ifndef STOPWATCH_H
#define STOPWATCH_H

typedef struct _STOPWATCH {
	Time time;
	Time lap;
	Mode mode;
} StopWatch;


void InitStopWatch();
void StopWatchState();
void StopWatchCalc();
void StopWatchEvent(bool sw1, bool sw2, bool sw3);

#endif