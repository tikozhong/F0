/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : uartdev.c
* Author             : Tiko Zhong
* Date First Issued  : 04/20/2020
* Description        : This file provides a set of functions needed to manage the
*                      communication using HAL_UARTxxx
********************************************************************************
* History:
* 04/20/2020: V0.1
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
//#include "publicRsrc.h"
#include "uartDev.h"
#include "stdarg.h"
#include "stdio.h"
#include "string.h"
/* Public variables ---------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

static void uartRxReset(UartRsrc_t *pRsrc);
static u8 uartRxMonitor(UartRsrc_t *pRsrc);
static u8 uartRxFetchLine(UartRsrc_t *pRsrc, char* line, u16 len);
static void uartTxMonitor(UartRsrc_t *pRsrc);
static u16 uartTxSend(UartRsrc_t *pRsrc, const u8* BUF, u16 len);

/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : uartSrscSetup
* Description    : 
* Input          : 
* Output         : None
* Return         : None
*******************************************************************************/
void setupUartDev(
	UartDev_t *pDev, 
	UART_HandleTypeDef* huart,
	u8* p,	/*	all memmory	*/
	u16	rxPoolLen,
	u8 rxBufLen,
	u16 txPoolLen,
	u8 txBufLen
){
	UartRsrc_t *pRsrc = &pDev->rsrc;
	pRsrc->huart = huart;
	
	memset(p,0,(rxPoolLen+2*rxBufLen+txPoolLen+txBufLen));
	pRsrc->rxPool = p;
	pRsrc->rxBuf0 = p+rxPoolLen;
	pRsrc->rxBuf1 = p+rxPoolLen+rxBufLen;
	pRsrc->txPool = p+rxPoolLen+2*rxBufLen;
	pRsrc->txBuf = p+rxPoolLen+2*rxBufLen+txPoolLen;
	
	pRsrc->rxCurBuf = pRsrc->rxBuf0;
	pRsrc->rxNxtBuf = pRsrc->rxBuf1;
	pRsrc->rxPoolIndx = 0;
	pRsrc->rxBufLen = rxBufLen;
	pRsrc->rxPoolLen = rxPoolLen-1;
	
	pRsrc->txPoolIndx = 0;
	pRsrc->txBufLen = txBufLen;
	pRsrc->txPoolLen = txPoolLen;
	
	//register op
	pDev->RxReset = uartRxReset;
	pDev->RxMonitor = uartRxMonitor;
	pDev->RxFetchLine = uartRxFetchLine;
	pDev->TxMonitor = uartTxMonitor;
	pDev->TxSend = uartTxSend;
	
	//start to receive
	while(HAL_UART_Receive_IT(huart, pRsrc->rxCurBuf, pRsrc->rxBufLen) == HAL_OK){}	

}

static u16 uartTxSend(UartRsrc_t *pRsrc, const u8* BUF, u16 len){
	u16 min,tmp;
	if(BUF == NULL || len==0)	return 0;
	if(pRsrc->txPoolIndx >= pRsrc->txPoolLen)	return 0;
	tmp = pRsrc->txPoolLen - pRsrc->txPoolIndx;
	min = (len<=tmp?len:tmp);
	memcpy(&pRsrc->txPool[pRsrc->txPoolIndx],BUF,min);
	pRsrc->txPoolIndx += min;
	return min;
}

static void uartTxMonitor(UartRsrc_t *pRsrc){
	u16 tmp;
	if(pRsrc->txPoolIndx == 0)	return;
	if(pRsrc->huart->gState == HAL_UART_STATE_BUSY_TX)	return;

	if(pRsrc->txPoolIndx <= pRsrc->txBufLen){	/* can load in one time */
		memcpy(pRsrc->txBuf, pRsrc->txPool, pRsrc->txPoolIndx);
		HAL_UART_Transmit_IT(pRsrc->huart, pRsrc->txBuf, pRsrc->txPoolIndx);
		pRsrc->txPoolIndx = 0;				
	}
	else{
		memcpy(pRsrc->txBuf, pRsrc->txPool, pRsrc->txBufLen);
		HAL_UART_Transmit_IT(pRsrc->huart, pRsrc->txBuf, pRsrc->txBufLen);
		pRsrc->txPoolIndx -= pRsrc->txBufLen;	
	}
}

