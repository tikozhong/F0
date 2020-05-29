/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : eepromdisk.c
* Author             : Tiko Zhong
* Date First Issued  : 08/07/2016
* Description        : 
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "eepromdisk.h"
#include "myString.h"

#define MBR_BASE 	0
#define FAT1_BASE	1	
#define FAT2_BASE	2
#define FIL0_BASE	3
#define FIL1_BASE	4
#define FIL2_BASE	5
#define FIL3_BASE	6
#define DAT_BASE	7

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
s8 readFromBuf (EROMDISK_RSRC_T *pRsrc, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
s8 writeToBuf (EROMDISK_RSRC_T *pRsrc, const u8 *buf, uint32_t blk_addr, uint16_t blk_len);
/*******************************************************************************
* Function Name  : delay
* Description    : delay for n us. COUNT_OF_1US should be tuned in different plat
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
DEV_STATUS setupEromDisk(EROMDISK_DEV_T *pDev, const u16 CAP, const PIN_T SCL, const PIN_T SDA){	
	pDev->rsrc.blkNum = 40+((CAP-7*EROM_DISK_BLK_SZ)>>9);
	AT24C256C_Setup(&pDev->rsrc.eeprom, CAP, SCL, SDA);
	pDev->readFromBuf = readFromBuf;
	pDev->writeToBuf = writeToBuf;
	return DEV_SUCCESS;
}
/*******************************************************************************
* Function Name  : to get aver of 80% samples in middle scope 
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
s8 readFromBuf (EROMDISK_RSRC_T *pRsrc, u8 *buf, u32 blk_addr, u16 blk_len){
	u16 i,j;
	u32 addr = blk_addr, bufAddr = 0;
	AT24C256C_Dev_T *pEromDev = &pRsrc->eeprom;

	for(i=0; i<blk_len; i++){
		if(addr == 0)			{
			pEromDev->Sector256Read(&pEromDev->rsrc, MBR_BASE, &buf[bufAddr], 512);
		}
		else if(addr == 6)	{	//FAT1
			pEromDev->Sector256Read(&pEromDev->rsrc, FAT1_BASE<<1, &buf[bufAddr], 512);
			bufAddr += 512;
		}
		else if(addr == 7)	{	//FAT2
			pEromDev->Sector256Read(&pEromDev->rsrc, FAT2_BASE<<1, &buf[bufAddr], 512);
			bufAddr += 512;
		}
		else if(addr == 8)	{
			pEromDev->Sector256Read(&pEromDev->rsrc, FIL0_BASE<<1, &buf[bufAddr], 512);
			bufAddr += 512;
		}
		else if(addr == 9)	{
			pEromDev->Sector256Read(&pEromDev->rsrc, FIL1_BASE<<1, &buf[bufAddr], 512);
			bufAddr += 512;
		}
		else if(addr == 10)	{
			pEromDev->Sector256Read(&pEromDev->rsrc, FIL2_BASE<<1, &buf[bufAddr], 512);
			bufAddr += 512;
		}
		else if(addr == 11)	{
			pEromDev->Sector256Read(&pEromDev->rsrc, FIL3_BASE<<1, &buf[bufAddr], 512);
			bufAddr += 512;
		}
		else if(addr >= 40)	{
			pEromDev->Sector256Read(&pEromDev->rsrc, (addr-40+DAT_BASE)<<1, &buf[bufAddr], 512);
			bufAddr += 512;
		}
		else {	
			for(j=0;j<512;j++)	buf[bufAddr+j] = 0;
			bufAddr += 512;
		}
		addr++;
	}
	return DISK_OK;
}

s8 writeToBuf (	EROMDISK_RSRC_T *pRsrc, const u8 *buf, u32 blk_addr, u16 blk_len){
	u16 i;
	u32 addr = blk_addr, bufAddr = 0;
	AT24C256C_Dev_T *pEromDev = &pRsrc->eeprom;

	for(i=0; i<blk_len; i++){
		if(addr == 0)			{
			pEromDev->Sector256Write(&pEromDev->rsrc, MBR_BASE, &buf[bufAddr], 512);
		}
		else if(addr == 6)	{	//FAT1
			pEromDev->Sector256Write(&pEromDev->rsrc, FAT1_BASE<<1, &buf[bufAddr], 512);
			bufAddr += 512;
		}
		else if(addr == 7)	{	//FAT2
			pEromDev->Sector256Write(&pEromDev->rsrc, FAT2_BASE<<1, &buf[bufAddr], 512);
			bufAddr += 512;
		}
		else if(addr == 8)	{
			pEromDev->Sector256Write(&pEromDev->rsrc, FIL0_BASE<<1, &buf[bufAddr], 512);
			bufAddr += 512;
		}
		else if(addr == 9)	{
			pEromDev->Sector256Write(&pEromDev->rsrc, FIL1_BASE<<1, &buf[bufAddr], 512);
			bufAddr += 512;
		}
		else if(addr == 10)	{
			pEromDev->Sector256Write(&pEromDev->rsrc, FIL2_BASE<<1, &buf[bufAddr], 512);
			bufAddr += 512;
		}
		else if(addr == 11)	{
			pEromDev->Sector256Write(&pEromDev->rsrc, FIL3_BASE<<1, &buf[bufAddr], 512);
			bufAddr += 512;
		}
		else if(addr >= 40)	{
			pEromDev->Sector256Write(&pEromDev->rsrc, (addr-40+DAT_BASE)<<1, &buf[bufAddr], 512);
			bufAddr += 512;
		}
		else {	bufAddr += 512;	}
		addr++;
	}
	return DISK_OK;
}	









								
/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
