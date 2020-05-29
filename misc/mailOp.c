/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : mailOp.c
* Author             : Tiko Zhong
* Date First Issued  : 07/03/201
* Description        : 
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "myString.h"
#include "mailOp.h"
#include "stdarg.h"
#include "packet.h"
/* Private define ------------------------------------------------------------*/
#define DEV_MAX 64
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
osStatus newSend(osMailQId sndr, osMailQId rcvr, u32 sub, const char* FORMAT, ...){
	va_list ap;	
	u32 tmpU;	
	u8 i;
	char *pStr;
	PAKET_T* pPacket=NULL;
	Mail_T *pMailSnd = osMailCAlloc(rcvr, osWaitForever);       // Allocate memory	
	if(pMailSnd == NULL)	return os_status_reserved;

	pMailSnd->traceIndex = sub;
	pMailSnd->productor = sndr;
	pPacket = &pMailSnd->Content;
	packetReset(pPacket);
	
	va_start(ap, FORMAT);	//get last arg addr
	//"%d%s%f"
  for(i=0;1;i++){
		if (FORMAT[i] == 0)	break;
		else if (FORMAT[i] == '%'){
			i++;
			if(FORMAT[i] == 'd' || FORMAT[i] == 'u' || FORMAT[i] == 'x'){
				tmpU = va_arg(ap, u32);
				packetAddTo(pPacket, "%d", tmpU);
				continue;
			}
			else if(FORMAT[i] == 's'){
				pStr = va_arg(ap, char*);
				packetAddTo(pPacket, "%s", pStr);
				continue;
			}
			else if(FORMAT[i] == 'p'){		//only add a pointer without copy any data
				pStr = va_arg(ap, char*);
				packetAddTo(pPacket, "%p", pStr);
				continue;
			}
			else if(FORMAT[i] == 'f'){
			}
			else{		break;	}
		}
		else if(FORMAT[i] == ' ' || FORMAT[i] == '\t')	continue;
		else {		break;	}
	}
	va_end(ap);
	
	return osMailPut(rcvr, pMailSnd); 
}

Mail_T* newMail(osMailQId sndr, osMailQId rcvr, u32 sub){
	Mail_T *pMailSnd = osMailCAlloc(rcvr, osWaitForever);       // Allocate memory	
	if(pMailSnd == NULL)	return NULL;
	pMailSnd->traceIndex = sub;
	pMailSnd->productor = sndr;
	packetReset(&pMailSnd->Content);
	return pMailSnd;
}

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
