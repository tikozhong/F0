/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : misc.c
* Author             : Tiko Zhong
* Date First Issued  : 07/03/201
* Description        : 
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "publicRsrc.h"

/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/ 
u8 pubicFlag;
//DEV_NAME_DEV_T DEVNAME;	//device name manager
//RESPONSE_OP_T	RESPONSE;
CONVERT_T CONVERT;		//convert dev
GpioConfigOp_t gpioConfig;
TASK_BONDING_T newTaskTab[MAX_TASK] = {NULL};
//TASK_BONDING_T* pTaskTab = NULL;

/* Private function prototypes -----------------------------------------------*/
/*******************************************************************************
* Function Name  : SetupPublicRsrc
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SetupPublicRsrc(void){
	pubicFlag = 0;
//	SetupDevNameDev(&DEVNAME);
	convertSetup(&CONVERT);
//	setupResponseOP(&RESPONSE);
	SetupGpioConfigOp(&gpioConfig);
}

void delayMs(u16 ms){
	osDelay(ms);
}

void taskLinkReset(void){
	u8 i;
	for(i=0;i<MAX_TASK; i++){	
		newTaskTab[i].DEV_TYPE = NULL;
		newTaskTab[i].StartTask = NULL;
		newTaskTab[i].TaskId = NULL;
		newTaskTab[i].MailId = NULL;
		newTaskTab[i].HELP = NULL;
	}
}

void taskLinkAdd(const u8* TYPE, s8 (*task)	(u8 argc, ...), osThreadId* pTaskId, osMailQId* pMailId, const u8* HELP){
	u8 i;
	for(i=0;i<MAX_TASK; i++){	
		if(newTaskTab[i].DEV_TYPE == NULL){
			newTaskTab[i].DEV_TYPE = TYPE;
			newTaskTab[i].StartTask = task;
			newTaskTab[i].TaskId = pTaskId;
			newTaskTab[i].MailId = pMailId;
			newTaskTab[i].HELP = HELP;
			break;
		}
	}
}

void taskTabAddDevName(const u8* TYPE, u8 *p, const u8* NAME){
	u8 i,j;
	for(i=0;i<MAX_TASK; i++){
		if(newTaskTab[i].DEV_TYPE == TYPE){
			for(j=0;j<MAX_DEV_NBR;j++){
				if(newTaskTab[i].devName[j] == NULL){
					newTaskTab[i].devName[j] = p;
					memset(p,0,DEV_NAME_LEN);
					strCpy(p, DEV_NAME_LEN, NAME);
					break;
				}
			}
		}
	}
}

void taskTabDelDevName(const u8* TYPE, u8 *p){
	u8 i,j;
	for(i=0;i<MAX_TASK; i++){	
		if(newTaskTab[i].DEV_TYPE == TYPE){
			for(j=0;j<MAX_DEV_NBR;j++){
				if(newTaskTab[i].devName[j] == p){
					newTaskTab[i].devName[j] = NULL;
					break;
				}
			}
		}
	}
}

u8 isDevNameAvalid(const u8* TYPE, const u8 *p){
	u8 i,j;
	for(i=0;i<MAX_TASK;i++){
		if(newTaskTab[i].TaskId == NULL)	continue;
		if(newTaskTab[i].MailId == NULL)	continue;
		for(j=0;j<MAX_DEV_NBR;j++)
			if(isSameStr(newTaskTab[i].devName[j], p))	return 0;
	}
	if(TYPE == NULL)	return 1;	/* if no consider avilid name for this type */
	for(i=0;i<MAX_TASK;i++){
		if(newTaskTab[i].TaskId == NULL)	continue;
		if(newTaskTab[i].MailId == NULL)	continue;
		if(isSameStr(newTaskTab[i].DEV_TYPE, p)){
			for(j=0;j<MAX_DEV_NBR;j++)
				if(newTaskTab[i].devName[j] == NULL)	return 1;	
		}
	}
	if(i == MAX_TASK)	return 1;
	return 0;
}

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
