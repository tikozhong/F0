/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : network_os.c
* Author             : Tiko Zhong
* Date First Issued  : 11/18/2016
* Description        : 
*                      
********************************************************************************
* History:
* 11/18/2016: V0.0	
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
//#include "board.h"
#include "publicRsrc.h"
#include "network_os.h"
#include "stdarg.h"
#include "w5500x.h"
#include "networkCmd.h"

/* Private define ------------------------------------------------------------*/
#define NETWORK_POLLING_TIME 16

/* Public variables ---------------------------------------------------------*/
extern osMailQId UartTaskMailId;

/* Private typedef -----------------------------------------------------------*/
typedef struct{
	TASK_BONDING_T* p;	
	char devName[DEV_NAME_LEN];
	SPI_HandleTypeDef* SPI;
	PIN_T* CS;
	PIN_T* IRQ;
}networkTaskArgument_t;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const u8 NET_WORK_T[] = {"w5500_t"};
/* Private function prototypes -----------------------------------------------*/
static void StartTaskNetwork(void * argument);
static s8 createTaskNetwork(osPriority priority, ...);
networkTaskArgument_t taskArgNetwork = {0};
const TASK_BASE_T TaskInfoNetwork = {
	NET_WORK_T,					//	const u8* DEV_TYPE;
	NULL,				//	const u8* HELP;
	createTaskNetwork,	//	s8 (*CreateTask)	(u8 argc, ...);
};
/*******************************************************************************
* Function Name  : inputFetch
* Description    : per 4ms timer call back, do inputFetch
* Network          : None
* Network         : None
* Return         : None
*******************************************************************************/
static void StartTaskNetwork(void * argument){
	osEvent  evt;
	Mail_T *pMailRcv, *pMailSnd;
	W5500_Dev_T NETWORK = {0}, *pDev = &NETWORK;
	W5500_Rsrc_T* pRsrc = &NETWORK.rsrc;
	osMailQId UartTaskMailId = devInfo[0].MailId;
	char str[32]={0};
	TASK_BONDING_T* p = NULL;
	networkTaskArgument_t* arg = (networkTaskArgument_t*)argument;

	//new a device
	W5500_Setup(&NETWORK, arg->SPI, *arg->CS, *arg->IRQ);	
	//setup os envirment
	p = arg->p;	
	//new local mail component
	osMailQDef(networkTaskMail, 1, Mail_T);
	while(p->MailId == NULL){
		p->MailId = osMailCreate(osMailQ(networkTaskMail), NULL);
	}
	devInfo_RegName(p, (char*)NETWORK.rsrc.name, arg->devName);	
	strcat(str, "start ");	strcat(str, p->devName);	strcat(str, "... ok");
	newSend(p->MailId, UartTaskMailId, 0,  "%s", str);

	for(;;){
		evt = osMailGet(p->MailId, 0);    // wait for mail
		if (evt.status == osEventMail){		//if there is mail, 2 operations: reply it, free it
			pMailRcv = (Mail_T*)evt.value.p;
			pMailSnd = newMail(p->MailId, pMailRcv->productor, pMailRcv->traceIndex);   // Allocate memory
			if(pMailSnd != NULL){
				networkCmd(&NETWORK, &pMailRcv->Content, &pMailSnd->Content);
				osMailPut(pMailRcv->productor, pMailSnd);  // reply Mail
			}
			else	newSend(p->MailId, pMailRcv->productor, 0,  "%s", "+err@NETWORK osMailCAlloc_fail\r\n");
			osMailFree(p->MailId, pMailRcv);
		}
		osDelay(NETWORK_POLLING_TIME);
	}
}

//this function is run outside this task
static s8 createTaskNetwork(osPriority priority, ...){
	va_list ap;
	u8 i;
	char* pStr;

	memset(&taskArgNetwork, 0, sizeof(networkTaskArgument_t));
	for(i=0;i<MAX_DEV;i++){
		if(devInfo[i].base == NULL){
			memset(&devInfo[i],0,sizeof(TASK_BONDING_T));
			devInfo[i].base = &TaskInfoNetwork;
			taskArgNetwork.p = &devInfo[i];
			break;
		}
	}	
	if(taskArgNetwork.p == NULL)	return -2;
	va_start(ap, priority);	//get last arg addr	
	pStr = va_arg(ap, char*);
	strcpy(taskArgNetwork.devName, pStr);
	taskArgNetwork.SPI = va_arg(ap, SPI_HandleTypeDef*);
	taskArgNetwork.CS = va_arg(ap, PIN_T*);
	taskArgNetwork.IRQ = va_arg(ap, PIN_T*);
	va_end(ap);	

	xTaskCreate( StartTaskNetwork, "network", 200, &taskArgNetwork, tskIDLE_PRIORITY+(priority-osPriorityIdle), &taskArgNetwork.p->TaskID);		//stack size in 4bytes	
	while(taskArgNetwork.p->MailId == NULL)	osDelay(1);
	return 0;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
