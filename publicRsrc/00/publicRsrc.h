/**********************************************************
filename: publicRsrc.h
**********************************************************/
#ifndef _PUBLIC_RSRC_H_
#define _PUBLIC_RSRC_H_

//#include "board.h"	
#include "misc.h"
#include "mystring.h"
#include "convert.h"
#include "mailOp.h"
#include "gpioDecal.h"
#include "compiler.h"

/* Private define ------------------------------------------------------------*/

/****************************************************************************
 @ enums
****************************************************************************/
/*****************************************************************************
 @ typedefs
****************************************************************************/

extern CONVERT_T CONVERT;
extern GpioConfigOp_t gpioConfig;
extern TASK_BONDING_T newTaskTab[MAX_TASK];

void SetupPublicRsrc(void);
void delayMs(u16 ms);
void taskLinkReset(void);
void taskLinkAdd(const u8* TYPE, s8 (*task)	(u8 argc, ...), osThreadId* pTaskId, osMailQId* pMailId, const u8* HELP);
void taskTabAddDevName(const u8* TYPE, u8 *p, const u8* NAME);
void taskTabDelDevName(const u8* TYPE, u8 *p);
u8 isDevNameAvalid(const u8* TYPE, const u8 *p);
#endif
