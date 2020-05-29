/**********************************************************
filename: eepromdisk.h
**********************************************************/
#ifndef _EEPROM_DISK_H_
#define _EEPROM_DISK_H_

#include "misc.h"
#include "at24c256.h"

#define STORAGE_IN_EEPROM 

#define EROM_DISK_BLK_SZ	512

/****************************************************************************
 @ enums
****************************************************************************/

/*****************************************************************************
 @ typedefs
****************************************************************************/
typedef enum{
	DISK_OK 	= 0,
	DISK_ERR = 1,
} DISK_RTN;

typedef struct{
	u8 *name;
	AT24C256C_Dev_T eeprom;
	u16 blkNum;
}EROMDISK_RSRC_T;

typedef struct{	
	EROMDISK_RSRC_T rsrc;
	s8 (*readFromBuf) (EROMDISK_RSRC_T *pRsrc, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
	s8 (*writeToBuf) (EROMDISK_RSRC_T *pRsrc, const u8 *buf, uint32_t blk_addr, uint16_t blk_len);
} EROMDISK_DEV_T;

typedef struct{	
	u16 CAP; 
	PIN_T SCL; 
	PIN_T SDA;
} EROMDISK_SETUP_PARAM_T;

/* setup a device */
DEV_STATUS setupEromDisk(EROMDISK_DEV_T *pDev, const u16 CAP, const PIN_T SCL, const PIN_T SDA);

#endif
