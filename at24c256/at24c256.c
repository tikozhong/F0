/**********************************************************
filename: at24c256.c
**********************************************************/

/************************包含头文件***************************************************/
#include "at24c256.h"
#include "misc.h"
#include "string.h"
#define TW_MS 5
#define MAX_PAGE_SZ	128

/**********************************************************
 Private function
**********************************************************/
static STATUS_AT24C256C readBuf(AT24C256C_Rsrc_T* pRsrc, u16 addr, u8 *pBuf, u16 nBytes);
static STATUS_AT24C256C writeBuf(AT24C256C_Rsrc_T* pRsrc, u16 addr, const u8 *pBuf, u16 nBytes);
static STATUS_AT24C256C writePage(AT24C256C_Rsrc_T* pRsrc, u16 addr, const u8 *pBuf, u8 nBytes);

static STATUS_AT24C256C writeStr(AT24C256C_Rsrc_T* pRsrc, u16 addr, const char *pDat);
static STATUS_AT24C256C readStr(AT24C256C_Rsrc_T* pRsrc, u16 addr, char *pDat, u8 len);	

//cell open	
static STATUS_AT24C256C cell16Write(AT24C256C_Rsrc_T* pRsrc, u8 cell, const u8* CONTENT);
static STATUS_AT24C256C cell16Read(AT24C256C_Rsrc_T* pRsrc, u8 cell, u8 *pDat, u16 nBytes);

//sector op
static STATUS_AT24C256C sectorErase(AT24C256C_Rsrc_T* pRsrc, u8 sectorAddr, u8 val);
static STATUS_AT24C256C entireErase	(AT24C256C_Rsrc_T* pRsrc, u8 val);
static STATUS_AT24C256C sector256Write	(AT24C256C_Rsrc_T* pRsrc, u8 sectorAddr, const u8 *pDat, u16 nBytes);
static STATUS_AT24C256C sector256Read	(AT24C256C_Rsrc_T* pRsrc, u8 sectorAddr, u8 *pDat, u16 nBytes);

/**********************************************************
 Public function
**********************************************************/
DEV_STATUS AT24C256C_Setup1(
	AT24C256C_Dev_T *pDev, 
	u8 addr, 
	const u32 cap, 
	const u8 pageSz, 
	const PIN_T scl, 
	const PIN_T sda, 
	void (*osWaitWrt) (u8 ms)	){
	IIC_IO_Setup(&pDev->rsrc.DevIIC, scl, sda);
	return (AT24C256C_Setup2(pDev, addr, cap, pageSz, &pDev->rsrc.DevIIC, osWaitWrt));
}
	
DEV_STATUS AT24C256C_Setup2(
	AT24C256C_Dev_T *pDev, 
	u8 addr, 
	const u32 cap, 
	const u8 pageSz, 
	IIC_IO_Dev_T* pIIC,  
	void (*osWaitWrt) (u8 ms)	
){
	pDev->rsrc.pIICDev = pIIC;
	pDev->rsrc.addr = addr;
	pDev->rsrc.capacity = cap;
	pDev->rsrc.pageSz = pageSz;
	pDev->rsrc.tick = HAL_GetTick();
	pDev->rsrc.osWaitWrt = osWaitWrt;
	//basic op
	pDev->Write = writeBuf;
	pDev->Read = readBuf;
	
	pDev->WriteStr = writeStr;
	pDev->ReadStr = readStr;
	//cell op
	pDev->Cell16Read = cell16Read;
	pDev->Cell16Write = cell16Write;	
	//sector op	(under test)
	pDev->Sector256Read = sector256Read;
	pDev->Sector256Write = sector256Write;
	pDev->EntireErase = entireErase;
	pDev->Sector256Erase = sectorErase;

	return DEV_SUCCESS;
}

/**********************************************************
 Public function
**********************************************************/
STATUS_AT24C256C sectorErase(AT24C256C_Rsrc_T* pRsrc, u8 Addr, u8 val){
	u16 addr = Addr, count;
	u8 buf[MAX_PAGE_SZ] = {val};
	
	addr <<= 8;
	for(count=0; count<256;){
		if(writePage(pRsrc, addr, buf, pRsrc->pageSz) == EEPROM_ERROR ) return EEPROM_ERROR;
		addr += pRsrc->pageSz;
		count += pRsrc->pageSz;
	}
	return EEPROM_SUCCESS;
}

