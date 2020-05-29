/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : STM32FLASHDISK.c
* Author             : Tiko Zhong
* Date First Issued  : 10.14,2016
* Description        : 
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "STM32FLASHDISK.h"
#include "myString.h"
#include "publicRsrc.h"
//#include "myuart_os.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//static u8 wrtBuf[FLASH_PAGE_SIZE];
/* Private functions ---------------------------------------------------------*/
static s8 pageErase(u32 addr, u16 count);
/**
  * @brief  Unlocks Flash for write access
  * @param  None
  * @retval None
  */
void FLASH_If_Init(void)
{
  /* Unlock the Program memory */
  HAL_FLASH_Unlock();

  /* Clear all FLASH flags */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR);
  /* Unlock the Program memory */
  HAL_FLASH_Lock();
}

/**
  * @brief  This function does an erase of all user flash area
  * @param  start: start of user flash area
  * @retval FLASHIF_OK : user flash area successfully erased
  *         FLASHIF_ERASEKO : error occurred
  */
uint32_t FLASH_If_Erase(uint32_t start)
{
  uint32_t NbrOfPages = 0;
  uint32_t PageError = 0;
  FLASH_EraseInitTypeDef pEraseInit;
  HAL_StatusTypeDef status = HAL_OK;

  /* Unlock the Flash to enable the flash control register access *************/ 
  HAL_FLASH_Unlock();

  /* Get the sector where start the user flash area */
  if (start < USER_FLASH_BANK1_END_ADDRESS)
  {
    NbrOfPages = ((USER_FLASH_BANK1_END_ADDRESS + 1) - start)/FLASH_PAGE_SIZE;
    pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
    pEraseInit.PageAddress = start;
    pEraseInit.Banks = FLASH_BANK_1;
    pEraseInit.NbPages = NbrOfPages;
    status = HAL_FLASHEx_Erase(&pEraseInit, &PageError);
#ifdef FLASH_BANK2_END	
    NbrOfPages = (USER_FLASH_BANK2_END_ADDRESS - USER_FLASH_BANK1_END_ADDRESS)/FLASH_PAGE_SIZE;
    pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
    pEraseInit.PageAddress = USER_FLASH_BANK2_START_ADDRESS;
    pEraseInit.Banks = FLASH_BANK_2;
    pEraseInit.NbPages = NbrOfPages;
    status = HAL_FLASHEx_Erase(&pEraseInit, &PageError);
  }
  else if (start < USER_FLASH_BANK2_END_ADDRESS)
  {
    NbrOfPages = ((USER_FLASH_BANK2_END_ADDRESS + 1) - start)/FLASH_PAGE_SIZE;
    pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
    pEraseInit.PageAddress = start;
    pEraseInit.Banks = FLASH_BANK_2;
    pEraseInit.NbPages = NbrOfPages;
    status = HAL_FLASHEx_Erase(&pEraseInit, &PageError);
#endif		
  }

  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock();

  if (status != HAL_OK)
  {
    /* Error occurred while page erase */
    return FLASHIF_ERASEKO;
  }

  return FLASHIF_OK;
}

/* Public functions ---------------------------------------------------------*/
/**
  * @brief  This function writes a data buffer in flash (data are 32-bit aligned).
  * @note   After writing data buffer, the flash content is checked.
  * @param  destination: start address for target location, in byte
  * @param  p_source: pointer on buffer with data to write
  * @param  length: length of data buffer (unit is 32-bit word)
  * @retval uint32_t 0: Data successfully written to Flash memory
  *         1: Error occurred while writing data in Flash memory
  *         2: Written Data in flash memory is different from expected one
  */
uint32_t FLASH_If_Write(uint32_t destination, uint32_t *p_source, uint32_t length)
{
  uint32_t i = 0;

  /* Unlock the Flash to enable the flash control register access *************/
  HAL_FLASH_Unlock();

  for (i = 0; (i < length) && (destination <= (USER_FLASH_BANK1_END_ADDRESS-4)); i++)
  {
    /* Device voltage range supposed to be [2.7V to 3.6V], the operation will
       be done by word */ 
    if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, destination, *(uint32_t*)(p_source+i)) == HAL_OK)      
    {
     /* Check the written value */
      if (*(uint32_t*)destination != *(uint32_t*)(p_source+i))
      {
        /* Flash content doesn't match SRAM content */
        return(FLASHIF_WRITINGCTRL_ERROR);
      }
      /* Increment FLASH destination address */
      destination += 4;
    }
    else
    {
      /* Error occurred while writing data in Flash memory */
      return (FLASHIF_WRITING_ERROR);
    }
  }

  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock();

  return (FLASHIF_OK);
}

