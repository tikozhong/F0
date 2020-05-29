/**********************************************************
filename: at24c256.h
**********************************************************/
#ifndef _AT24C256_H_
#define _AT24C256_H_

#include "misc.h"
#include "IIC_IO.h"

#define CAPACITY_AT24C04C 512
#define CAPACITY_AT24C128C 16384
#define CAPACITY_AT24C256C 32768

#define PAGESZ_AT24256C 64

#define DEVICE_ADDR 0xa0
/*****************************************************************************
 @ typedefs
****************************************************************************/
typedef enum {
	EEPROM_SUCCESS 	= 0,
	EEPROM_ERROR		= 1,
	EEPROM_BUSY 		= 2,
	EEPROM_WRITING 	= 3,
	EEPROM_ERROR01		= 4,
	EEPROM_ERROR02		= 5,
	EEPROM_ERROR03		= 6,
	EEPROM_ERROR04		= 7,
	EEPROM_ERROR05		= 8,
	EEPROM_ERROR06		= 9,
	EEPROM_ERROR07		= 10,
} STATUS_AT24C256C;

typedef struct{
	char name[DEV_NAME_LEN];
	/* config	*/
	IIC_IO_Dev_T DevIIC;
	IIC_IO_Dev_T* pIICDev;
	u32 capacity;
	u8 pageSz,addr;
	u32 tick;
	u8 writeCycTime;
	void (*osWaitWrt) (u8);
}AT24C256C_Rsrc_T;

/*
	when page write more than 64 bytes,
	pBuf must point to a static pointer
*/
typedef struct{
	AT24C256C_Rsrc_T rsrc;
	//basic
	STATUS_AT24C256C (*Write)(AT24C256C_Rsrc_T* pRsrc, u16 addr, const u8 *pDat, u16 nBytes);
	STATUS_AT24C256C (*Read)(AT24C256C_Rsrc_T* pRsrc, u16 addr, u8 *pDat, u16 nBytes);	
	//usr
	STATUS_AT24C256C (*WriteStr)(AT24C256C_Rsrc_T* pRsrc, u16 addr, const char *pDat);
	STATUS_AT24C256C (*ReadStr)(AT24C256C_Rsrc_T* pRsrc, u16 addr, char *pDat, u8 len);	
	//cell ops
	STATUS_AT24C256C (*Cell16Write)(AT24C256C_Rsrc_T* pRsrc, u8 cellIndx, const u8* CONTENT);
	STATUS_AT24C256C (*Cell16Read)(AT24C256C_Rsrc_T* pRsrc, u8 cellIndx, u8 *pDat, u16 nBytes);
	//sector ops (for fatfs)
	STATUS_AT24C256C (*Sector256Erase)(AT24C256C_Rsrc_T* pRsrc, u8 sectorAddr, u8 val);
	STATUS_AT24C256C (*EntireErase)	(AT24C256C_Rsrc_T* pRsrc, u8 val);
	STATUS_AT24C256C (*Sector256Write)	(AT24C256C_Rsrc_T* pRsrc, u8 sectorAddr, const u8 *pDat, u16 nBytes);
	STATUS_AT24C256C (*Sector256Read)	(AT24C256C_Rsrc_T* pRsrc, u8 sectorAddr, u8 *pDat, u16 nBytes);
	u32 errFlags;
}AT24C256C_Dev_T;

DEV_STATUS AT24C256C_Setup1(
	AT24C256C_Dev_T *pDev, 
	u8 addr,       //config by A0..2 
	const u32 cap, 
	const u8 pageSz, 
	const PIN_T scl, 
	const PIN_T sda, 
	void (*osWaitWrt) (u8)
);

DEV_STATUS AT24C256C_Setup2(
	AT24C256C_Dev_T *pDev, 
	u8 addr, 
	const u32 cap, 
	const u8 pageSz, 
	IIC_IO_Dev_T* pIIC, 
	void (*osWaitWrt) (u8)
);

#endif