/**********************************************************
 Public function
**********************************************************/
STATUS_AT24C256C entireErase	(AT24C256C_Rsrc_T* pRsrc, u8 val){
	u32 addr;
	for(addr=0; addr<pRsrc->capacity; addr+=64){
		if(writePage(pRsrc,addr,NULL,64) != EEPROM_SUCCESS)
			return EEPROM_ERROR;
	}
	return EEPROM_SUCCESS;
}

/**********************************************************
 Public function
**********************************************************/
STATUS_AT24C256C sector256Write	(AT24C256C_Rsrc_T* pRsrc, u8 sectorAddr, const u8 *pDat, u16 nBytes){
	u16 addr = sectorAddr;	addr <<= 8;
	if(writeBuf(pRsrc, addr, pDat, nBytes) == EEPROM_ERROR) return EEPROM_ERROR;
	return EEPROM_SUCCESS;
}

/**********************************************************
 Public function
**********************************************************/
STATUS_AT24C256C sector256Read	(AT24C256C_Rsrc_T* pRsrc, u8 sectorAddr, u8 *pDat, u16 nBytes){
	if(readBuf(pRsrc, sectorAddr<<8, pDat, nBytes) == EEPROM_ERROR) return EEPROM_ERROR;
	return EEPROM_SUCCESS;
}

static STATUS_AT24C256C cell16Write(AT24C256C_Rsrc_T* pRsrc, u8 cell, const u8* CONTENT){
	u16 addr = cell<<4;
	u8 len=0;
	
//	strCpy(buf,MAX_PAGE_SZ,CONTENT);
	len = strlen((const char*)CONTENT);
	return writeBuf(pRsrc, addr, CONTENT, len);
}

static STATUS_AT24C256C cell16Read(AT24C256C_Rsrc_T* pRsrc, u8 cell, u8 *pDat, u16 nBytes){
	u16 addr = cell<<4;
	return readBuf(pRsrc, addr, pDat, nBytes);
}

/**********************************************************
 read data
**********************************************************/
STATUS_AT24C256C readBuf(AT24C256C_Rsrc_T* pRsrc, u16 addr, u8 *pBuf, u16 nBytes){
	u16 thsAddr = addr, count;
	u8 tmpU8;
	u8 devAddr = DEVICE_ADDR | (pRsrc->addr<<1);

	IIC_IO_Dev_T *pI2cDev = pRsrc->pIICDev;
	IIC_IO_Rsrc_T *pI2cRsrc = &pI2cDev->rsrc;

	if(nBytes == 0 )	return EEPROM_ERROR;
	if(addr+nBytes-1 >= CAPACITY_AT24C256C)	return EEPROM_ERROR;

	//read data
	//make start
	pI2cDev->Start(pI2cRsrc);
	//device addr
	pI2cDev->WriteByte(pI2cRsrc, devAddr);
	if(pI2cDev->WaitAck(pI2cRsrc) == 1)	{
		pI2cDev->Stop(pI2cRsrc);
		return EEPROM_ERROR;
	}
	//word addr
	tmpU8 = thsAddr>>8;
	pI2cDev->WriteByte(pI2cRsrc,tmpU8);
	if(pI2cDev->WaitAck(pI2cRsrc) == 1)	{
		pI2cDev->Stop(pI2cRsrc);
		return EEPROM_ERROR;
	}
	tmpU8 = thsAddr&0x00ff;
	pI2cDev->WriteByte(pI2cRsrc,tmpU8);
	if(pI2cDev->WaitAck(pI2cRsrc) == 1)	{
		pI2cDev->Stop(pI2cRsrc);
		return EEPROM_ERROR;
	}

	//re-start
	//HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_RESET);		NOP();
	pI2cDev->Restart(pI2cRsrc);
	//device addr
	pI2cDev->WriteByte(pI2cRsrc, devAddr|0x01);
	if(pI2cDev->WaitAck(pI2cRsrc) == 1)	{
		pI2cDev->Stop(pI2cRsrc);
		return EEPROM_ERROR;
	}
	//read data
	for(count=0; count<nBytes; count++){
		pBuf[count] = pI2cDev->ReadByte(pI2cRsrc);
		if( count == nBytes-1)	pI2cDev->WriteNoAck(pI2cRsrc);	//last byte, be sent noAck
		else	pI2cDev->WriteAck(pI2cRsrc);
	}
	//stop
	pI2cDev->Stop(pI2cRsrc);

	return EEPROM_SUCCESS;

}

