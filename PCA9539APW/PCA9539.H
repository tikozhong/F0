/**********************************************************
filename: pca9539.h
**********************************************************/
#ifndef _PCA9539_H_
#define _PCA9539_H_

#include "misc.h"
#include "IIC_IO.h"
/*****************************************************************************
 @ typedefs
****************************************************************************/
typedef struct{
	/* config	*/
	IIC_IO_Dev_T iic;
	IIC_IO_Dev_T* pIIC;
	u8 devAddr;
}PCA9539_Rsrc_T;

/*
	when page write more than 64 bytes,
	pBuf must point to a static pointer
*/
typedef struct{
	PCA9539_Rsrc_T rsrc;
	//op
	void	(*WriteReg)(PCA9539_Rsrc_T*, u8 addr, const u8* dat, u8 len);
	void 	(*ReadReg)(PCA9539_Rsrc_T*, u8 addr, u8* dat, u8 len);
}PCA9539_Dev_T;

DEV_STATUS PCA9539_Setup1(
	PCA9539_Dev_T *pDev, 
	const PIN_T SCL, 
	const PIN_T SDA, 
	const u8 devAddr,
	u16 config,
	u16 initSta
);
DEV_STATUS PCA9539_Setup2(
	PCA9539_Dev_T *pDev, 
	IIC_IO_Dev_T* pIIC,  
	const u8 devAddr,
	u16 config,
	u16 initSta
);
#endif
