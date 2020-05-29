/**********************************************************
filename: PCA9539.c
**********************************************************/

/************************包含头文件***************************************************/
#include "PCA9539.h"

/**********************************************************
 Private function
**********************************************************/
static void	pca9539WriteReg(PCA9539_Rsrc_T*, u8 addr, const u8* dat, u8 len);
static void pca9539ReadReg(PCA9539_Rsrc_T*, u8 addr, u8* dat, u8 len);
	
/**********************************************************
 Public function
**********************************************************/
DEV_STATUS PCA9539_Setup1(
	PCA9539_Dev_T *pDev, 
	const PIN_T SCL, 
	const PIN_T SDA, 
	const u8 devAddr,
	u16 config,
	u16 initSta
){
	PCA9539_Rsrc_T* pRsrc = &pDev->rsrc;
	IIC_IO_Setup(&pRsrc->iic, SCL, SDA, miscDelayMs256);
	return (PCA9539_Setup2(pDev, &pRsrc->iic, devAddr, config, initSta));
}

DEV_STATUS PCA9539_Setup2(
	PCA9539_Dev_T *pDev, 
	IIC_IO_Dev_T* pIIC,  
	const u8 devAddr,
	u16 config,
	u16 initSta
){
	PCA9539_Rsrc_T* pRsrc = &pDev->rsrc;
	pRsrc->pIIC = pIIC;
	pRsrc->devAddr = devAddr&0x03;
	pDev->WriteReg = pca9539WriteReg;
	pDev->ReadReg = pca9539ReadReg;
	//config
	pca9539WriteReg(pRsrc, 0x06, (u8*)&config, 2);
	//initial output
	pca9539WriteReg(pRsrc, 0x02, (u8*)&initSta, 2);
	return DEV_SUCCESS;
}

/**********************************************************
 Public function
**********************************************************/
static void	pca9539WriteReg(PCA9539_Rsrc_T* pRsrc, u8 regAddr, const u8* dat, u8 len){
	u8 i,tmp;
	IIC_IO_Dev_T* iicDev = pRsrc->pIIC;
	IIC_IO_Rsrc_T *iicRsrc = &iicDev->rsrc;
	//start
	iicDev->Start(iicRsrc);
	//slave addr
	i = 0xe8 | (pRsrc->devAddr<<1);
	iicDev->WriteByte(iicRsrc, i);
	if(iicDev->WaitAck(iicRsrc)==1){	iicDev->Stop(iicRsrc);	return;}
	//command byte
	iicDev->WriteByte(iicRsrc, regAddr);
	if(iicDev->WaitAck(iicRsrc)==1){	iicDev->Stop(iicRsrc);	return;}
	for(i=0;i<len;i++){
		tmp = dat[i];
		iicDev->WriteByte(iicRsrc, tmp);
		if(iicDev->WaitAck(iicRsrc)==1){	iicDev->Stop(iicRsrc);	return;}
	}	
	//stop
	iicDev->Stop(iicRsrc);
}

static void pca9539ReadReg(PCA9539_Rsrc_T* pRsrc, u8 regAddr, u8* dat, u8 len){
	u8 i;
	IIC_IO_Dev_T* iicDev = pRsrc->pIIC;
	IIC_IO_Rsrc_T *iicRsrc = &iicDev->rsrc;
	//start
	iicDev->Start(iicRsrc);
	//slave addr, write, write
	i = 0xe8 | (pRsrc->devAddr<<1);
	iicDev->WriteByte(iicRsrc, i);
	if(iicDev->WaitAck(iicRsrc)==1){	iicDev->Stop(iicRsrc);	return;}
	//command byte
	iicDev->WriteByte(iicRsrc, regAddr);
	if(iicDev->WaitAck(iicRsrc)==1){	iicDev->Stop(iicRsrc);	return;}
	//restart
	iicDev->Restart(iicRsrc);
	//slave addr, write, read
	i = 0xe9 | (pRsrc->devAddr<<1);
	iicDev->WriteByte(iicRsrc, i);
	if(iicDev->WaitAck(iicRsrc)==1){	iicDev->Stop(iicRsrc);	return;}
	for(i=0;i<len;i++){
		dat[i] = iicDev->ReadByte(iicRsrc);
		if(i==len-1)	iicDev->WriteNoAck(iicRsrc);
		else	iicDev->WriteAck(iicRsrc);		
	}	
	//stop
	iicDev->Stop(iicRsrc);
}

/**********************************************************
 == THE END ==
**********************************************************/
