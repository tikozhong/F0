/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : dictionary_os.c
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
#include "dictionary_os.h"
#include "stdarg.h"
#include "dictionaryCmd.h"
#include "responseX.h"
/* Public variables ---------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
typedef struct{
	char devName[DEV_NAME_LEN];
	TASK_BONDING_T* p;
	AT24C256C_Dev_T* pDev;
}eepromTaskArgument_t;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const char DICT_T[] = {"dict_t"};
/* Private function prototypes -----------------------------------------------*/
static void StartTaskDict(void * argument);
static s8 createTaskDict(osPriority priority, ...);
static void waitWrt (u8);
static void printOut(const char* DEV, const char* FUNC, const char* KEY, const char* VAL);

eepromTaskArgument_t taskArgEeprom = {0};
TASK_BASE_T TaskInfoDict = {
	DICT_T,						//	const u8* DEV_TYPE;
	DICT_HELP,					//	const u8* HELP;
	createTaskDict,		//	s8 (*CreateTask)	(u8 argc, ...);
	NULL,
	0
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
void StartTaskDict(void * argument){
	osEvent  evt;
	Mail_T *pMailRcv, *pMailSnd;
	AT24C256C_Dev_T* pEEPROM = NULL;
	eepromTaskArgument_t* arg = (eepromTaskArgument_t*)argument;
	TASK_BONDING_T* p = arg->p;
	
	//new local mail component
	osMailQDef(eepromTaskMail, 2, Mail_T);
	while(p->MailId == NULL){
		p->MailId = osMailCreate(osMailQ(eepromTaskMail), NULL);
	}
	pEEPROM = arg->pDev;
	pEEPROM->rsrc.osWaitWrt = waitWrt;
	devInfo_RegName(arg->p, dict.rsrc.name, arg->devName);
	print("start %s...ok\n", arg->devName);
	p->base->count++;
	//complete setup info
	dict.rsrc.printOut = printOut;
	
	for(;;){
		evt = osMailGet(p->MailId, 0);  	// wait for mail
		if (evt.status == osEventMail){		//if there is mail, 2 operations: reply it, free it
			xSemaphoreTake( xSemaphoreIIC, portMAX_DELAY);
			pMailRcv = (Mail_T*)evt.value.p;
			pMailSnd = newMail(p->MailId, pMailRcv->productor, pMailRcv->traceIndex);   // Allocate memory
			if(pMailSnd != NULL){
				dictionaryCmd(&dict, &pMailRcv->Content, &pMailSnd->Content);
				osMailPut(pMailRcv->productor, pMailSnd);                         // reply Mail
			}
			else	newSend(p->MailId, pMailRcv->productor, 0,  "%s", "+err@DICT osMailCAlloc_fail\r\n");
			osMailFree(p->MailId, pMailRcv);
			xSemaphoreGive( xSemaphoreIIC);
		}
		osDelay(16);
	}
}

static s8 createTaskDict(osPriority priority, ...){
	va_list ap;
	u8 i;
	char* pStr;

	memset(&taskArgEeprom, 0, sizeof(eepromTaskArgument_t));	
	for(i=0;i<MAX_INSTANCE;i++){
		if(devInfo[i].base == NULL){
			memset(&devInfo[i],0,sizeof(TASK_BONDING_T));
			devInfo[i].base = &TaskInfoDict;
			taskArgEeprom.p = &devInfo[i];
			break;
		}
	}	
	if(taskArgEeprom.p == NULL)	return -2;
	va_start(ap, priority);	//get last arg addr	
	pStr = va_arg(ap, char*);
	strcpy(taskArgEeprom.devName, pStr);
	taskArgEeprom.pDev = va_arg(ap, AT24C256C_Dev_T*);
	va_end(ap);	

	xTaskCreate( StartTaskDict, "dictTask", 300, &taskArgEeprom, tskIDLE_PRIORITY+(priority-osPriorityIdle), &taskArgEeprom.p->TaskID);		//stack size in 2bytes	
	while(taskArgEeprom.p->MailId == NULL)	osDelay(1);
	return 0;
}

static void waitWrt (u8 twr){
	osDelay(twr);
}

static void printOut(const char* DEV, const char* FUNC, const char* KEY, const char* VAL){
	Mail_T *pMailSnd;
	if(taskArgEeprom.p->MailId){
		pMailSnd = newMail(taskArgEeprom.p->MailId, devInfo[0].MailId, 0);   // Allocate memory
		if(pMailSnd != NULL){
			packetReset(&pMailSnd->Content);
			packetSetStyle(&pMailSnd->Content, PAKET_STYLE_CPP);
			pMailSnd->Content.option0 = 2;	//default, packet will as cpp style	
			RESPONSE(&pMailSnd->Content, MSG, DEV, FUNC, "%s%s", KEY, VAL);
			osMailPut(devInfo[0].MailId, pMailSnd); 
		}
		osDelay(10);
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
