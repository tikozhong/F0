/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : cylinder_os.c
* Author             : Tiko Zhong
* Date First Issued  : 11/18/2016
* Description        : 
*                      
********************************************************************************
* History:
* 11/18/2016: V0.0	
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "cmsis_os.h"
#include "misc.h"
#include "board.h"
#include "myString.h"
#include "L6480_os.h"
//#include "cylindersMngrCmd.h"
#include "cspin_command.h"

/* Public variables ---------------------------------------------------------*/
extern osMailQId UartTaskMailId;
extern osMailQId MotorTaskMailId;
/* Private define ------------------------------------------------------------*/
//#define CMD_RTN_LEN	128
#define CLDR_SUM	8
/* Private typedef -----------------------------------------------------------*/
CLDR_MNGR_DEV_T cylinders;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/*******************************************************************************
* Function Name  : inputFetch
* Description    : per 4ms timer call back, do inputFetch
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void StartMotorTask(void const * argument){
	osMailQId owner;
	osEvent  evt;
	CLDR_DEV_T *pDev;
	Mail_T *pMailRcv, *pMailSnd;
	u8 i;	u32 inputSta,outputSta;

	owner = CylinderTaskMailId;
	sendMail(owner, UartTaskMailId, 0xff, (const u8*)"CylinderManager setup..");
	if(cylindersManagerSetup(&cylinders, CLDR_SUM) == DEV_SUCCESS)
		sendMail(owner, UartTaskMailId, 0xff, (const u8*)"ok\r\n");
	else	sendMail(owner, UartTaskMailId, 0xff, (const u8*)"err\r\n");
	for(;;){
		//deal mail
		evt = osMailGet(owner, 0);        // wait for mail
		if(evt.status == osEventMail){
			pMailRcv = evt.value.p;
			pMailSnd = osMailCAlloc(pMailRcv->productor, osWaitForever);       // Allocate memory
			if(pMailSnd != NULL){
				//new a mail to reply
				pMailSnd->productor = owner;
				pMailSnd->traceIndex = pMailRcv->traceIndex;
				memset(pMailSnd->content, 0, BYTES_PER_MAIL);	
				//exe cmd
				if(cylindersMngrCmd(&cylinders, pMailRcv->content, pMailSnd->content, BYTES_PER_MAIL) == 0){
					for(i=0; i<cylinders.rsrc.devCap; i++){
						pDev = cylinders.rsrc.pDevs[i];
						if(pDev == NULL)	continue;
						if(cylinderCmd(pDev, pMailRcv->content, pMailSnd->content, BYTES_PER_MAIL))	break;
					}
				}
				osMailPut(pMailRcv->productor, pMailSnd);                         // reply Mail
			}
			else	sendMail(owner, pMailRcv->productor, 0xff, (const u8*)"+err@cylinder_osMailCAlloc_fail\r\n");
			osMailFree(owner, pMailRcv);
		}
		//polling
		osMutexWait(devInputMutexHandle, osWaitForever);
		inputSta = devInput.rsrc.status;
		osMutexRelease(devInputMutexHandle);
		osMutexWait(devOutputMutexHandle, osWaitForever);
		outputSta = devOutput.rsrc.outputBuf;
		for(i=0; i<cylinders.rsrc.devCap; i++){
			pDev = cylinders.rsrc.pDevs[i];
			if(pDev == NULL)	continue;
			pDev->TickTask(&pDev->rsrc, inputSta, &outputSta);
		}
		devOutput.WritePinHEX(&devOutput.rsrc, outputSta);
		osMutexRelease(devOutputMutexHandle);
		//sent callback msg
		for(i=0; i<cylinders.rsrc.devCap; i++){
			pDev = cylinders.rsrc.pDevs[i];
			if(pDev == NULL)	continue;
			if(pDev->IsMsg(&pDev->rsrc)){
				pMailSnd = osMailCAlloc(UartTaskMailId, osWaitForever);
				pDev->MakeCallBackMsg(&pDev->rsrc, pMailSnd->content, BYTES_PER_MAIL);
				pMailSnd->productor = owner;
				pMailSnd->traceIndex = 0xff;
				osMailPut(UartTaskMailId, pMailSnd);       // reply Mail
			}			
		}
		osDelay(16);
	}
}

//sys.newdev("type" ,"argument..")
u8 newL6480Task (PARAM_DEV_T *e){
	if(e->rsrc.indx != 2 || pFsDev != NULL)	return 0;
	if(indexOf(e->rsrc.params[0], FS_T) != 0)	return 0;
	osThreadDef(fsTask, StartFsTaskTask, osPriorityNormal, 0, 640);
	fsTaskHandle = osThreadCreate(osThread(fsTask), &e->rsrc);
	return 1;	
}

void disposeL6480Task(void){
	if(pDev != NULL)	devNameList_del(pDev->rsrc.name);
	osThreadTerminate(alsTaskHandle);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
