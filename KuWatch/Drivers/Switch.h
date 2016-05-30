#ifndef SWITCH_H
#define SWITCH_H

// Typedefine for Switch Interrupt Handler Function
typedef void (*switch_handler)(void);

void InitSwitch(void);
void InitSwitch1Handler(switch_handler);
void InitSwitch2Handler(switch_handler);
void InitSwitch3Handler(switch_handler);
void Switch3_Polling(void);
#endif