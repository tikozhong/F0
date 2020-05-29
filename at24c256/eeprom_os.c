/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : eeprom_os.c
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
#include "eeprom_os.h"
#include "stdarg.h"
#include "publicRsrc.h"
/* Public variables ---------------------------------------------------------*/
//extern osMailQId UartTaskMailId;
/* Private define ------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
typedef struct{
	char devName[DEV_NAME_LEN];
	TASK_BONDING_T* p;
	AT24C256C_Dev_T* pDev;
}eepromTaskArgument_t;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const u8 EEPROM_T[] = {"eeprom_t"};
/* Private function prototypes -----------------------------------------------*/
static void StartTaskEeprom(void * argument);
static s8 createTaskEeprom(osPriority priority, ...);
static void waitWrt (u8);
eepromTaskArgument_t taskArgEeprom = {0};
const TASK_BASE_T TaskInfoEeprom = {
	EEPROM_T,						//	const u8* DEV_TYPE;
	NULL,								//	const u8* HELP;
	createTaskEeprom,		//	s8 (*CreateTask)	(u8 argc, ...);
};

//static s8 tmr5ms = 0;
static s8 eepromIOWrite(u16 addr, const u8 *pDat, u16 nBytes);
static s8 eepromIORead(u16 addr, u8 *pDat, u16 nBytes);	

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
	AT24C256C_Dev_T* EEPROM = NULL;
	osMailQId UartTaskMailId = devInfo[0].MailId;
	char str[32]={0};
	eepromTaskArgument_t arg = *(eepromTaskArgument_t*)argument;

	//new local mail component
	osMailQDef(eepromTaskMail, 1, Mail_T);
	while(arg.p->MailId == NULL){
		arg.p->MailId = osMailCreate(osMailQ(eepromTaskMail), NULL);
	}
	EEPROM = arg.pDev;
	EEPROM->rsrc.osWaitWrt = waitWrt;
	devInfo_RegName(arg.p, EEPROM->rsrc.name, arg.devName);
	strcat(str, "start ");	strcat(str, arg.devName);	strcat(str, "... ok");
	newSend(arg.p->MailId, UartTaskMailId, 0,  "%s", str);
	
	for(;;){
		evt = osMailGet(arg.p->MailId, 0);        // wait for mail
		if (evt.status == osEventMail){		//if there is mail, 2 operations: reply it, free it
			xSemaphoreTake( xSemaphoreIIC, portMAX_DELAY);
			pMailRcv = (Mail_T*)evt.value.p;
			pMailSnd = newMail(arg.p->MailId, pMailRcv->productor, pMailRcv->traceIndex);   // Allocate memory
			if(pMailSnd != NULL){
				dictionaryCmd(&dict, &pMailRcv->Content, &pMailSnd->Content);
				osMailPut(pMailRcv->productor, pMailSnd);                         // reply Mail
			}
			else	newSend(arg.p->MailId, pMailRcv->productor, 0,  "%s", "+err@DICT osMailCAlloc_fail\r\n");
			osMailFree(arg.p->MailId, pMailRcv);
			xSemaphoreGive( xSemaphoreIIC);
		}
		osDelay(1);
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
	taskArgEeprom.pDev = va_arg(ap, AT24C256C_Dev_T*);
	va_end(ap);	

	xTaskCreate( StartTaskEeprom, "eepromTask", 300, &taskArgEeprom, tskIDLE_PRIORITY+(priority-osPriorityIdle), &taskArgEeprom.p->TaskID);		//stack size in 2bytes	
	while(taskArgEeprom.p->MailId == NULL)	osDelay(1);
	return 0;
}

static void waitWrt (u8 twr){
	osDelay(twr);
}

static s8 eepromIOWrite(u16 addr, const u8 *pDat, u16 nBytes){
	EEPROM.Write(&EEPROM.rsrc, addr, pDat, nBytes);
	return 0;
}

static s8 eepromIORead(u16 addr, u8 *pDat, u16 nBytes){
	EEPROM.Read(&EEPROM.rsrc, addr, pDat, nBytes);
	return 0;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
