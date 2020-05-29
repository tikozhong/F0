/**********************************************************
filename: uartFwd.h
**********************************************************/
#ifndef _UART_FWD_H_
#define _UART_FWD_H_

#include "misc.h"
/*****************************************************************************
 @ typedefs
****************************************************************************/
typedef struct{
	char name[DEV_NAME_LEN];
	//config
	UART_HandleTypeDef* UART[5];
	PIN_T sel;
	//VAR
	u16 tick,timeout,txCh;
	u8 squ;
	u16 sendTo;
	u8 rxbuf[512*10];
	u8 txbuf[32];
}UARTFWD_Rsrc_T;

typedef struct{
	//resource
	UARTFWD_Rsrc_T rsrc;
	//operation
	s8 (*Forward)(UARTFWD_Rsrc_T* pRsrc, u16 chCfg, const char* CMD, u16 timeOut);
	void (*Polling)(UARTFWD_Rsrc_T* pRsrc);
}UARTFWD_Dev_T;

DEV_STATUS UARTFWD_Setup(
	UARTFWD_Dev_T *pDev,
	UART_HandleTypeDef* uart1,
	UART_HandleTypeDef* uart2,
	UART_HandleTypeDef* uart3,
	UART_HandleTypeDef* uart4,
	UART_HandleTypeDef* uart5,
	PIN_T sel
);
#endif
