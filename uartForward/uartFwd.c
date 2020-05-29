/**********************************************************
filename: uartFwd.c
**********************************************************/

/************************包含头文件***************************************************/
#include "uartFwd.h"
#include "publicRsrc.h"
#include "myuart_os.h"
/**********************************************************
 Private function
**********************************************************/
static s8 forward(UARTFWD_Rsrc_T* pRsrc, u16 chCfg, const char* CMD, u16 timeOut);
static void polling(UARTFWD_Rsrc_T* pRsrc);
/**********************************************************
 Public function
**********************************************************/
DEV_STATUS UARTFWD_Setup(
	UARTFWD_Dev_T *pDev,
	UART_HandleTypeDef* uart1,
	UART_HandleTypeDef* uart2,
	UART_HandleTypeDef* uart3,
	UART_HandleTypeDef* uart4,
	UART_HandleTypeDef* uart5,
	PIN_T sel
){
	UARTFWD_Rsrc_T* pRsrc = &pDev->rsrc;
	memset(pRsrc, 0, sizeof(UARTFWD_Rsrc_T));
	pRsrc->UART[0] = uart1;
	pRsrc->UART[1] = uart2;
	pRsrc->UART[2] = uart3;
	pRsrc->UART[3] = uart4;
	pRsrc->UART[4] = uart5;
	pRsrc->sel = sel;

	//register operations
	pDev->Forward = forward;
	pDev->Polling = polling;	
	//turn off all channel
	HAL_GPIO_WritePin(pRsrc->sel.GPIOx, pRsrc->sel.GPIO_Pin, GPIO_PIN_RESET);	
	return DEV_SUCCESS;
}

/**********************************************************
 Public function
**********************************************************/
static s8 forward(UARTFWD_Rsrc_T* pRsrc, u16 chCfg, const char* CMD, u16 timeOut){
	u8 i;
	if(pRsrc->squ)	return -1;
	memset(pRsrc->txbuf, 0, 32);
	for(i=0;i<50;i++){
		if(CMD[i] == 0)	break;
		pRsrc->txbuf[i] = CMD[i];
	}
	pRsrc->txbuf[i++] = '\r';
	pRsrc->txbuf[i] = '\n';
	
	pRsrc->txCh = chCfg;
	pRsrc->timeout = timeOut;
	pRsrc->squ = 1;	//start in polling function
	pRsrc->sendTo = 0x03ff;	//send to all ch
	return 0;
}

//in 4ms tick
static void polling(UARTFWD_Rsrc_T* pRsrc){
	u8 i,j;
	u16 rcvCnt;
	switch (pRsrc->squ){
		case 1:
			HAL_GPIO_WritePin(pRsrc->sel.GPIOx, pRsrc->sel.GPIO_Pin, GPIO_PIN_SET);
			for(i=0;i<5;i++){
				if((pRsrc->txCh & (1U<<i)) == 0)	continue;
				HAL_UART_AbortReceive_IT(pRsrc->UART[i]);
				while((pRsrc->UART[i]->gState == HAL_UART_STATE_BUSY_TX) || (pRsrc->UART[i]->gState == HAL_UART_STATE_BUSY_TX_RX) || (pRsrc->UART[i]->gState == HAL_UART_STATE_BUSY));
				HAL_UART_Transmit_IT(pRsrc->UART[i], pRsrc->txbuf, strlen((char*)pRsrc->txbuf)+2);
				memset(&pRsrc->rxbuf[512*i], 0, 512);	
				HAL_UART_Receive_IT(pRsrc->UART[i], &pRsrc->rxbuf[512*i], 512);
			}
			pRsrc->squ = 2;
			break;
		case 2:	//wait for tx complete	
			for(i=0,j=0;i<5;i++){
				if((pRsrc->UART[i]->gState == HAL_UART_STATE_BUSY_TX) || (pRsrc->UART[i]->gState == HAL_UART_STATE_BUSY_TX_RX))
				{	j=1;	break;}
			}
			if(j==0){
				pRsrc->tick = 0;
				pRsrc->squ = 3;
			}
			break;
		case 3:
			if(pRsrc->tick++ >= pRsrc->timeout/4){
				for(i=0;i<5;i++){
					rcvCnt = pRsrc->UART[i]->RxXferSize - pRsrc->UART[i]->RxXferCount;
					HAL_UART_AbortReceive_IT(pRsrc->UART[i]);
					if(rcvCnt>1){
						dev.PrintDecUint(&dev.Rsrc, (const u8*)"<INCUBE.CH", i, 2, (const u8*)">\n");
						dev.PrintStr(&dev.Rsrc, (u8*)&pRsrc->rxbuf[512*i]);
						dev.PrintDecUint(&dev.Rsrc, (const u8*)"</INCUBE.CH", i, 2, (const u8*)">\r\n");
					}			
				}
				HAL_GPIO_WritePin(pRsrc->sel.GPIOx, pRsrc->sel.GPIO_Pin, GPIO_PIN_RESET);
				for(i=0;i<5;i++){
					if((pRsrc->txCh & (1U<<(i+5))) == 0)	continue;
					while((pRsrc->UART[i]->gState == HAL_UART_STATE_BUSY_TX) || (pRsrc->UART[i]->gState == HAL_UART_STATE_BUSY_TX_RX) || (pRsrc->UART[i]->gState == HAL_UART_STATE_BUSY));
					HAL_UART_Transmit_IT(pRsrc->UART[i], pRsrc->txbuf, strlen((char*)pRsrc->txbuf)+2);					
					memset(&pRsrc->rxbuf[512*(i+5)], 0, 512);
					HAL_UART_Receive_IT(pRsrc->UART[i], &pRsrc->rxbuf[512*(i+5)], 512);					
				}
				pRsrc->squ = 4;
				//send out msg
			}
			break;
			
		case 4:	//wait for tx complete	
			for(i=0,j=0;i<5;i++){
				if((pRsrc->UART[i]->gState == HAL_UART_STATE_BUSY_TX) || (pRsrc->UART[i]->gState == HAL_UART_STATE_BUSY_TX_RX))
				{	j=1;	break;	}
			}
			if(j==0){
				pRsrc->tick = 0;
				pRsrc->squ = 5;
			}
			break;
		case 5:
			if(pRsrc->tick++ >= pRsrc->timeout/4){
				for(i=0;i<5;i++){
					rcvCnt = pRsrc->UART[i]->RxXferSize - pRsrc->UART[i]->RxXferCount;
					HAL_UART_AbortReceive_IT(pRsrc->UART[i]);
					if(rcvCnt>1){
						dev.PrintDecUint(&dev.Rsrc, (const u8*)"<INCUBE.CH", (i+5), 2, (const u8*)">\n");
						dev.PrintStr(&dev.Rsrc, (u8*)&pRsrc->rxbuf[512*(i+5)]);
						dev.PrintDecUint(&dev.Rsrc, (const u8*)"</INCUBE.CH", (i+5), 2, (const u8*)">\r\n");
					}
				}
				HAL_GPIO_WritePin(pRsrc->sel.GPIOx, pRsrc->sel.GPIO_Pin, GPIO_PIN_SET);	
				pRsrc->squ = 0;
			}
			break;
		default:
			pRsrc->squ = 0;
		break;
	}
}

/**********************************************************
 == THE END ==
**********************************************************/
