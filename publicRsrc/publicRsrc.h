/**********************************************************
filename: publicRsrc.h
**********************************************************/
#ifndef _PUBLIC_RSRC_H_
#define _PUBLIC_RSRC_H_

//#include "board.h"	
#include "misc.h"
#include "mystring.h"
#include "convert.h"
//#include "mailOp.h"
#include "gpioDecal.h"
//#include "compiler.h"
#include "responseX.h"

/* Private define ------------------------------------------------------------*/

/****************************************************************************
 @ enums
****************************************************************************/
/*****************************************************************************
 @ typedefs
****************************************************************************/

extern const char RESPONSE_OK[];
extern const char RESPONSE_ERR[];
extern const char RESPONSE_MSG[];

extern CONVERT_T CONVERT;
extern GpioConfigOp_t PIN_OP;;
//extern TASK_BASE_T* taskInfo[MAX_TASK];
//extern TASK_BONDING_T devInfo[MAX_INSTANCE];
//extern void print(const char* X, ...);
//extern void printS(const char* X);

void SetupPublicRsrc(void);
void delayMs(u16 ms);
//void taskInfoTabAdd(TASK_BASE_T*);
//s8 devInfo_RegName(TASK_BONDING_T* xDevInfo, char* devName, const char* GIVEN_NAME);

#endif
