/******************** (C) COPYRIGHT 2015 INCUBECN *********************
* File Name          : mcp4822_os.h
* Author             : Tiko Zhong
* Date First Issued  : 05/17/2018
* Description        : 
*                      
***********************************************************************
* History:
* 01/19/2017: V0.0	
**********************************************************************/
/* Includes ---------------------------------------------------------*/
#ifndef _MCP4822_OS_H
#define _MCP4822_OS_H
#include "mcp4822_cmd.h"
#include "misc.h"
/* Public variables ------------------------------------------------*/
extern osMailQId MCP4822TaskMailId;
extern osThreadId MCP4822TaskHandle;
extern const u8 MCP4822_T[];
extern const u8 MCP4822_FILE[];
extern const u8 MCP4822_HEAD[];
extern const u8 MCP4822_TAIL[];
/* Private typedef --------------------------------------------------*/
/* Private define ---------------------------------------------------*/
/* Private macro ----------------------------------------------------*/
/* Private variables ------------------------------------------------*/
/* Private function prototypes --------------------------------------*/
s8 newMCP4822Task(u8 argc, ...);

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
