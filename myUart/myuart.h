/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : myuart.h
* Author             : Tiko Zhong
* Date First Issued  : 11/28/2015
* Description        : This file provides a set of functions needed to manage the
*                      communication using HAL_UARTxxx
********************************************************************************
* History:
* 11/28/2015: V0.1
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _MY_UART_H
#define _MY_UART_H

/* Includes ------------------------------------------------------------------*/
#include "misc.h"
#include "ring_buffer.h"
//#define RXBUF_LEN 300		//
#define TXBUF_LEN 128		//
#define DOUBLE_BUF_LEN 128	//depend on bautrate and polling tick

#define TX_RINB_BUFF_LEN	256
#define RX_RINB_BUFF_LEN	256

/* Exported types ------------------------------------------------------------*/
typedef struct{
	UART_HandleTypeDef *huart;
	char name[DEV_NAME_LEN];
	u32 baud;
	//tx
	RINGBUFF_T txRB_Obj;	
	u8 txRingBuff[TX_RINB_BUFF_LEN];
	u8 txBuf[TXBUF_LEN];
	//rx
	RINGBUFF_T rxRB_Obj;		
	u8 rxRingBuff[RX_RINB_BUFF_LEN];
	u8 doubleBuf[2][DOUBLE_BUF_LEN];	//durying the polling cycle, it can not float over
	u8 *curRcvBuf, *nxtRcvBuf;
}UART_RSRC;

typedef struct{
	UART_RSRC Rsrc;
	void (*send)(UART_RSRC *pRsrc, const u8* pBuff, u16 sz);	
	void (*printS)(UART_RSRC *pRsrc, const char* pBuff);	
	void (*printF)(UART_RSRC *pRsrc, const char* pBuff, ...);
	s32 (*polling)(UART_RSRC *pRsrc);
	void (*txCpltCallback)(UART_RSRC *pRsrc);	
	void (*rxCpltCallback)(UART_RSRC *pRsrc);		
}UART_DEV;
//sizeof(UART_DEV)1428

/* Exported variables --------------------------------------------------------*/
//extern u8 rxBufIndx;
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
DEV_STATUS uartDevSetup(UART_DEV *pDev, UART_HandleTypeDef *huart, u32 baud);

#endif /* _MY_UART_H */

/******************* (C) COPYRIGHT 2015 INCUBECN *****END OF FILE****/
