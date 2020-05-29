/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : mcp3421_os.c
* Author             : Tiko Zhong
* Date First Issued  : 12/28/2016
* Description        : 
*                      
********************************************************************************
* History:
* 12/28/2016: V0.0	
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "board.h"
#include "stdarg.h"
#include "mcp3421_os.h"
#include "mcp3421_CMD.h"
/* Public variables ---------------------------------------------------------*/

//extern osMailQId FsTaskMailId;

/* Private define ------------------------------------------------------------*/
#define CMD_RTN_LEN	128
#define MCP3421_CNT	3
#define MCP3421_TASK_HIGH_SPEED 4		//when there is device to run
#define MCP3421_TASK_LOW_SPEED 16		//when there is on a device

/* Private typedef -----------------------------------------------------------*/
typedef struct{
	char devName[DEV_NAME_LEN];
	TASK_BONDING_T* p;
	MCP3421_Dev_T* pDev;
}mcp3421TaskArgument_t;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const char MCP3421_T[] = {"mcp3421_t"};
const char MCP3421_FILE[] = {"board.ini"};
const u8 MCP3421_HEAD[] = {"<mcp3421>"};
const u8 MCP3421_TAIL[] = {"</mcp3421>"};

/* Private function prototypes -----------------------------------------------*/
static void StartTaskMcp3421(void * argument);
static s8 createTaskMcp3421(osPriority priority, ...);
static s8 createTaskMcp3421ByCmd(osPriority priority, void* packet);

mcp3421TaskArgument_t taskArgMcp3421 = {0};
TASK_BASE_T TaskInfoMcp3421 = {
	MCP3421_T,					//	const u8* DEV_TYPE;
	MC3421_HELP,				//	const u8* HELP;
	createTaskMcp3421,			//	s8 (*CreateTask)	(u8 argc, ...);
	createTaskMcp3421ByCmd,
	0
};


/*******************************************************************************
* Function Name  : inputFetch
* Description    : per 4ms timer call back, do inputFetch
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void StartTaskMcp3421(void * argument){
	osEvent  evt;
	Mail_T *pMailRcv, *pMailSnd;
	MCP3421_Dev_T* dev = NULL;
	TASK_BONDING_T* p = NULL;
	mcp3421TaskArgument_t* arg = (mcp3421TaskArgument_t*)argument;
//  osMailQId mcp3421CurMailID = NULL;
  
	p = arg->p;
	p->base->count++;	
	dev = arg->pDev;
	//new local mail component
	osMailQDef(mcp3421TaskMail, 1, Mail_T);
	while(arg->p->MailId == NULL){
		arg->p->MailId = osMailCreate(osMailQ(mcp3421TaskMail), NULL);
	}
	devInfo_RegName(p, (char*)dev->rsrc.name, arg->devName);
  
	print("start %s...ok\n", arg->devName);

	for(;;){
//		mcp3421CurMailID = arg->p->MailId;
		evt = osMailGet(p->MailId, 0);        // wait for mail
		if (evt.status == osEventMail){				//if there is mail, 2 operations: reply it, free it
			pMailRcv = (Mail_T*)evt.value.p;
			pMailSnd = osMailCAlloc(pMailRcv->productor, osWaitForever);       // Allocate memory
			if(pMailSnd != NULL){
				pMailSnd->productor = p->MailId;
				pMailSnd->traceIndex = pMailRcv->traceIndex;
				if(mcp3421Cmd(dev, &pMailRcv->Content, &pMailSnd->Content) == 0){
					RESPONSE_ORG(&pMailSnd->Content, ERR, &pMailRcv->Content);
				}
				osMailPut(pMailRcv->productor, pMailSnd);                         // reply Mail
			}
			else	newSend(arg->p->MailId, pMailRcv->productor, 0,  "%s", "+err@RGB Sensor osMailCAlloc_fail\r\n");
			osMailFree(arg->p->MailId, pMailRcv);
		}
		//polling task
		dev->Polling(&dev->rsrc, 4);
		osDelay(4);
	}
}

static s8 createTaskMcp3421ByCmd(osPriority priority, void* p){
	PAKET_T* packet = p;
	if(packetIsMatch(packet,"%s%s") && isSameStr(packet->addr[0], MCP3421_T)){
		createTaskMcp3421(priority, packet->addr[1], &mcp3421);
		return 0;
	}
	return -1;
}

//this function is run outside this task
static s8 createTaskMcp3421(osPriority priority, ...){
	va_list ap;
	u8 i;
	char* pStr;

	memset(&taskArgMcp3421, 0, sizeof(mcp3421TaskArgument_t));
	for(i=0;i<MAX_INSTANCE;i++){
		if(devInfo[i].base == NULL){
			memset(&devInfo[i],0,sizeof(TASK_BONDING_T));
			devInfo[i].base = &TaskInfoMcp3421;
			taskArgMcp3421.p = &devInfo[i];
			break;
		}
	}	
	if(taskArgMcp3421.p == NULL)	return -1;
	va_start(ap, priority);	//get last arg addr	
	pStr = va_arg(ap, char*);
	strcpy(taskArgMcp3421.devName, pStr);
	taskArgMcp3421.pDev = va_arg(ap, MCP3421_Dev_T*);
	va_end(ap);	

	xTaskCreate( StartTaskMcp3421, "mcp3421", 200, &taskArgMcp3421, tskIDLE_PRIORITY+(priority-osPriorityIdle), &taskArgMcp3421.p->TaskID);		//stack size in 4bytes	
	while(taskArgMcp3421.p->MailId == NULL)	osDelay(1);
	return 0;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
