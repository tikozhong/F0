/******************** (C) COPYRIGHT 2015 INCUBECN *********************
* File Name          : leddrvr_os.h
* Author             : Tiko Zhong
* Date First Issued  : 09/04/2017
* Description        : 
*                      
***********************************************************************
* History:
* 09/04/2017: V0.0	
**********************************************************************/
/* Includes ---------------------------------------------------------*/
#ifndef _LEDDRVR_OS_H
#define _LEDDRVR_OS_H

#include "misc.h"
#include "leddrvr_cmd.h"
/* Public variables ------------------------------------------------*/
extern osMailQId leddrvrTaskMailId;
extern osThreadId leddrvrTaskHandle;
extern const u8 LEDDRVR_T[];
//extern AT24C256C_Dev_T EEPROM;
/* Private typedef --------------------------------------------------*/
/* Private define ---------------------------------------------------*/
/* Private macro ----------------------------------------------------*/
/* Private variables ------------------------------------------------*/
/* Private function prototypes --------------------------------------*/
s8 newLeddrvrTask(u8 argc, ...);

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
