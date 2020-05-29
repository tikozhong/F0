/**********************************************************
filename: STM32FLASHDISK.h
**********************************************************/
#ifndef _STM32_FLASH_DISK_H_
#define _STM32_FLASH_DISK_H_

#include "misc.h"
#include "STM32FLASH_512K_PAGE_DEFS.h"

/* Define the address from where user application will be loaded.
   Note: this area is reserved for the IAP code                  */
#define FLASH_PAGE_STEP         FLASH_PAGE_SIZE           /* Size of page : 2 Kbytes */
//#define APPLICATION_ADDRESS     (uint32_t)ADDR_FLASH_PAGE_16     	 /* Start user code address: ADDR_FLASH_PAGE_32 */
//#define USB_STORAGE_ADDRESS     (uint32_t)ADDR_FLASH_PAGE_127      /* Start user code address: ADDR_FLASH_PAGE_128 */
//#define SWAP_AREA_ADDRESS     	(uint32_t)ADDR_FLASH_PAGE_255      /* Start user code address: ADDR_FLASH_PAGE_255 */

#define DISK_BLOCK_SIZE 	512

#define BOOT_PAGE_START		(uint32_t)ADDR_FLASH_PAGE_00
#define BOOT_PAGE_END			(uint32_t)ADDR_FLASH_PAGE_15

#define APP_PAGE_START		(uint32_t)ADDR_FLASH_PAGE_16
#define APP_PAGE_END			(uint32_t)ADDR_FLASH_PAGE_126

#define SWAP_PAGE_INDX		(uint32_t)ADDR_FLASH_PAGE_127

#define STRG_PAGE_START		(uint32_t)ADDR_FLASH_PAGE_128
#define STRG_PAGE_END			(uint32_t)ADDR_FLASH_PAGE_255

/* Notable Flash addresses */
#define USER_FLASH_BANK1_START_ADDRESS  0x08000000
#define USER_FLASH_BANK1_END_ADDRESS    FLASH_BANK1_END
/* Define the user application size */

#ifdef FLASH_BANK2_END	
	#define USER_FLASH_BANK2_START_ADDRESS  0x08080000
	#define USER_FLASH_BANK2_END_ADDRESS    FLASH_BANK2_END
	#define USR_DISK_FLASH_START_ADDR				(USER_FLASH_BANK2_END_ADDRESS - USR_DISK_FLASH_CAPACITY)
	#define USR_DISK_FLASH_END_ADDR					(USER_FLASH_BANK2_END_ADDRESS)
#else
	#define USR_DISK_FLASH_START_ADDR	(USER_FLASH_BANK1_END_ADDRESS - USR_DISK_FLASH_CAPACITY+1)
	#define USR_DISK_FLASH_END_ADDR		(USER_FLASH_BANK1_END_ADDRESS)
#endif


/* Define bitmap representing user flash area that could be write protected (check restricted to pages 8-39). */
#define FLASH_PAGE_TO_BE_PROTECTED (OB_WRP_PAGES8TO9 | OB_WRP_PAGES10TO11 | OB_WRP_PAGES12TO13 | OB_WRP_PAGES14TO15 | \
                                    OB_WRP_PAGES16TO17 | OB_WRP_PAGES18TO19 | OB_WRP_PAGES20TO21 | OB_WRP_PAGES22TO23 | \
                                    OB_WRP_PAGES24TO25 | OB_WRP_PAGES26TO27 | OB_WRP_PAGES28TO29 | OB_WRP_PAGES30TO31 | \
                                    OB_WRP_PAGES32TO33 | OB_WRP_PAGES34TO35 | OB_WRP_PAGES36TO37 | OB_WRP_PAGES38TO39  )  


/* Exported macro ------------------------------------------------------------*/
/* ABSoulute value */
#define ABS_RETURN(x,y)               (((x) < (y)) ? (y) : (x))

/* Get the number of sectors from where the user program will be loaded */
#define FLASH_SECTOR_NUMBER           ((uint32_t)(ABS_RETURN(APPLICATION_ADDRESS,FLASH_START_BANK1))>>12)

/* Compute the mask to test if the Flash memory, where the user program will be
  loaded, is write protected */
#define FLASH_PROTECTED_SECTORS       (~(uint32_t)((1 << FLASH_SECTOR_NUMBER) - 1))


/****************************************************************************
 @ enums
****************************************************************************/
//typedef enum{
//	DISK_OK 	= 0,
//	DISK_ERR = 1,
//} DISK_RTN;

/* Error code */
enum 
{
  FLASHIF_OK = 0,
  FLASHIF_ERASEKO,
  FLASHIF_WRITINGCTRL_ERROR,
  FLASHIF_WRITING_ERROR,
  FLASHIF_PROTECTION_ERRROR
};

/* protection type */  
enum{
  FLASHIF_PROTECTION_NONE         = 0,
  FLASHIF_PROTECTION_PCROPENABLED = 0x1,
  FLASHIF_PROTECTION_WRPENABLED   = 0x2,
  FLASHIF_PROTECTION_RDPENABLED   = 0x4,
};

/* protection update */
enum {
	FLASHIF_WRP_ENABLE,
	FLASHIF_WRP_DISABLE
};

/*****************************************************************************
 @ typedefs
****************************************************************************/

/*****************************************************************************
 @ Exported functions 
****************************************************************************/
//flash interface
void FLASH_If_Init(void);
//uint32_t FLASH_If_Erase(uint32_t StartSector);
//uint32_t FLASH_If_GetWriteProtectionStatus(void);
//uint32_t FLASH_If_Write(uint32_t destination, uint32_t *p_source, uint32_t length);
//uint32_t FLASH_If_WriteProtectionConfig(uint32_t protectionstate);
//disk interface
s8 xRead (u8 *buf, uint32_t blk_addr, uint16_t blk_len);
s8 xWrite(const u8 *buf, uint32_t blk_addr, uint16_t blk_len);
//void PageErase(u32 addr, u16 count);
void diskSpaceFormat(void);
void appSpaceFormat(void);
#endif
