#ifndef KUWATCH_H 
#define KUWATCH_H 

// Setup Boolean Type
typedef int bool;
#define false 0 
#define true 1 

#define DISABLE_INTS {_asm("fclr i");}
#define ENABLE_INTS {_asm("fset i");}

// Define Enumeration
// Main State
typedef enum _STATE {
	INIT,
	DATETIME,
	STOPWATCH,
	ALARM,
	TIMER,
	ALARM_RING,
	TIMER_RING
} State;

// Mode State
typedef enum _MODE_STATE {
	NOTHING,
	SET_YEAR,
	SET_MONTH,
	SET_DAY,
	SET_HOUR,
	SET_MINUTE,
	SET_SECOND,
	SET_ONOFF,
	RUNNING,
	DAYOFWEEK
} Mode;

typedef enum _DAY {
	SUNDAY,
	MONDAY,
	TUESDAY,
	WEDNESDAY,
	THURSDAY,
	FRIDAY,
	SATURDAY	
} Day;

// Prototypes
void KuWatch(void);

void changeState(bool restore, State st);

#endif