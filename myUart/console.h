/**********************************************************
filename: console.h
**********************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef CONSOLE_H
#define CONSOLE_H

#include "misc.h"
#include "myUart.h"
#include "packet.h"
#define FRINED_CNT 16

//extern UART_DEV dev;
//void printPacketDetail(PAKET_T* p);
void print(const char* FORMAT_ORG, ...);
//extern osMailQId UartTaskMailId;
//extern osThreadId UartTaskHandle;
//extern const u8 UART_T[];
//extern const u8 SYS_HELP[];

/* output variables for extern function --------------------------------------*/
//s8 newUartTask (u8 argc, ...);
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
