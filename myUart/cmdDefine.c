/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : cmddefine.c
* Author             : Tiko Zhong
* Date First Issued  : 11/18/2016
* Description        : 
*                      
********************************************************************************
* History:
* 12/10/2016: V0.0	
*******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "cmdDefine.h"
/* Private define ------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/

/* Public variables ---------------------------------------------------------*/
//transform input
const u8 IN_DEF0_OLD[] = {"open door\r\n"};		const u8 IN_DEF0_NEW[] = {"door.gotopos(rstpos)\r\n"};
const u8 IN_DEF1_OLD[] = {"close door\r\n"};	const u8 IN_DEF1_NEW[] = {"door.gotopos(actpos)\r\n"};
const u8 IN_DEF2_OLD[] = {"go inside\r\n"};		const u8 IN_DEF2_NEW[] = {"dut.gotopos(actpos)\r\n"};
const u8 IN_DEF3_OLD[] = {"go outside\r\n"};	const u8 IN_DEF3_NEW[] = {"dut.gotopos(rstpos)\r\n"};
CMD_DEF_T CMD_TRS_IN[TRX_IN_DEFS] = {
	{IN_DEF0_OLD, IN_DEF0_NEW},
	{IN_DEF1_OLD, IN_DEF1_NEW},
	{IN_DEF2_OLD, IN_DEF2_NEW},
	{IN_DEF3_OLD, IN_DEF3_NEW},
};

//transform output
const u8 OUT_DEF0_OLD[] = {"door.gotopos(rstpos)\r\n"};	const u8 OUT_DEF0_NEW[] = {"open door\r\n"};
const u8 OUT_DEF1_OLD[] = {"door.gotopos(actpos)\r\n"};	const u8 OUT_DEF1_NEW[] = {"close door\r\n"};
const u8 OUT_DEF2_OLD[] = {"rstpos"};	const u8 OUT_DEF2_NEW[] = {"1"};
const u8 OUT_DEF3_OLD[] = {"actpos"};	const u8 OUT_DEF3_NEW[] = {"2"};

CMD_DEF_T CMD_TRS_OUT[TRX_OUT_DEFS] = {
	{OUT_DEF0_OLD, OUT_DEF0_NEW},
	{OUT_DEF1_OLD, OUT_DEF1_NEW},
	{OUT_DEF2_OLD, OUT_DEF2_NEW},
	{OUT_DEF3_OLD, OUT_DEF3_NEW},
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/


