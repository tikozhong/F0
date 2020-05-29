/**********************************************************
filename: w5500x.h
**********************************************************/
#ifndef _W5500_H_
#define _W5500_H_

#include "misc.h"
#include "wizchip_conf.h"
/*****************************************************************************
 @ typedefs
****************************************************************************/
typedef struct{
	u8 name[DEV_NAME_LEN];
	/* config	*/
	SPI_HandleTypeDef* SPI;
	PIN_T CS, IRQ;
	_WIZCHIP* WIZCHIP;
	wiz_NetInfo netInfo;
	wiz_PhyConf phyCfg;
	wiz_NetTimeout timeOut;
	u32 error;
}W5500_Rsrc_T;

/*
	when page write more than 64 bytes,
	pBuf must point to a static pointer
*/
typedef struct{
	W5500_Rsrc_T rsrc;
	//op
	void (*SetIp)	(W5500_Rsrc_T*, u8 ip0, u8 ip1, u8 ip2, u8 ip3);
	void (*SetIpMask)	(W5500_Rsrc_T*, u8 msk0, u8 msk1, u8 msk2, u8 msk3);
	void (*SetGateWay)	(W5500_Rsrc_T*, u8 gw0, u8 gw1, u8 gw2, u8 gw3);	
	void (*GetNetInfo)	(W5500_Rsrc_T*);
}W5500_Dev_T;

DEV_STATUS W5500_Setup(
	W5500_Dev_T *pDev, 
	SPI_HandleTypeDef* SPI,
	const PIN_T CS, 
	const PIN_T IRQ
);
#endif
