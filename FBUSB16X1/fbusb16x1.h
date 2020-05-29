/**********************************************************
filename: fbusb16x1.h
**********************************************************/
#ifndef _FBUSB16X1_H_
#define _FBUSB16X1_H_

//#include "publicRsrc.h"
#include "misc.h"
#include "74hc595.h"
/*****************************************************************************
 @ typedefs
****************************************************************************/
typedef struct{
	//input
	char name[DEV_NAME_LEN];
//	DEV_PORT devPort;
	//config
	HC595DEV_T hc595;
	PIN_T ADDR[4];
	PIN_T HC595LCK1,HC595LCK2,HC595SCK,HC595DAT;
	PIN_T LED_SCK,LED_DAT;
	PIN_T RUNNING_LED;
	//private var
	u8 tick;
	u32 UsbCfg[16];
	u8 LedPortColor[16][8];
	u32 errFlags;
}FBUSB16X1_Rsrc_T;

typedef struct{
	//resource
	FBUSB16X1_Rsrc_T rsrc;
	//operation
	void (*SetUSB)(FBUSB16X1_Rsrc_T*, u8 brdIndx, u8 usbIndx, u8 pwr_on);
	void (*SetLED)(FBUSB16X1_Rsrc_T*, u8 brdIndx, u8 portIndx, u8 ledIndx, u8 color);
	u8 (*GetUSB)	(FBUSB16X1_Rsrc_T*, u8 brdIndx);
	u8 (*GetLED)	(FBUSB16X1_Rsrc_T*, u8 brdIndx, u8 ledIndx);
	void (*Polling)(FBUSB16X1_Rsrc_T*);
}FBUSB16X1_Dev_T;

DEV_STATUS FBUSB16X1_Setup(FBUSB16X1_Dev_T *pDev, DEV_PORT* port);
#endif