/**
  * @brief  Returns the write protection status of application flash area.
  * @param  None
  * @retval If a sector in application area is write-protected returned value is a combinaison
            of the possible values : FLASHIF_PROTECTION_WRPENABLED, FLASHIF_PROTECTION_PCROPENABLED, ...
  *         If no sector is write-protected FLASHIF_PROTECTION_NONE is returned.
  */
uint32_t FLASH_If_GetWriteProtectionStatus(void)
{
  uint32_t ProtectedPAGE = FLASHIF_PROTECTION_NONE;
  FLASH_OBProgramInitTypeDef OptionsBytesStruct;

  /* Unlock the Flash to enable the flash control register access *************/
  HAL_FLASH_Unlock();

  /* Check if there are write protected sectors inside the user flash area ****/
  HAL_FLASHEx_OBGetConfig(&OptionsBytesStruct);

  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock();

  /* Get pages already write protected ****************************************/
  ProtectedPAGE = ~(OptionsBytesStruct.WRPPage) & FLASH_PAGE_TO_BE_PROTECTED;

  /* Check if desired pages are already write protected ***********************/
  if(ProtectedPAGE != 0)
  {
    /* Some sectors inside the user flash area are write protected */
    return FLASHIF_PROTECTION_WRPENABLED;
  }
  else
  { 
    /* No write protected sectors inside the user flash area */
    return FLASHIF_PROTECTION_NONE;
  }
}

/**
  * @brief  Configure the write protection status of user flash area.
  * @param  protectionstate : FLASHIF_WRP_DISABLE or FLASHIF_WRP_ENABLE the protection
  * @retval uint32_t FLASHIF_OK if change is applied.
  */
uint32_t FLASH_If_WriteProtectionConfig(uint32_t protectionstate)
{
  uint32_t ProtectedPAGE = 0x0;
  FLASH_OBProgramInitTypeDef config_new, config_old;
  HAL_StatusTypeDef result = HAL_OK;
  

  /* Get pages write protection status ****************************************/
  HAL_FLASHEx_OBGetConfig(&config_old);

  /* The parameter says whether we turn the protection on or off */
  config_new.WRPState = (protectionstate == FLASHIF_WRP_ENABLE ? OB_WRPSTATE_ENABLE : OB_WRPSTATE_DISABLE);

  /* We want to modify only the Write protection */
  config_new.OptionType = OPTIONBYTE_WRP;
  
  /* No read protection, keep BOR and reset settings */
  config_new.RDPLevel = OB_RDP_LEVEL_0;
  config_new.USERConfig = config_old.USERConfig;  
  /* Get pages already write protected ****************************************/
  ProtectedPAGE = config_old.WRPPage | FLASH_PAGE_TO_BE_PROTECTED;

  /* Unlock the Flash to enable the flash control register access *************/ 
  HAL_FLASH_Unlock();

  /* Unlock the Options Bytes *************************************************/
  HAL_FLASH_OB_Unlock();
  
  /* Erase all the option Bytes ***********************************************/
  result = HAL_FLASHEx_OBErase();
    
  if (result == HAL_OK)
  {
    config_new.WRPPage    = ProtectedPAGE;
    result = HAL_FLASHEx_OBProgram(&config_new);
  }
  
  return (result == HAL_OK ? FLASHIF_OK: FLASHIF_PROTECTION_ERRROR);
}		


