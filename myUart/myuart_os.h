/**********************************************************
filename: myuart_os.h
**********************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _MYUART_OS_H
#define _MYUART_OS_H

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
extern TASK_BASE_T TaskInfoUart;
/* output variables for extern function --------------------------------------*/
//s8 newUartTask (u8 argc, ...);
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
