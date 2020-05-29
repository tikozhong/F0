/**********************************************************
filename: mcp4822.h
**********************************************************/
#ifndef _MCP4822_H_
#define _MCP4822_H_

#include "publicRsrc.h"

#define MCP4822_SET GPIO_PIN_SET
#define MCP4822_RESET GPIO_PIN_RESET

/*****************************************************************************
 @ typedefs
****************************************************************************/
typedef struct{
	u8 name[DEV_NAME_LEN];
	/* config	*/
	PIN_T CS;
	PIN_T SCK;
	PIN_T SDI;
	//VAR
}MCP4822Rsrc_T;

typedef struct{
	MCP4822Rsrc_T rsrc;
	void (*SetVolt_mV)	(MCP4822Rsrc_T* pRsrc, u8 ch, u16 mv);
	void (*SetVolt_500uV)	(MCP4822Rsrc_T* pRsrc, u8 ch, u16 _500uV);
}MCP4822Dev_t;
//sizeof(Tm1638Dev_t) = ???

s8 mcp4822DevSetup(
	MCP4822Dev_t *pDev, 
	PIN_T CS, 
	PIN_T SCK,
	PIN_T SDI
);
		
#endif
