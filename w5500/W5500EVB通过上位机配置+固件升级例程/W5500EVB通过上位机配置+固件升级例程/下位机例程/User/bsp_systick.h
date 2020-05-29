#ifndef _BSP_SYSTICK_H
#define _BSP_SYSTICK_H
#include"stdio.h"
#include "types.h"
#include "stm32f10x.h"

void Systick_Init (uint8 SYSCLK);
void delay_s( uint32 time_s );
void delay_us(uint32 time_us);
void delay_ms(uint32 time_ms);

#endif
