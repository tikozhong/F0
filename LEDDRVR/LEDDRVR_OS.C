/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : leddrvr_os.c
* Author             : Tiko Zhong
* Date First Issued  : 09/04/2017
* Description        : 
*                      
********************************************************************************
* History:
* 09/04/2017: V0.0	
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "publicRsrc.h"
#include "leddrvr_os.h"
#include "responseX.h"

#include "misc.h"
#include "stdarg.h"
#include "packet.h"
#include "compiler.h"

//#include "leddrvr_cmd.h"
#include "leddrvr.h"
/* Public variables ---------------------------------------------------------*/
extern osMailQId UartTaskMailId;
//AT24C256C_Dev_T EEPROM;
/* Private define ------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const u8 LEDDRVR_T[] = {"leddrvr_t"};
osThreadId leddrvrTaskHandle = NULL;
osMailQId leddrvrTaskMailId = NULL;
LedDrvrDev_t led;
/* Private function prototypes -----------------------------------------------*/
//static u8 leddrvrTaskCmd(Mail_T* rcvr, Mail_T *sndr);
//static AT24C256C_Dev_T* pDev = NULL;

//delegate for dev
//static void streamOutMsg(const u8* MSG);
/*******************************************************************************
* Function Name  : newLeddrvrTask
* Description    : per 4ms timer call back, do inputFetch
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void StartLeddrvrTask(void const * argument){
	osMailQId owner;
	osEvent  evt;
	Mail_T *pMailRcv, *pMailSnd;
	
	osMessageQId productor;
	u32 traceIndex;	

	//new local mail component
	osMailQDef(leddrvrTaskMail, 1, Mail_T);
	owner = osMailCreate(osMailQ(leddrvrTaskMail), NULL);
	//wait outside env ready
	while(UartTaskMailId == NULL || owner == NULL)		{ osDelay(4);}	//wait uart ready		
	//ready go
	ledDrvrDevSetup(&led,	(const u8*)"led", LCK0, SCK, MOSI, LCK1, SCK, MOSI, SCL, SDA);
	led.SetAllBrightness(&led.rsrc, 0);
	leddrvrTaskMailId = owner;
	//wait a mail to setup a device
	taskTabAddDevName(LEDDRVR_T, led.rsrc.name, (const u8*)"led");
	send(owner, UartTaskMailId, 0x00, "%s", "start eeprom task.. ok");

	for(;;){
		evt = osMailGet(owner, 0);        // wait for mail
		if (evt.status == osEventMail){		//if there is mail, 2 operations: reply it, free it
			pMailRcv = (Mail_T*)evt.value.p;
			productor = pMailRcv->productor;
			traceIndex = pMailRcv->traceIndex;
			pMailSnd = (Mail_T*)osMailCAlloc(UartTaskMailId, osWaitForever);       // Allocate memory
			if(pMailSnd != NULL){
				ledCmd(&led, pMailRcv->Content.addr[0], pMailSnd->Content.addr[0], 128);
//				ledDrvrCmd(&led, &pMailRcv->Content, &pMailSnd->Content);
				osMailFree(owner, pMailRcv);
				pMailSnd->productor = owner;
				pMailSnd->traceIndex = traceIndex;
				osMailPut(UartTaskMailId, pMailSnd);                         // reply Mail
			}
			else{
				send(owner, UartTaskMailId, 0x00, "%s", "osMailCAlloc_fail\r\n");
				osMailFree(owner, pMailRcv);
			}
		}
		osDelay(4);
	}
}

s8 newLeddrvrTask(u8 argc, ...){
	osThreadDef(leddrvrTaskDef, StartLeddrvrTask, osPriorityNormal, 0, 256);
	if(leddrvrTaskHandle == NULL){
		leddrvrTaskHandle = osThreadCreate(osThread(leddrvrTaskDef), NULL);
		if(leddrvrTaskHandle != NULL)	return 1;
	}
	return 0;
}

//static u8 EEPROMTaskCmd(osMailQId sndr, osMailQId rcvr, const u8* S, STR_PIECE_T* Content, PARAM_DEV_T* e)
//static u8 leddrvrTaskCmd(Mail_T* rcvr, Mail_T *sndr){
//	u8 *rtnStr, rtnStrSz;
//	u8* S;
//	CompiLer_Dev_T eDev;
//	CompiLer_Rsrc_T* e;
//	
//	rtnStr = (u8*)sndr->Content.addr[0];
//	rtnStrSz = PACKE_POOL_SZ;
//	S = (u8*)rcvr->Content.addr[0];	
//	
//	compilerSetup(&eDev);
//	e = &eDev.rsrc;
//	if(eDev.comply(e, S, NULL, 0)<0)	return 0;
//	if(eDev.isMatchDevName(e, (const u8*)"sys") != PASS)	return 0;

//	//sys.newdev("dev_t", "devname");
//	if(eDev.isMatchFuncName(e, (const u8*)"newdev") == PASS){	//match function name
//		if(eDev.isMatchFormat(e,STR,STR) == PASS){	//match format too
//			if(isSameStr(e->params[0], EEPROM_T) == 0)	return 0;	//reply nothing
//			else if(isDevNameAvalid(e->params[0], e->params[1]) == 0)
//				RESPONSE_X(ERR,rtnStr, rtnStrSz, (const u8*)"sys", (const u8*)"newdev", "%s",(const u8*)"DEV_NAME");
//			else{	
//				if(pDev != NULL)	RESPONSE_X(ERR, rtnStr, rtnStrSz, (const u8*)"sys", (const u8*)"newdev", "%s",(const u8*)"EEPROM_ACTIVE");				
//				pDev = &EEPROM;				
//				AT24C256C_Setup(pDev, (const u8*)e->params[1], PAGESZ_AT24256C, PAGESZ_AT24256C, SCL, SDA);
//				taskTabAddDevName(EEPROM_T, pDev->rsrc.name,e->params[1]);
//				RESPONSE_X(OK,rtnStr, rtnStrSz, NULL, NULL, (const char*)S);
//			}
//			return 1;
//		}
//	}
//	
//	return 0;	
//}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