//disk interface
/*******************************************************************************
* Function Name  : to get aver of 80% samples in middle scope 
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
s8 xRead (u8 *buf, uint32_t blk_addr, uint16_t blk_len){
	u32 i,j,k,addStart;

	addStart = STRG_PAGE_START + blk_addr*DISK_BLOCK_SIZE;
	for(i=0;i<blk_len;i++)
		for(j=0;j<DISK_BLOCK_SIZE;j++){
			k = addStart+i*DISK_BLOCK_SIZE+j;
			if(k <= STRG_PAGE_END)	buf[i*DISK_BLOCK_SIZE+j] = *(u8*)(k);
			else	return -1;
		}
	return 0;
}

static s8 pageErase(u32 addr, u16 count){
	FLASH_EraseInitTypeDef EraseInitStruct;
	u32 PAGEError;
	
	EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.PageAddress = addr;
	EraseInitStruct.Banks = FLASH_BANK_1;
	EraseInitStruct.NbPages     = count;
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)	return -1;

	return 0;
}

s8 xWrite(const u8 *buf, uint32_t blk_addr, uint16_t blk_len){
	u8 blkPerPage;
	u32 i,j,k,bufIndx,data32,currenPage;
	u32 from,end,pageFrom,pageEnd;
	
	from = (STRG_PAGE_START + blk_addr*DISK_BLOCK_SIZE);	
	end = from+blk_len*DISK_BLOCK_SIZE-1;
	if(end >= STRG_PAGE_END+FLASH_PAGE_SIZE)	return -10;

	pageFrom = from/FLASH_PAGE_SIZE;
	pageEnd = end/FLASH_PAGE_SIZE;
	blkPerPage = FLASH_PAGE_SIZE/DISK_BLOCK_SIZE;	
	bufIndx = 0;
	
  /* Unlock the Flash to enable the flash control register access *************/
	HAL_FLASH_Unlock();	
	for(i=pageFrom; i<=pageEnd; i++){
		currenPage = i*FLASH_PAGE_SIZE;
		// Copy this page data to swap area page, 2048 bytes/page
		if(pageErase(SWAP_PAGE_INDX, 1)<0)	return -2;	/* erase swap page */
		for(j=0; j<FLASH_PAGE_SIZE/4; j++){								/* Program swap page */
			data32 = *(u32*)(currenPage+j*4);
			if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, SWAP_PAGE_INDX+j*4, data32) == HAL_OK) {
				if (data32 != *(u32*)(SWAP_PAGE_INDX+j*4))	return -3;	/* Check the written value */
			}
			else	{	return -4;	}
		}
		// Erase current page
		if(pageErase(currenPage, 1)<0)	return -5;	/* erase current page */
		// Program current page (choose and program)
		for(j=0; j<blkPerPage; j++){
			if(bufIndx >= blk_len*DISK_BLOCK_SIZE)	break;
			if((currenPage+j*DISK_BLOCK_SIZE >= from) && (currenPage+j*DISK_BLOCK_SIZE <= end)){	/* data from buf */
				for(k=0; k<DISK_BLOCK_SIZE/4; k++){
					data32 = buf[bufIndx+3];	data32 <<= 8;
					data32 |= buf[bufIndx+2];	data32 <<= 8;
					data32 |= buf[bufIndx+1];	data32 <<= 8;
					data32 |= buf[bufIndx+0];	bufIndx += 4;
					if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, currenPage+j*DISK_BLOCK_SIZE+k*4, data32) == HAL_OK) {
						if (data32 != *(u32*)(currenPage+j*DISK_BLOCK_SIZE+k*4))	return -6;	/* Check the written value */
					}
					else	{	return -7;	}
				}
			}
			else{	/* data from swap page */
				for(k=0; k<DISK_BLOCK_SIZE/4; k++){
					data32 = *(u32*)(SWAP_PAGE_INDX+j*DISK_BLOCK_SIZE+k*4);
					if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, currenPage+j*DISK_BLOCK_SIZE+k*4, data32) == HAL_OK) {
						if (data32 != *(u32*)(currenPage+j*DISK_BLOCK_SIZE+k*4))	return -8;	/* Check the written value */
					}
					else	{	return -9;	}
				}
			}
		}
	}
  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock();
	return 0;
}

void PageErase(u32 addr, u16 count){
	HAL_FLASH_Unlock();
	pageErase(addr, count);
	HAL_FLASH_Lock();
}

void diskSpaceFormat(void){
//	FLASH_If_Erase(USB_STORAGE_ADDRESS);
	PageErase(STRG_PAGE_START, (STRG_PAGE_END-STRG_PAGE_START)/FLASH_PAGE_SIZE+1);
}

void appSpaceFormat(void){
	PageErase(APP_PAGE_START, (APP_PAGE_END-APP_PAGE_START)/FLASH_PAGE_SIZE+1);
}

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
