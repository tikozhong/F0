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

const char RESPONSE_OK[] = {"+ok@"};
const char RESPONSE_ERR[] = {"+err@"};
const char RESPONSE_MSG[] = {"+msg@"};
DEV_NAME_DEV_T DEVNAME;	//device name manager
RESPONSE_OP_T	RESPONSE;
CONVERT_T CONVERT;		//convert dev
GpioConfigOp_t PIN_OP;
TASK_BASE_T* taskInfo[MAX_TASK] = {0};
TASK_BONDING_T devInfo[MAX_INSTANCE] = {0};
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
	convertSetup(&CONVERT);
	SetupGpioConfigOp(&PIN_OP);
}

void delayMs(u16 ms){
	osDelay(ms);
}

void taskInfoTabAdd(TASK_BASE_T* p){
	u8 i;
	for(i=0;i<MAX_TASK; i++){
		if(taskInfo[i] == NULL){
			taskInfo[i] = p;
			break;
		}
	}
}

s8 devInfo_RegName(TASK_BONDING_T* xDevInfo, char* devName, const char* GIVEN_NAME){
	u8 i;
	//check if same name as GIVEN_NAME
	for(i=0;i<MAX_INSTANCE; i++){
		if(devInfo[i].base == NULL || devInfo[i].MailId == NULL)	continue;
		if(isSameStr(devInfo[i].devName, GIVEN_NAME))	return -1;
	}

	xDevInfo->devName = devName;
	memset(devName,0,DEV_NAME_LEN);
	strcpy(devName, GIVEN_NAME);
	return 0;
}

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
