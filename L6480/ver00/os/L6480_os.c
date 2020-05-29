/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : l6480_os.c
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
#include "l6480_os.h"
#include "stdarg.h"
#include "cspin_command.h"
/* Public variables ---------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
typedef struct{
	TASK_BONDING_T* p;	
	char devName[DEV_NAME_LEN];
	PCA9539_Dev_T* PCA9539[2];
	u32 intiStatus;
}stepperTaskArgument_t;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const u8 STEPPER_T[] = {"stepper_t"};
/* Private function prototypes -----------------------------------------------*/
static void StartTaskStepper(void * argument);
static s8 createTaskStepper(osPriority priority, ...);
static stepperTaskArgument_t taskArg = {0};
const TASK_BASE_T TaskInfoL6480 = {
	STEPPER_T,						//	const u8* DEV_TYPE;
	STEPPER_HELP,					//	const u8* HELP;
	createTaskStepper,		//	s8 (*CreateTask)	(u8 argc, ...);
};

/*******************************************************************************
* Function Name  : StartTask
* Description    : per 4ms timer call back, do inputFetch
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void StartTaskStepper(void * argument){
	osEvent  evt;
	Mail_T *pMailRcv, *pMailSnd;
	cSPIN_DEV_T dev = {0};
	osMailQId UartTaskMailId = devInfo[0].MailId;
	char str[32]={0};
	TASK_BONDING_T* p = NULL;
	stepperTaskArgument_t* arg = (stepperTaskArgument_t*)argument;
	
	cSPIN_Setup(&dev);	
	p = arg->p;
	
	//new local mail component
	osMailQDef(outputTaskMail, 1, Mail_T);
	while(p->MailId == NULL){
		p->MailId = osMailCreate(osMailQ(outputTaskMail), NULL);
	}
	devInfo_RegName(p, (char*)dev.rsrc.name, arg->devName);
	strcat(str, "start ");	strcat(str, p->devName);	strcat(str, "... ok");
	newSend(p->MailId, UartTaskMailId, 0,  "%s", str);

	for(;;){
		evt = osMailGet(p->MailId, 0);    // wait for mail
		xSemaphoreTake( xSemaphoreIIC, portMAX_DELAY);
		if (evt.status == osEventMail){		//if there is mail, 2 operations: reply it, free it
			pMailRcv = (Mail_T*)evt.value.p;
			pMailSnd = newMail(p->MailId, pMailRcv->productor, pMailRcv->traceIndex);   // Allocate memory
			if(pMailSnd != NULL){
				stepperCmd(&dev, &pMailRcv->Content, &pMailSnd->Content);
				osMailPut(pMailRcv->productor, pMailSnd);  // reply Mail
			}
			else	newSend(p->MailId, pMailRcv->productor, 0,  "%s", "+err@OUTPUT osMailCAlloc_fail\r\n");
			osMailFree(p->MailId, pMailRcv);
		}
//		dev.Polling(&dev.rsrc);
		xSemaphoreGive(xSemaphoreIIC);
		osDelay(1);
	}
}

//this function is run outside this task
static s8 createTaskStepper(osPriority priority, ...){
	va_list ap;
	u8 i;
	char* pStr;

	memset(&taskArg, 0, sizeof(stepperTaskArgument_t));
	for(i=0;i<MAX_DEV;i++){
		if(devInfo[i].base == NULL){
			memset(&devInfo[i],0,sizeof(TASK_BONDING_T));
			devInfo[i].base = &TaskInfoL6480;
			taskArg.p = &devInfo[i];
			break;
		}
	}	
	if(taskArg.p == NULL)	return -2;
	va_start(ap, priority);	//get last arg addr	
	pStr = va_arg(ap, char*);
	strcpy(taskArg.devName, pStr);
	taskArg.PCA9539[0] = va_arg(ap, PCA9539_Dev_T*);
	taskArg.PCA9539[1] = va_arg(ap, PCA9539_Dev_T*);
	taskArg.intiStatus = va_arg(ap, u32);
	va_end(ap);	

	xTaskCreate( StartTaskStepper, "stepper", 100, &taskArg, tskIDLE_PRIORITY+(priority-osPriorityIdle), &taskArg.p->TaskID);		//stack size in 4bytes	
	while(taskArg.p->MailId == NULL)	osDelay(1);
	return 0;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