/**********************************************************
 write data
**********************************************************/
STATUS_AT24C256C writePage(AT24C256C_Rsrc_T* pRsrc, u16 addr, const u8 *pBuf, u8 nBytes){
	u16 thsAddr = addr;
	u8 i, tmpU8;
	IIC_IO_Dev_T *pI2cDev = pRsrc->pIICDev;
	IIC_IO_Rsrc_T *pI2cRsrc = &pI2cDev->rsrc;
	u8 devAddr = DEVICE_ADDR | (pRsrc->addr<<1);
	
	if((nBytes > pRsrc->pageSz) || nBytes==0 )	return EEPROM_ERROR01;

	//make start
	pI2cDev->Start(pI2cRsrc);
	//device addr
	pI2cDev->WriteByte(pI2cRsrc, devAddr);
	if(pI2cDev->WaitAck(pI2cRsrc) == 1)	{
		pI2cDev->Stop(pI2cRsrc);
		return EEPROM_ERROR02;
	}
	//word addr
	tmpU8 = thsAddr>>8;
	pI2cDev->WriteByte(pI2cRsrc, tmpU8);
	if(pI2cDev->WaitAck(pI2cRsrc) == 1){
		pI2cDev->Stop(pI2cRsrc);
		return EEPROM_ERROR03;
	}
	tmpU8 = thsAddr&0x00ff;
	pI2cDev->WriteByte(pI2cRsrc, tmpU8);
	if(pI2cDev->WaitAck(pI2cRsrc) == 1)	{
		pI2cDev->Stop(pI2cRsrc);
		return EEPROM_ERROR04;
	}
	//write data
	for(i=0;i<nBytes;i++){
		if(pBuf==NULL)	tmpU8 = 0;
		else	tmpU8 = pBuf[i];
		pI2cDev->WriteByte(pI2cRsrc, tmpU8);
		if(pI2cDev->WaitAck(pI2cRsrc) == 1)	{
			pI2cDev->Stop(pI2cRsrc);
			return EEPROM_ERROR05;
		}
	}	
	//make stop
	pI2cDev->Stop(pI2cRsrc);
	
	//wait about 5ms for write cycle time
	if(pRsrc->osWaitWrt)	pRsrc->osWaitWrt(5);
	else	HAL_Delay(5);
	
	return EEPROM_SUCCESS;
}

STATUS_AT24C256C writeBuf(AT24C256C_Rsrc_T* pRsrc, u16 addr, const u8 *pBuf, u16 nBytes){
	u16 thsAddr = addr, count;
	u8 tmpU8;

	//if more than capacity
	if(nBytes == 0 )	return EEPROM_ERROR;
	if(addr+nBytes-1 >= pRsrc->capacity)	return EEPROM_ERROR;

	for(count=0; (nBytes-count)>0; ){
		tmpU8 = thsAddr^0xffff;
		tmpU8 &= 0x3f; 
		tmpU8 += 1;
		if(nBytes-count < tmpU8)	tmpU8 = nBytes-count;
		if(writePage(pRsrc, thsAddr, &pBuf[count], tmpU8) == EEPROM_ERROR)	return EEPROM_ERROR;
		thsAddr += tmpU8;
		count += tmpU8;
	}
	return EEPROM_SUCCESS;
}


static STATUS_AT24C256C writeStr(AT24C256C_Rsrc_T* pRsrc, u16 addr, const char *pDat){
	return(writeBuf(pRsrc, addr, (const u8*)pDat, strlen(pDat)));
}

static STATUS_AT24C256C readStr(AT24C256C_Rsrc_T* pRsrc, u16 addr, char *pDat, u8 len){
	char str[64] = {0};
	if(readBuf(pRsrc, addr, (u8*)str, 64) != EEPROM_SUCCESS)	return EEPROM_ERROR;
	memcpy(str,pDat,len);
     //strCpy(pDat, len, str);
	return EEPROM_SUCCESS;
}


/**********************************************************
 == THE END ==
**********************************************************/
