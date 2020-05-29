/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : FBLED5050X3_os.c
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
#include "FBLED5050X3_os.h"
#include "responseX.h"
#include "stdarg.h"
#include "misc.h"
#include "FBLED5050X3_cmd.h"
/* Public variables ---------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef struct{
	char devName[DEV_NAME_LEN];
	TASK_BONDING_T* p;
	const DEV_PORT* PORT;
}TaskArgument_t;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const u8 FBLED5050X3_T[] = {"FBLED5050X3_t"};
/* Private function prototypes -----------------------------------------------*/
static void StartTaskFbled5050x3(void * argument);
static s8 createTaskFbled5050x3(osPriority priority, ...);

const TASK_BASE_T TaskInfoFbled5050x3 = {
	FBLED5050X3_T,				//	const u8* DEV_TYPE;
	FBLED5050X3_LED_HELP,	//	const u8* HELP;
	createTaskFbled5050x3,			//	s8 (*CreateTask)	(u8 argc, ...);
};
static TaskArgument_t taskArguFbled5050x3 = {0};
/*******************************************************************************
* Function Name  : StartTask
* Description    : per 4ms timer call back, do inputFetch
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void StartTaskFbled5050x3(void * argument){
	osEvent  evt;
	Mail_T *pMailRcv, *pMailSnd;
	FBLED5050X3_Dev_T dev;
	osMailQId UartTaskMailId = devInfo[0].MailId;
	TaskArgument_t arg = *(TaskArgument_t*)argument;
	char str[32] = {0};
	//new local mail component	
	osMailQDef(Fbled5050x3TaskMail, 1, Mail_T);
	while(arg.p->MailId == NULL){ arg.p->MailId = osMailCreate(osMailQ(Fbled5050x3TaskMail), NULL);	}
	FBLED5050X3_Setup(&dev, arg.PORT);
	devInfo_RegName(arg.p, dev.rsrc.name, arg.devName);
	strcat(str, "start ");	strcat(str, arg.devName);	strcat(str, "... ok");
	newSend(arg.p->MailId, UartTaskMailId, 0,  "%s", str);

	for(;;){
		evt = osMailGet(arg.p->MailId, 0);        // wait for mail
		if (evt.status == osEventMail){		//if there is mail, 2 operations: reply it, free it
			pMailRcv = (Mail_T*)evt.value.p;
			pMailSnd = newMail(arg.p->MailId, pMailRcv->productor, pMailRcv->traceIndex);   // Allocate memory
			if(pMailSnd != NULL){
				FBLED5050X3_Cmd(&dev, &pMailRcv->Content, &pMailSnd->Content);
				osMailPut(pMailRcv->productor, pMailSnd);  // reply Mail
			}
			else	newSend(arg.p->MailId, pMailRcv->productor, 0,  "%s", "+err@MBI5024 osMailCAlloc_fail\r\n");
			osMailFree(arg.p->MailId, pMailRcv);
		}
//		dev.Polling(&dev.rsrc);
		osDelay(64);
	}
}

static s8 createTaskFbled5050x3(osPriority priority, ...){
	va_list ap;
	u8 i;
	char* pStr;

	memset(&taskArguFbled5050x3, 0, sizeof(TaskArgument_t));
	for(i=0;i<MAX_DEV;i++){
		if(devInfo[i].base == NULL){
			memset(&devInfo[i],0,sizeof(TASK_BONDING_T));
			devInfo[i].base = &TaskInfoFbled5050x3;
			taskArguFbled5050x3.p = &devInfo[i];
			break;
		}
	}	
	if(taskArguFbled5050x3.p == NULL)	return -1;
	va_start(ap, priority);	//get last arg addr	
	pStr = va_arg(ap, char*);
	strcpy(taskArguFbled5050x3.devName, pStr);
	taskArguFbled5050x3.PORT = (const DEV_PORT*)va_arg(ap, const DEV_PORT*);
	va_end(ap);	
	
	xTaskCreate( StartTaskFbled5050x3, "led", 100, &taskArguFbled5050x3, tskIDLE_PRIORITY+(priority-osPriorityIdle), &taskArguFbled5050x3.p->TaskID);		//stack size in 2bytes	
	while(taskArguFbled5050x3.p->MailId == NULL)	osDelay(4);
	return 0;
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
