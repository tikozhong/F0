/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : cylinder_os.c
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
#include "cylinder_os.h"
#include "stdarg.h"
#include "cylinderCmd.h"
#include "input_os.h"
#include "output_os.h"
/* Public variables ---------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/
typedef struct{
	TASK_BONDING_T* p;	
	char devName[DEV_NAME_LEN];
	u8 drvRstIndx;
	u8 drvActIndx;
	u8 snsRstIndx; 
	u8 snsActIndx; 
	u8 msgEn;
}TaskArgument_t;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const char CYLINDER_T[] = {"cldr_t"};
/* Private function prototypes -----------------------------------------------*/
static void StartTask(void * argument);
static s8 createTaskCylinder(osPriority priority, ...);
static s8 createTaskCylinderByCmd(osPriority priority, void* p);
TaskArgument_t taskArg = {0};
TASK_BASE_T TaskInfoCylinder = {
	CYLINDER_T,						//	const u8* DEV_TYPE;
	CLDR_HELP,				//	const u8* HELP;
	createTaskCylinder,		//	s8 (*CreateTask)	(u8 argc, ...);
	createTaskCylinderByCmd,
	0
};

/*******************************************************************************
* Function Name  : StartTask
* Description    : per 4ms timer call back, do inputFetch
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void StartTask(void * argument){
	u32 inputStatus, outputStatus;
	osEvent  evt;
	Mail_T *pMailRcv, *pMailSnd;
	CLDR_DEV_T dev = {0};
//	char str[32]={0};
	TASK_BONDING_T* p = NULL;
	TaskArgument_t* arg = (TaskArgument_t*)argument;
	
	cylinderSetup(&dev, arg->drvRstIndx, arg->drvActIndx, arg->snsRstIndx, arg->snsActIndx, arg->msgEn);	
	p = arg->p;
	p->base->count++;	
	
	//new local mail component
	osMailQDef(cylinderTaskMail, 1, Mail_T);
	while(p->MailId == NULL){
		p->MailId = osMailCreate(osMailQ(cylinderTaskMail), NULL);
	}
	devInfo_RegName(p, (char*)dev.rsrc.name, arg->devName);
	print("start %s...ok\n", arg->devName);

	for(;;){
		evt = osMailGet(p->MailId, 0);    // wait for mail
		xSemaphoreTake( xSemaphoreIIC, portMAX_DELAY);
		if (evt.status == osEventMail){		//if there is mail, 2 operations: reply it, free it
			pMailRcv = (Mail_T*)evt.value.p;
			pMailSnd = newMail(p->MailId, pMailRcv->productor, pMailRcv->traceIndex);   // Allocate memory
			if(pMailSnd != NULL){
				cylinderCmd(&dev, &pMailRcv->Content, &pMailSnd->Content);
				osMailPut(pMailRcv->productor, pMailSnd);  // reply Mail
			}
			else	newSend(p->MailId, pMailRcv->productor, 0,  "%s", "+err@CYLINDER osMailCAlloc_fail\r\n");
			osMailFree(p->MailId, pMailRcv);
		}
		inputStatus = INPUT.rsrc.status;
		outputStatus = OUTPUT.rsrc.status;
		dev.TickTask(&dev.rsrc, inputStatus, &outputStatus);
		OUTPUT.WritePinHEX(&OUTPUT.rsrc, outputStatus);
		xSemaphoreGive(xSemaphoreIIC);
		osDelay(16);
	}
}

static s8 createTaskCylinderByCmd(osPriority priority, void* p){
	PAKET_T* packet = p;
	u32 x[4];
	if(packetIsMatch(packet,"%s%s%d%d%d%d") && isSameStr(packet->addr[0], CYLINDER_T)){
		x[0] = *(u32*)packet->addr[2];
		x[1] = *(u32*)packet->addr[3];
		x[2] = *(u32*)packet->addr[4];
		x[3] = *(u32*)packet->addr[5];
		createTaskCylinder(priority, packet->addr[1], x[0],x[1],x[2],x[3]);
		return 0;
	}
	return -1;
}

//this function is run outside this task
static s8 createTaskCylinder(osPriority priority, ...){
	va_list ap;
	u8 i;
	char* pStr;

	memset(&taskArg, 0, sizeof(TaskArgument_t));
	for(i=0;i<MAX_INSTANCE;i++){
		if(devInfo[i].base == NULL){
			memset(&devInfo[i],0,sizeof(TASK_BONDING_T));
			devInfo[i].base = &TaskInfoCylinder;
			taskArg.p = &devInfo[i];
			break;
		}
	}	
	if(taskArg.p == NULL)	return -2;
	va_start(ap, priority);	//get last arg addr	
	pStr = va_arg(ap, char*);
	strcpy(taskArg.devName, pStr);
	taskArg.drvRstIndx = va_arg(ap, u32);
	taskArg.drvActIndx = va_arg(ap, u32);
	taskArg.snsRstIndx = va_arg(ap, u32);
	taskArg.snsActIndx = va_arg(ap, u32);
	taskArg.msgEn = va_arg(ap, u32);
	va_end(ap);	

	xTaskCreate( StartTask, "cylinder", 100, &taskArg, tskIDLE_PRIORITY+(priority-osPriorityIdle), &taskArg.p->TaskID);		//stack size in 4bytes	
	while(taskArg.p->MailId == NULL)	osDelay(1);
	return 0;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
