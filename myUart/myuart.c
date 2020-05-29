/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : myuart.c
* Author             : Tiko Zhong
* Date First Issued  : 11/28/2015
* Description        : This file provides a set of functions needed to manage the
*                      communication using HAL_UARTxxx
********************************************************************************
* History:
* 11/28/2015: V0.2	
* 11/28/2015: V0.1
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
//#include "publicRsrc.h"
#include "myuart.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"
//#include "board.h"
/* Public variables ---------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

static void uartPrintS(UART_RSRC *pRsrc, const char* pStr);
static void uartPrintF(UART_RSRC *pRsrc, const char* FORMAT_ORG, ...);
static void uartSend(UART_RSRC *pRsrc, const u8* pBuff, u16 sz);
static s32 uartPolling(UART_RSRC *pRsrc);

static void uartTxCpltCallback(UART_RSRC *pRsrc);
static void uartRxCpltCallback(UART_RSRC *pRsrc);

/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : uartSrscSetup
* Description    : 
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
DEV_STATUS uartDevSetup(UART_DEV *pDev, UART_HandleTypeDef *huart, u32 baud){
	UART_RSRC *pRsrc = &pDev->Rsrc;
	
	memset(pRsrc, 0, sizeof(UART_RSRC));
	pRsrc->huart = huart;
	pRsrc->baud = baud;	
	RingBuffer_Init(&pRsrc->txRB_Obj, pRsrc->txRingBuff, 1, TX_RINB_BUFF_LEN);
	pRsrc->txRB_Obj.isBusy = 0;	
	RingBuffer_Init(&pRsrc->rxRB_Obj, pRsrc->rxRingBuff, 1, RX_RINB_BUFF_LEN);
	pRsrc->rxRB_Obj.isBusy = 0;	
	
	//operations
	pDev->polling = uartPolling;
	pDev->send = uartSend;
	pDev->printS = uartPrintS;
	pDev->printF = uartPrintF;
	pDev->txCpltCallback = uartTxCpltCallback;
	pDev->rxCpltCallback = uartRxCpltCallback;
	
	//start recive process
	while(HAL_UART_Receive_IT(huart, pRsrc->doubleBuf[0], DOUBLE_BUF_LEN) == HAL_OK){}
	pRsrc->curRcvBuf = pRsrc->doubleBuf[0];
	pRsrc->nxtRcvBuf = pRsrc->doubleBuf[1];
	return DEV_SUCCESS;
}

/*******************************************************************************
* Function Name  : uartStart
* Description    : start a receive task. this is task 
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
static void uartRxCpltCallback(UART_RSRC *pRsrc){
	
}

static void uartTxCpltCallback(UART_RSRC *pRsrc){
//using a polling to instead of txcpltcallback
//	s32 count;
//	u16 min;
//	if(pRsrc->txRB_Obj.isBusy==0){
//		count = RingBuffer_GetCount(&pRsrc->txRB_Obj);
//		if(count>0){
//			min = (pRsrc->txBufLen<count ? pRsrc->txBufLen : count);
//			RingBuffer_PopMult(&pRsrc->txRB_Obj, pRsrc->txBuf, min);
//			HAL_UART_Transmit_IT(pRsrc->huart, pRsrc->txBuf, min);
//		}		
//	}
}
/*******************************************************************************
* Function Name  : uartPolling
* Description    : this function is run in irq, to send out the txRingBuffer.
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
static s32 uartPolling(UART_RSRC *pRsrc){
	s32 count,freeBytes;
	u16 min;

	u8 *pTmp, tmpU8;

	//polling for tx  HAL_UART_GetState
	if(pRsrc->huart->gState == HAL_UART_STATE_READY && pRsrc->txRB_Obj.isBusy==0){
		count = RingBuffer_GetCount(&pRsrc->txRB_Obj);
		if(count>0){
			min = (count <= TXBUF_LEN ? count : TXBUF_LEN);
			RingBuffer_PopMult(&pRsrc->txRB_Obj, pRsrc->txBuf, min);
			HAL_UART_Transmit_IT(pRsrc->huart, pRsrc->txBuf, min);
		}
	}
	//polling for rx
	count = pRsrc->huart->pRxBuffPtr - pRsrc->curRcvBuf;		
	if(count > 0){	//add to ring buffer
		HAL_UART_AbortReceive_IT(pRsrc->huart);
		while(HAL_UART_Receive_IT(pRsrc->huart, pRsrc->nxtRcvBuf, DOUBLE_BUF_LEN) == HAL_OK){}
		//forget the first in
		for(freeBytes = RingBuffer_GetFree(&pRsrc->rxRB_Obj);count > freeBytes;freeBytes++){	
			RingBuffer_Pop(&pRsrc->rxRB_Obj, &tmpU8);
		}
		RingBuffer_InsertMult(&pRsrc->rxRB_Obj, pRsrc->curRcvBuf, count);
		//rset
		pTmp = pRsrc->curRcvBuf;
		pRsrc->curRcvBuf = pRsrc->nxtRcvBuf;
		pRsrc->nxtRcvBuf = pTmp;		
	}

	return count;
}

/*******************************************************************************
* Function Name  : uartJustSend
* Description    : print out a string with spefied uart
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
static void uartSend(UART_RSRC *pRsrc, const u8* pBuff, u16 sz){
//	const u8* ptr = pBuff;
	s32 count;
	u16 rest = sz;
	u16 min;
	const u8* ptr = pBuff;
	
	pRsrc->txRB_Obj.isBusy = 1;	
	//if hardware is free, send more or less
	if(pRsrc->huart->gState == HAL_UART_STATE_READY && RingBuffer_GetCount(&pRsrc->txRB_Obj)==0){
		min = (sz <= TXBUF_LEN ? sz : TXBUF_LEN);
		memcpy(pRsrc->txBuf, pBuff, min);
		HAL_UART_Transmit_IT(pRsrc->huart, pRsrc->txBuf, min);
		rest -= min;
		ptr += min;
	}
	while(rest){	//add to ring buffer
		count = RingBuffer_GetFree(&pRsrc->txRB_Obj);
		if(count > 0){
			if(count-rest >= 0){
				RingBuffer_InsertMult(&pRsrc->txRB_Obj, ptr, rest);
				rest = 0;
			}
			else{
				RingBuffer_InsertMult(&pRsrc->txRB_Obj, ptr, count);
				rest -= count;
				ptr += count;
			}
		}
		//if hardware is free, pop buffer to send
		count = RingBuffer_GetCount(&pRsrc->txRB_Obj);
		if(pRsrc->huart->gState == HAL_UART_STATE_READY && count>0){
			min = (count <= TXBUF_LEN ? count : TXBUF_LEN);
			RingBuffer_PopMult(&pRsrc->txRB_Obj, pRsrc->txBuf, min);
			HAL_UART_Transmit_IT(pRsrc->huart, pRsrc->txBuf, min);
		}
	}
	pRsrc->txRB_Obj.isBusy = 0;
}

//string only
static void uartPrintS(UART_RSRC *pRsrc, const char* FORMAT_ORG){
	uartSend(pRsrc, (u8*)FORMAT_ORG, strlen(FORMAT_ORG));
}

//string with format, not more than 256 byte
void uartPrintF(UART_RSRC *pRsrc, const char* FORMAT_ORG, ...){
	va_list ap;
	s16 bytes;
	char pTmp[256];
	
	va_start(ap, FORMAT_ORG);
	bytes = vsnprintf(pTmp, 256, FORMAT_ORG, ap);
	va_end(ap);
	
	if(bytes>0){
		uartSend(pRsrc, (u8*)pTmp, bytes);
	}
}

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
