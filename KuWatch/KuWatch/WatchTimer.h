#ifndef WATCHTIMER_H
#define WATCHTIMER_H

typedef struct _TIMER {
	Time time;
	Mode mode;
} Timer;

void InitWatchTimer();
void WatchTimerState();
void WatchTimerCalc();
void WatchTimerEvent(bool sw1, bool sw2, bool sw3);

#endif