/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : newtask.c
* Author             : Tiko Zhong
* Date First Issued  : 12/28/2016
* Description        : 
*                      
********************************************************************************
* History:
* 12/28/2016: V0.0	
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
//common
#include "board.h"
#include "publicRsrc.h"
#include "newtask.h"
//basic task
#include "myuart_os.h"
#include "dictionary_os.h"
#include "output_os.h"
#include "input_os.h"
#include "fs_os.h"
//external task
#include "cylinder_os.h"
#include "isl29125rom_os.h"
#include "mcp3421_os.h"

//#include "network_os.h"
//#include "uartFwd_os.h"
//#include "FBLED5050X3_os.h"

//#include "mcp4822_os.h"
//#include "heatProb_os.h"

//#include "console_os.h"

//#include "isl29125ROM_os.h"
//#include "cspin_os.h"
//#include "mcp3421_os.h"
//#include "isl29125ROM_os.h"
//#include "tm1638_os.h"

//#include "max44009_os.h"
//#include "myAdc_os.h"

/* Public variables ----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
//#define CMD_RTN_LEN	128
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/*******************************************************************************
* Function Name  : newTask
* Description    : to register and run tasks
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void newTask(void){
	//config
	SetupPublicRsrc();	//should be run before boardInit()
	boardInit();

	//register task
	taskInfoTabAdd(&TaskInfoUart);
	taskInfoTabAdd(&TaskInfoDict);
	taskInfoTabAdd(&TaskInfoOutput);
	taskInfoTabAdd(&TaskInfoInput);
	taskInfoTabAdd(&TaskInfoFs);
	taskInfoTabAdd(&TaskInfoCylinder);
	taskInfoTabAdd(&TaskInfoIsl29125);
  taskInfoTabAdd(&TaskInfoMcp3421);
	
	//default task
	TaskInfoUart.CreateTask(osPriorityNormal, "uart", &LED, 57600);
	TaskInfoOutput.CreateTask(osPriorityNormal, "output", &PCA9539[0], &PCA9539[1], 0x0000);
	TaskInfoInput.CreateTask(osPriorityNormal, "input", &PCA9539[0], &PCA9539[1]);
	TaskInfoDict.CreateTask(osPriorityNormal, "dict", &EEPROM);
	TaskInfoFs.CreateTask(osPriorityNormal,"fs");
	if(mcp3421.rsrc.error == 0)	TaskInfoMcp3421.CreateTask(osPriorityNormal,"meter", &mcp3421);
//	TaskInfoIsl29125.CreateTask(osPriorityNormal, "rgb", 0);
	//	TaskInfoCylinder.CreateTask(osPriorityNormal, "door", 2, 3, 0, 1, 0);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
