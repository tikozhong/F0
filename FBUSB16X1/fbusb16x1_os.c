/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : fbusb16x1_os.c
* Author             : Tiko Zhong
* Date First Issued  : 07/16/2018
* Description        : 
*                      
********************************************************************************
* History:
* 07/16/2018: V0.0	
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "publicRsrc.h"
#include "fbusb16x1_os.h"
#include "responseX.h"
#include "stdarg.h"
#include "misc.h"
#include "fbusb16x1_cmd.h"
/* Public variables ---------------------------------------------------------*/
//extern osMailQId UartTaskMailId;
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef struct{
	char devName[DEV_NAME_LEN];
	TASK_BONDING_T* p;
	DEV_PORT* port;
}TaskArgument_t;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const u8 FBUSB16X1_T[] = {"fbusb16x1_t"};
/* Private function prototypes -----------------------------------------------*/
static void StartTaskFbusb16x1(void * argument);
static s8 createTaskFbusb16x1(osPriority priority, ...);

const TASK_BASE_T TaskInfoFbusb16x1 = {
	FBUSB16X1_T,		//	const u8* DEV_TYPE;
	FBUSB16X1_HELP,	//	const u8* HELP;
	createTaskFbusb16x1,			//	s8 (*CreateTask)	(u8 argc, ...);
};
static TaskArgument_t taskArgument = {0};
/*******************************************************************************
* Function Name  : StartTask
* Description    : per 4ms timer call back, do inputFetch
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void StartTaskFbusb16x1(void * argument){
	osEvent  evt;
	Mail_T *pMailRcv, *pMailSnd;
	FBUSB16X1_Dev_T dev;
	osMailQId UartTaskMailId = devInfo[0].MailId;
	TaskArgument_t arg = *(TaskArgument_t*)argument;
	char str[32] = {0};
	//new local mail component	
	osMailQDef(fbusb16x1TaskMail, 1, Mail_T);
	while(arg.p->MailId == NULL){ 
		arg.p->MailId = osMailCreate(osMailQ(fbusb16x1TaskMail), NULL);
	}	
	FBUSB16X1_Setup(&dev, arg.port);
	devInfo_RegName(arg.p, dev.rsrc.name, arg.devName);
	strcat(str, "start ");	strcat(str, arg.devName);	strcat(str, "... ok");
	newSend(arg.p->MailId, UartTaskMailId, 0,  "%s", str);	
	
	for(;;){
		evt = osMailGet(arg.p->MailId, 0);        // wait for mail
		if (evt.status == osEventMail){		//if there is mail, 2 operations: reply it, free it
			pMailRcv = (Mail_T*)evt.value.p;
			pMailSnd = newMail(arg.p->MailId, pMailRcv->productor, pMailRcv->traceIndex);   // Allocate memory
			if(pMailSnd != NULL){
				fbusb16x1Cmd(&dev, &pMailRcv->Content, &pMailSnd->Content);
				osMailPut(pMailRcv->productor, pMailSnd);  // reply Mail
			}
			else	newSend(arg.p->MailId, pMailRcv->productor, 0,  "%s", "+err@FBUSB16X1 osMailCAlloc_fail\r\n");
			osMailFree(arg.p->MailId, pMailRcv);
		}
//		dev.Polling(&dev.rsrc);
		osDelay(64);
	}
}

static s8 createTaskFbusb16x1(osPriority priority, ...){
	va_list ap;
	u8 i;
	char* pStr;

	memset(&taskArgument, 0, sizeof(TaskArgument_t));	
	for(i=0;i<MAX_DEV;i++){
		if(devInfo[i].base == NULL){
			memset(&devInfo[i],0,sizeof(TASK_BONDING_T));
			devInfo[i].base = &TaskInfoFbusb16x1;
			taskArgument.p = &devInfo[i];				
			break;
		}
	}	
	if(taskArgument.p == NULL)	return -1;
	//createTask(u8 argc, priority, "devName", portIndx)
	va_start(ap, priority);	//get last arg addr	
	pStr = va_arg(ap, char*);
	strcpy(taskArgument.devName, pStr);
	taskArgument.port = (DEV_PORT*)va_arg(ap, DEV_PORT*);
	va_end(ap);	
	
	xTaskCreate( StartTaskFbusb16x1, "fbusb16x1Task", 250, &taskArgument, tskIDLE_PRIORITY+(priority-osPriorityIdle), &taskArgument.p->TaskID);		//stack size in 2bytes	
	while(taskArgument.p->MailId == NULL)	osDelay(1);
	return 0;
}

//static u8 EEPROMTaskCmd(osMailQId sndr, osMailQId rcvr, const u8* S, STR_PIECE_T* Content, PARAM_DEV_T* e)
//static u8 eepromTaskCmd(Mail_T* rcvr, PAKET_T *pPacket){
//	u8 *rtnStr, rtnStrSz;
//	u8* S;
//	CompiLer_Dev_T eDev;
//	CompiLer_Rsrc_T* e;
//	
//	rtnStr = (u8*)pPacket->addr[0];
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
//				AT24C256C_Setup(pDev, (const u8*)e->params[1], PAGESZ_AT24256C, PAGESZ_AT24256C, EEPROM_SCL, EEPROM_SDA);
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
