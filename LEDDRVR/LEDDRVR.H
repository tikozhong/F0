/**********************************************************
filename: leddrvr.h
**********************************************************/
#ifndef _LEDDRVR_H_
#define _LEDDRVR_H_

#include "publicRsrc.h"
#include "led1642.h"
#include "at24c256.h"

#define LED_CH	

/*****************************************************************************
 @ typedefs
****************************************************************************/
typedef struct{
	u8 name[DEV_NAME_LEN];
	/* config	*/
	LED1642_Dev_T LED1642[2];
	AT24C256C_Dev_T eeprom;
}LedDrvr_Rsrc_T;

typedef struct{
	LedDrvr_Rsrc_T rsrc;
	//led op group
	s8 (*SetBrightness)	(LedDrvr_Rsrc_T* pRsrc, u8 ch, u16 brightness);		//SET LED BRIGHTNESS
	void (*SetAllBrightness)	(LedDrvr_Rsrc_T* pRsrc, u16 brightness);		//SET LED BRIGHTNESS
	s8 (*GetBrightness)	(LedDrvr_Rsrc_T* pRsrc, u8 ch, u16* brightness);	//GET LED BRIGHTNESS
	void (*ToggleLed)	(LedDrvr_Rsrc_T* pRsrc);	//read a string to eeprom (end of null)
	void (*SetMaxCurrent)	(LedDrvr_Rsrc_T* pRsrc, u8 current);	//read a string to eeprom (end of null)
	s8 (*SetConfig)	(LedDrvr_Rsrc_T* pRsrc, u8 ch, u16 config);
	s8 (*GetConfig)	(LedDrvr_Rsrc_T* pRsrc, u8 ch, u16* config);
	void (*SetLedResolution)(LedDrvr_Rsrc_T* pRsrc, u8 res);
	
	//eeprom op group
	s8 (*ReadBrightnessCal)	(LedDrvr_Rsrc_T* pRsrc, u8 ch, u16* brightness);		//read brightness from eeprom
	s8 (*WriteBrightnessCal)	(LedDrvr_Rsrc_T* pRsrc, u8 ch, u16 brightness);		//write brightness to eeprom
	s8 (*WriteNumber)	(LedDrvr_Rsrc_T* pRsrc, u16 addr, u32 num);		//write a number to eeprom
	s8 (*ReadNumber)	(LedDrvr_Rsrc_T* pRsrc, u16 addr, u32* num);	//read a number to eeprom
	s8 (*WriteString)	(LedDrvr_Rsrc_T* pRsrc, u16 addr, const char* );	//write a string to eeprom (end of null)
	s8 (*ReadString)	(LedDrvr_Rsrc_T* pRsrc, u16 addr, char* );	//write a string to eeprom (end of null)

}LedDrvrDev_t;
//sizeof(Tm1638Dev_t) = ???

s8 ledDrvrDevSetup(
	LedDrvrDev_t *pDev, 
	const u8* NAME,
	const PIN_T	LCK1, const PIN_T SCK1, const PIN_T SDI1,
	const PIN_T LCK2, const PIN_T SCK2, const PIN_T SDI2,
	const PIN_T SCL, const PIN_T SDA
);

s8 ledCmd(LedDrvrDev_t *dev, const u8 *CMD, u8 *pRtn, u16 sz);
	
#endif
