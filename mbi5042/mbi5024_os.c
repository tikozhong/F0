/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : mbi5024_os.c
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
#include "mbi5024_os.h"
#include "responseX.h"
#include "stdarg.h"
#include "misc.h"
#include "mbi5024_cmd.h"
/* Public variables ---------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef struct{
	char devName[DEV_NAME_LEN];
	TASK_BONDING_T* p;
	PIN_T *LCK0,*LCK1,*LCK2,*SCK,*DAT;
}TaskArgument_t;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const u8 MBI5024_T[] = {"mbi5024_t"};
/* Private function prototypes -----------------------------------------------*/
static void StartTaskMbi5024(void * argument);
static s8 createTaskMbi5024(osPriority priority, ...);

const TASK_BASE_T TaskInfoMbi5024 = {
	MBI5024_T,				//	const u8* DEV_TYPE;
	MBI5240_LED_HELP,	//	const u8* HELP;
	createTaskMbi5024,			//	s8 (*CreateTask)	(u8 argc, ...);
};
static TaskArgument_t taskArguMbi5024 = {0};
/*******************************************************************************
* Function Name  : StartTask
* Description    : per 4ms timer call back, do inputFetch
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void StartTaskMbi5024(void * argument){
	osEvent  evt;
	Mail_T *pMailRcv, *pMailSnd;
	MBI5024_Dev_T dev;
	osMailQId UartTaskMailId = devInfo[0].MailId;
	TaskArgument_t arg = *(TaskArgument_t*)argument;
	char str[32] = {0};
	//new local mail component	
	osMailQDef(mbi5024TaskMail, 1, Mail_T);
	while(arg.p->MailId == NULL){ arg.p->MailId = osMailCreate(osMailQ(mbi5024TaskMail), NULL);	}
	MBI5024_Setup(&dev, *arg.LCK0, *arg.LCK1, *arg.LCK2, *arg.SCK, *arg.DAT);
	devInfo_RegName(arg.p, dev.rsrc.name, arg.devName);
	strcat(str, "start ");	strcat(str, arg.devName);	strcat(str, "... ok");
	newSend(arg.p->MailId, UartTaskMailId, 0,  "%s", str);

	for(;;){
		evt = osMailGet(arg.p->MailId, 0);        // wait for mail
		if (evt.status == osEventMail){		//if there is mail, 2 operations: reply it, free it
			pMailRcv = (Mail_T*)evt.value.p;
			pMailSnd = newMail(arg.p->MailId, pMailRcv->productor, pMailRcv->traceIndex);   // Allocate memory
			if(pMailSnd != NULL){
				mbi5024Cmd(&dev, &pMailRcv->Content, &pMailSnd->Content);
				osMailPut(pMailRcv->productor, pMailSnd);  // reply Mail
			}
			else	newSend(arg.p->MailId, pMailRcv->productor, 0,  "%s", "+err@MBI5024 osMailCAlloc_fail\r\n");
			osMailFree(arg.p->MailId, pMailRcv);
		}
//		dev.Polling(&dev.rsrc);
		osDelay(64);
	}
}

static s8 createTaskMbi5024(osPriority priority, ...){
	va_list ap;
	u8 i;
	char* pStr;

	memset(&taskArguMbi5024, 0, sizeof(TaskArgument_t));	
	for(i=0;i<MAX_DEV;i++){
		if(devInfo[i].base == NULL){
			memset(&devInfo[i],0,sizeof(TASK_BONDING_T));
			devInfo[i].base = &TaskInfoMbi5024;
			taskArguMbi5024.p = &devInfo[i];
			break;
		}
	}	
	if(taskArguMbi5024.p == NULL)	return -1;
	va_start(ap, priority);	//get last arg addr	
	pStr = va_arg(ap, char*);
	strcpy(taskArguMbi5024.devName, pStr);
	taskArguMbi5024.LCK0 = (PIN_T*)va_arg(ap, PIN_T*);
	taskArguMbi5024.LCK1 = (PIN_T*)va_arg(ap, PIN_T*);
	taskArguMbi5024.LCK2 = (PIN_T*)va_arg(ap, PIN_T*);
	taskArguMbi5024.SCK = (PIN_T*)va_arg(ap, PIN_T*);
	taskArguMbi5024.DAT = (PIN_T*)va_arg(ap, PIN_T*);
	va_end(ap);	
	
	xTaskCreate( StartTaskMbi5024, "led", 100, &taskArguMbi5024, tskIDLE_PRIORITY+(priority-osPriorityIdle), &taskArguMbi5024.p->TaskID);		//stack size in 2bytes	
	while(taskArguMbi5024.p->MailId == NULL)	osDelay(4);
	return 0;
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
