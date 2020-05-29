/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : uartFwd_os.c
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
#include "uartFwd_os.h"
#include "responseX.h"
#include "stdarg.h"
#include "misc.h"
#include "uartFwd_cmd.h"
/* Public variables ---------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
typedef struct{
	char devName[DEV_NAME_LEN];
	TASK_BONDING_T* p;
	UART_HandleTypeDef* UART[5];
	PIN_T *SEL;
}TaskArgument_t;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static const u8 UARTFWD_T[] = {"uartfwd_t"};
/* Private function prototypes -----------------------------------------------*/
static void StartTask(void * argument);
static s8 createTaskUartFwd(osPriority priority, ...);

const TASK_BASE_T TaskInfoUartFwd = {
	UARTFWD_T,				//	const u8* DEV_TYPE;
	UARTFWD_LED_HELP,	//	const u8* HELP;
	createTaskUartFwd,			//	s8 (*CreateTask)	(u8 argc, ...);
};
static TaskArgument_t taskArgument = {0};
/*******************************************************************************
* Function Name  : StartTask
* Description    : per 4ms timer call back, do inputFetch
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void StartTask(void * argument){
	osEvent  evt;
	Mail_T *pMailRcv, *pMailSnd;
	UARTFWD_Dev_T dev;
	osMailQId UartTaskMailId = devInfo[0].MailId;
	TaskArgument_t arg = *(TaskArgument_t*)argument;
	char str[32] = {0};

	//new local mail component	
	osMailQDef(mbi5024TaskMail, 1, Mail_T);
	while(arg.p->MailId == NULL){ 
		arg.p->MailId = osMailCreate(osMailQ(mbi5024TaskMail), NULL);
	}
	UARTFWD_Setup(&dev, arg.UART[0], arg.UART[1], arg.UART[2], arg.UART[3], arg.UART[4], *arg.SEL);
	devInfo_RegName(arg.p, dev.rsrc.name, arg.devName);
	strcat(str, "start ");	strcat(str, arg.devName);	strcat(str, "... ok");
	newSend(arg.p->MailId, UartTaskMailId, 0,  "%s", str);
	
	for(;;){
		evt = osMailGet(arg.p->MailId, 0);        // wait for mail
		if (evt.status == osEventMail){						//if there is mail, 2 operations: reply it, free it
			pMailRcv = (Mail_T*)evt.value.p;
			pMailSnd = newMail(arg.p->MailId, pMailRcv->productor, pMailRcv->traceIndex);   // Allocate memory
			if(pMailSnd != NULL){
				uartFwdCmd(&dev, &pMailRcv->Content, &pMailSnd->Content);
				osMailPut(pMailRcv->productor, pMailSnd);  // reply Mail
			}
			else	newSend(arg.p->MailId, pMailRcv->productor, 0,  "%s", "+err@MBI5024 osMailCAlloc_fail\r\n");
			osMailFree(arg.p->MailId, pMailRcv);
		}
//		x <<=1;
//		if(x==0x0100)	x=0x0101;
//		dev.rsrc.pca9539.Write(&dev.rsrc.pca9539.rsrc, x);
		dev.Polling(&dev.rsrc);
		osDelay(4);
	}
}

static s8 createTaskUartFwd(osPriority priority, ...){
	va_list ap;
	u8 i;
	char* pStr;

	memset(&taskArgument, 0, sizeof(TaskArgument_t));	
	for(i=0;i<MAX_DEV;i++){
		if(devInfo[i].base == NULL){
			memset(&devInfo[i],0,sizeof(TASK_BONDING_T));
			devInfo[i].base = &TaskInfoUartFwd;
			taskArgument.p = &devInfo[i];				
			break;
		}
	}	
	if(taskArgument.p == NULL)	return -1;
	//createTask(u8 argc, priority, "devName", portIndx)
	va_start(ap, priority);	//get last arg addr	
	pStr = va_arg(ap, char*);
	strcpy(taskArgument.devName, pStr);
	taskArgument.UART[0] = (UART_HandleTypeDef*)va_arg(ap, UART_HandleTypeDef*);
	taskArgument.UART[1] = (UART_HandleTypeDef*)va_arg(ap, UART_HandleTypeDef*);
	taskArgument.UART[2] = (UART_HandleTypeDef*)va_arg(ap, UART_HandleTypeDef*);
	taskArgument.UART[3] = (UART_HandleTypeDef*)va_arg(ap, UART_HandleTypeDef*);
	taskArgument.UART[4] = (UART_HandleTypeDef*)va_arg(ap, UART_HandleTypeDef*);
	taskArgument.SEL = (PIN_T*)va_arg(ap, PIN_T*);
	va_end(ap);	
	
	xTaskCreate( StartTask, "uartfwd", 2000, &taskArgument, tskIDLE_PRIORITY+(priority-osPriorityIdle), &taskArgument.p->TaskID);		//stack size in 4bytes	
	while(taskArgument.p->MailId == NULL)	osDelay(1);
	return 0;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
