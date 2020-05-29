/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : gpio_os.c
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
#include "gpio_os.h"
#include "responseX.h"
#include "stdarg.h"
#include "misc.h"
//#include "gpio_cmd.h"

/* Public variables ---------------------------------------------------------*/
//extern osMailQId UartTaskMailId;
/* Private define ------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
typedef struct{
	char devName[DEV_NAME_LEN];
	TASK_BONDING_T* p;
	u32 capacity;
	u8 pageSz,addr;
	PIN_T scl,sda;
}eepromTaskArgument_t;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const u8 EEPROM_T[] = {"eeprom_t"};
/* Private function prototypes -----------------------------------------------*/
static void StartTaskEeprom(void * argument);
static s8 createTaskEeprom(osPriority priority, ...);
static void waitWrt (u8);
eepromTaskArgument_t taskArgEeprom = {0};
const TASK_BASE_T TaskInfoGpio = {
	EEPROM_T,			//	const u8* DEV_TYPE;
	EEPROM_HELP,	//	const u8* HELP;
	createTaskEeprom,		//	s8 (*CreateTask)	(u8 argc, ...);
};

//delegate for dev
//static void streamOutMsg(const u8* MSG);
/*******************************************************************************
* Function Name  : devtask
* Description    : per 4ms timer call back, do inputFetch
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void StartTaskEeprom(void * argument){
	osEvent  evt;
	Mail_T *pMailRcv, *pMailSnd;
	AT24C256C_Dev_T EEPROM = {0};
	osMailQId UartTaskMailId = devInfo[0].MailId;
	char str[32]={0};
	eepromTaskArgument_t arg = *(eepromTaskArgument_t*)argument;
	
	//new local mail component
	osMailQDef(eepromTaskMail, 1, Mail_T);
	while(arg.p->MailId == NULL){
		arg.p->MailId = osMailCreate(osMailQ(eepromTaskMail), NULL);
	}
	//wait a mail to setup a device
	AT24C256C_Setup(&EEPROM, arg.addr, arg.capacity, arg.pageSz, arg.scl, arg.sda);
	EEPROM.rsrc.osWaitWrt = waitWrt;		
	devInfo_RegName(arg.p, EEPROM.rsrc.name, arg.devName);
	strcat(str, "start ");	strcat(str, arg.devName);	strcat(str, "... ok");
	newSend(arg.p->MailId, UartTaskMailId, 0,  "%s", str);	

	for(;;){
		evt = osMailGet(arg.p->MailId, 0);        // wait for mail
		if (evt.status == osEventMail){		//if there is mail, 2 operations: reply it, free it
			pMailRcv = (Mail_T*)evt.value.p;
			pMailSnd = newMail(arg.p->MailId, pMailRcv->productor, pMailRcv->traceIndex);   // Allocate memory
			if(pMailSnd != NULL){
				eepromCmd(&EEPROM, &pMailRcv->Content, &pMailSnd->Content);
				osMailPut(pMailRcv->productor, pMailSnd);                         // reply Mail
			}
			else	newSend(arg.p->MailId, pMailRcv->productor, 0,  "%s", "+err@EEPROM osMailCAlloc_fail\r\n");
			osMailFree(arg.p->MailId, pMailRcv);
		}
		osDelay(16);
	}
}

static s8 createTaskEeprom(osPriority priority, ...){
	va_list ap;
	u8 i;
	char* pStr;

	memset(&taskArgEeprom, 0, sizeof(eepromTaskArgument_t));	
	for(i=0;i<MAX_DEV;i++){
		if(devInfo[i].base == NULL){
			memset(&devInfo[i],0,sizeof(TASK_BONDING_T));
			devInfo[i].base = &TaskInfoEeprom;
			taskArgEeprom.p = &devInfo[i];				
			break;
		}
	}	
	if(taskArgEeprom.p == NULL)	return -2;
	va_start(ap, priority);	//get last arg addr	
	pStr = va_arg(ap, char*);
	strcpy(taskArgEeprom.devName, pStr);
	taskArgEeprom.capacity = va_arg(ap, u32);
	taskArgEeprom.pageSz = va_arg(ap, u32);
	taskArgEeprom.scl = *(PIN_T*)va_arg(ap, PIN_T*);
	taskArgEeprom.sda = *(PIN_T*)va_arg(ap, PIN_T*);
	taskArgEeprom.addr = va_arg(ap, u32);
	va_end(ap);	

	xTaskCreate( StartTaskEeprom, "eepromTask", 300, &taskArgEeprom, tskIDLE_PRIORITY+(priority-osPriorityIdle), &taskArgEeprom.p->TaskID);		//stack size in 2bytes	
	while(taskArgEeprom.p->MailId == NULL)	osDelay(1);
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

static void waitWrt (u8 twr){
	osDelay(twr);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