static void uartRxReset(UartRsrc_t *pRsrc){
	u16 count,tmp;
	u8* pTmp;
	UART_HandleTypeDef *huart = pRsrc->huart;
	
	CLEAR_BIT(huart->Instance->CR1, (USART_CR1_RXNEIE));
	
	memset(pRsrc->rxPool,0,pRsrc->rxPoolLen);
	memset(pRsrc->rxBuf0,0,pRsrc->rxBufLen);
	memset(pRsrc->rxBuf1,0,pRsrc->rxBufLen);
	pRsrc->rxCurBuf = pRsrc->rxBuf0;
	pRsrc->rxNxtBuf = pRsrc->rxBuf1;
	pRsrc->rxPoolIndx = 0;
  
	//restart uart
	huart->pRxBuffPtr  = pRsrc->rxCurBuf;
	huart->RxXferCount = pRsrc->rxBufLen;
	
	SET_BIT(huart->Instance->CR1, USART_CR1_RXNEIE);

}

static u8 uartRxMonitor(UartRsrc_t *pRsrc){
	u16 count,tmp;
	u8* pTmp;
	UART_HandleTypeDef *huart = pRsrc->huart;
	
  CLEAR_BIT(huart->Instance->CR1, (USART_CR1_RXNEIE));
	count = huart->RxXferCount;
	//restart uart
	huart->pRxBuffPtr  = pRsrc->rxNxtBuf;
	huart->RxXferCount = pRsrc->rxBufLen;
	SET_BIT(huart->Instance->CR1, USART_CR1_RXNEIE);
	
	count = huart->RxXferSize - count;
	if(count == 0){	/* nothing copy buffer to pool */}
	else{
		//copy pCurRcvUi to buffer
		if(pRsrc->rxPoolLen < pRsrc->rxPoolIndx)		pRsrc->rxPoolIndx = pRsrc->rxPoolLen;
		tmp = pRsrc->rxPoolLen - pRsrc->rxPoolIndx;	//empty memory to copy to
		if(count > tmp){
			tmp = count - tmp;	//need to loop count
			memcpy(pRsrc->rxPool, &pRsrc->rxPool[tmp], pRsrc->rxPoolLen - tmp);
			pRsrc->rxPoolIndx -= tmp;
			memset(&pRsrc->rxPool[pRsrc->rxPoolIndx],0,pRsrc->rxPoolLen - pRsrc->rxPoolIndx);
		}
		memcpy(&pRsrc->rxPool[pRsrc->rxPoolIndx], pRsrc->rxCurBuf, count);
		pRsrc->rxPoolIndx += count;
	}

	//swap pool, get ready for next buffer
	pTmp = pRsrc->rxCurBuf;
	pRsrc->rxCurBuf = pRsrc->rxNxtBuf;
	pRsrc->rxNxtBuf = pTmp;
	memset(pRsrc->rxNxtBuf,0,pRsrc->rxBufLen);
	return (count);
}

static u8 uartRxFetchLine(UartRsrc_t *pRsrc, char* line, u16 len){
	u8 ret = 0;
	char *p;
	u16 i,j,tmp;
	
	if(pRsrc->rxPoolIndx){
		for(i=0;i<pRsrc->rxPoolIndx;i++){
			p = strstr((char*)&pRsrc->rxPool[i], "\r\n");
			if(p){

				j = p-(char*)&pRsrc->rxPool[i]+2;
				tmp = (j>len?len:j);
				memcpy(line,&pRsrc->rxPool[i],tmp);
				//loop
				tmp = p+2-(char*)pRsrc->rxPool;	//need to loop count
				memcpy(pRsrc->rxPool, &pRsrc->rxPool[tmp], pRsrc->rxPoolLen - tmp);
				pRsrc->rxPoolIndx -= tmp;
				memset(&pRsrc->rxPool[pRsrc->rxPoolIndx],0,pRsrc->rxPoolLen - pRsrc->rxPoolIndx);
				ret = 1;
				break;
			}
		}
	}
	return ret;
}

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
