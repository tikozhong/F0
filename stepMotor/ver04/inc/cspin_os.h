/******************** (C) COPYRIGHT 2015 INCUBECN *********************
* File Name          : cspin_os.h
* Author             : Tiko Zhong
* Date First Issued  : 01/19/201
* Description        : 
*                      
***********************************************************************
* History:
* 01/19/2017: V0.0	
**********************************************************************/
/* Includes ---------------------------------------------------------*/
#ifndef _CSPIN_OS_H
#define _CSPIN_OS_H

#include "misc.h"
/* Public variables ------------------------------------------------*/
extern osMailQId L6480TaskMailId;
extern const u8 L6480_T[];
extern const u8 L6480_FILE[];
extern const u8 L6480_HEAD[];
extern const u8 L6480_TAIL[];
/* Private typedef --------------------------------------------------*/
/* Private define ---------------------------------------------------*/
/* Private macro ----------------------------------------------------*/
/* Private variables ------------------------------------------------*/
/* Private function prototypes --------------------------------------*/
u8 newL6480Task(void);

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
