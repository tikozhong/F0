#ifndef _TIMER_H_
#define _TIMER_H_
#include "stm32f10x.h"
#include "Types.h"

extern uint8 sec;
extern uint8 read_PHY_flag;
extern uint16 plus_wait_count;
extern uint16 nagle_count;
void Timer_Configuration(void);
void Timer2_ISR(void);
#endif 

