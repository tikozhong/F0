/**********************************************************
filename: leddrvr.c
**********************************************************/

/*********************************************************/
#include "leddrvr.h"

//define brightness addr and length
#define	BRIGHTNESS_ADDR_BASE	0x0000
#define	BRIGHTNESS_SZ	(24*2)
//define sn addr and length
#define	SN_ADDR_BASE	(BRIGHTNESS_ADDR_BASE + BRIGHTNESS_SZ)
#define	SN_SZ	(16)
//define fw addr and length
#define	FW_ADDR_BASE	(SN_ADDR_BASE + SN_SZ)
#define	FW_SZ	(16)
//define num addr and length
#define	NUM_ADDR_BASE	(FW_ADDR_BASE + FW_SZ)
#define	NUM_SZ	(944)
//define string addr and length
#define	STR_ADDR_BASE	(NUM_ADDR_BASE + NUM_SZ)
#define	STR_SZ	(2048)
//define num addr and length
#define	USR_ADDR_BASE	(STR_ADDR_BASE + STR_SZ)

const u16 BRIGHTNESS_EEPROM_ADDR[24] = {
	BRIGHTNESS_ADDR_BASE+0*2,	//channel 0
	BRIGHTNESS_ADDR_BASE+1*2,	//channel 1
	BRIGHTNESS_ADDR_BASE+2*2,	//channel 2
	BRIGHTNESS_ADDR_BASE+3*2,	//channel 3
	BRIGHTNESS_ADDR_BASE+4*2,	//channel 4
	BRIGHTNESS_ADDR_BASE+5*2,	//channel 5
	BRIGHTNESS_ADDR_BASE+6*2,	//channel 6
	BRIGHTNESS_ADDR_BASE+7*2,	//channel 7
	BRIGHTNESS_ADDR_BASE+8*2,	//channel 8
	BRIGHTNESS_ADDR_BASE+9*2,	//channel 9
	BRIGHTNESS_ADDR_BASE+10*2,	//channel 10
	BRIGHTNESS_ADDR_BASE+11*2,	//channel 11
	BRIGHTNESS_ADDR_BASE+12*2,	//channel 12
	BRIGHTNESS_ADDR_BASE+13*2,	//channel 13
	BRIGHTNESS_ADDR_BASE+14*2,	//channel 14
	BRIGHTNESS_ADDR_BASE+15*2,	//channel 15
	BRIGHTNESS_ADDR_BASE+16*2,	//channel 16
	BRIGHTNESS_ADDR_BASE+17*2,	//channel 17
	BRIGHTNESS_ADDR_BASE+18*2,	//channel 18
	BRIGHTNESS_ADDR_BASE+19*2,	//channel 19
	BRIGHTNESS_ADDR_BASE+20*2,	//channel 20
	BRIGHTNESS_ADDR_BASE+21*2,	//channel 21
	BRIGHTNESS_ADDR_BASE+22*2,	//channel 22
	BRIGHTNESS_ADDR_BASE+23*2,	//channel 23
};

/* Private typedef --------------------------------------*/
/* Private define ---------------------------------------*/
/* Private macro ----------------------------------------*/
/* Private variables ------------------------------------*/
/* Private function prototypes --------------------------*/
static s8 setBrightness	(LedDrvr_Rsrc_T* pRsrc, u8 ch, u16 brightness);
static void setAllBrightness	(LedDrvr_Rsrc_T* pRsrc, u16 brightness);
static s8 getBrightness	(LedDrvr_Rsrc_T* pRsrc, u8 ch, u16* brightness);	//GET LED BRIGHTNESS
static s8 writeBrightnessToEeprom	(LedDrvr_Rsrc_T* pRsrc, u8 ch, u16 brightness);		//write brightness to eeprom
static s8 readBrightnessFromEeprom		(LedDrvr_Rsrc_T* pRsrc, u8 ch, u16* brightness);		//read brightness to eeprom
static s8 writeNumber	(LedDrvr_Rsrc_T* pRsrc, u16 addr, u32 num);		//write a number to eeprom
static s8 readNumber	(LedDrvr_Rsrc_T* pRsrc, u16 addr, u32* num);	//read a number to eeprom
static s8 writeString	(LedDrvr_Rsrc_T* pRsrc, u16 addr, const char *pStr);	//write a string to eeprom (end of null)
static s8 readString	(LedDrvr_Rsrc_T* pRsrc, u16 addr, char* str);					//read a string to eeprom (end of null)
static void toggleLed	(LedDrvr_Rsrc_T* pRsrc);
static s8 setConfig(LedDrvr_Rsrc_T* pRsrc, u8 ch, u16 config);
static s8 getConfig(LedDrvr_Rsrc_T* pRsrc, u8 ch, u16* config);
static void setLedResolution(LedDrvr_Rsrc_T* pRsrc, u8 res);

static s8 fetchNum(const u8 *NUMSTR, u32* dat);

//static void setVolt_mV	(MCP4822Rsrc_T* pRsrc, u8 ch, u16 mv);
//static void setVolt_500uV	(MCP4822Rsrc_T* pRsrc, u8 ch, u16 _500uV);
/**********************************************************
 Public function
**********************************************************/
s8 ledDrvrDevSetup(
	LedDrvrDev_t *pDev, 
	const u8* NAME,
	const PIN_T	LCK1, const PIN_T SCK1, const PIN_T SDI1,
	const PIN_T LCK2, const PIN_T SCK2, const PIN_T SDI2,
	const PIN_T SCL, const PIN_T SDA
){
	u8 i,buff[2] = {0xff};
	LedDrvr_Rsrc_T* pRsrc = &pDev->rsrc;
	//LED1642 SETUP
	LED1642_Setup(&pRsrc->LED1642[0], LCK1, SCK1, SDI1);
	LED1642_Setup(&pRsrc->LED1642[1], LCK2, SCK2, SDI2);
	//setup eeprom
	AT24C256C_Setup(&pRsrc->eeprom, NULL, CAPACITY_AT24C256C, PAGESZ_AT24256C, SCL, SDA);
	//methods
	pDev->ReadBrightnessCal = readBrightnessFromEeprom;
	pDev->WriteBrightnessCal = writeBrightnessToEeprom;
	pDev->WriteNumber = writeNumber;
	pDev->ReadNumber = readNumber;
	pDev->WriteString = writeString;
	pDev->ReadString = readString;
	
	pDev->SetBrightness = setBrightness;
	pDev->SetAllBrightness = setAllBrightness;
	pDev->GetBrightness = getBrightness;
	
	
	pDev->ToggleLed = toggleLed;
	pDev->GetConfig = getConfig;
	pDev->SetConfig = setConfig;
	pDev->SetLedResolution = setLedResolution;
	//initial
	pRsrc->LED1642[0].Switch(&pRsrc->LED1642[0].rsrc);
	pRsrc->LED1642[1].Switch(&pRsrc->LED1642[1].rsrc);
	return 0;
}

//apply layer
/**********************************************************
* Function Name  : getBrightness
* Description    : 
* Input          : None
* Output         : None
* Return         : None
**********************************************************/
static s8 getBrightness	(LedDrvr_Rsrc_T* pRsrc, u8 ch, u16* brightness){	//GET LED BRIGHTNESS
	u8 chipIndx = ch>>4;
	u8 indx = ch & 0x0f;
	if(chipIndx >= 2)	return -1;
	return (pRsrc->LED1642[chipIndx].GetBrightness(&pRsrc->LED1642[chipIndx].rsrc, indx, brightness));
}

static s8 setBrightness	(LedDrvr_Rsrc_T* pRsrc, u8 ch, u16 brightness){
	u8 chipIndx = ch>>4;
	u8 indx = ch & 0x0f;
	if(chipIndx >= 2)	return -1;
	return (pRsrc->LED1642[chipIndx].SetBrightness(&pRsrc->LED1642[chipIndx].rsrc, indx, brightness));
}

static void setAllBrightness	(LedDrvr_Rsrc_T* pRsrc, u16 brightness){
	pRsrc->LED1642[0].SetAllBrightness(&pRsrc->LED1642[0].rsrc, brightness);
	pRsrc->LED1642[1].SetAllBrightness(&pRsrc->LED1642[1].rsrc, brightness);
}

static s8 writeBrightnessToEeprom	(LedDrvr_Rsrc_T* pRsrc, u8 ch, u16 brightness){		//write brightness from eeprom
	if(ch>=24)	return -1;
	pRsrc->eeprom.Write(&pRsrc->eeprom.rsrc, BRIGHTNESS_EEPROM_ADDR[ch], (u8*)(&brightness), 2);
	return 0;
}

static s8 readBrightnessFromEeprom		(LedDrvr_Rsrc_T* pRsrc, u8 ch, u16* brightness){		//read brightness to eeprom
	if(ch>=24)	return -1;
	pRsrc->eeprom.Read(&pRsrc->eeprom.rsrc, BRIGHTNESS_EEPROM_ADDR[ch], (u8*)(brightness), 2);
	return 0;
}

//0 .. (NUM_SZ/4) 
static s8 writeNumber	(LedDrvr_Rsrc_T* pRsrc, u16 indx, u32 num){		//write a number to eeprom
	pRsrc->eeprom.Write(&pRsrc->eeprom.rsrc, indx/4, (u8*)(&num), 4);
	return 0;
}

//0 .. (NUM_SZ/4) 
static s8 readNumber	(LedDrvr_Rsrc_T* pRsrc, u16 indx, u32* num){	//read a number to eeprom
	pRsrc->eeprom.Read(&pRsrc->eeprom.rsrc, indx/4, (u8*)(num), 4);
	return 0;
}

static s8 writeString	(LedDrvr_Rsrc_T* pRsrc, u16 addr, const char *pStr){	//write a string to eeprom (end of null)
	return 0;
}

static s8 readString	(LedDrvr_Rsrc_T* pRsrc, u16 addr, char *str){				//read a string to eeprom (end of null)

	return 0;
}

static void toggleLed	(LedDrvr_Rsrc_T* pRsrc){
	pRsrc->LED1642[0].Switch(&pRsrc->LED1642[0].rsrc);
	pRsrc->LED1642[1].Switch(&pRsrc->LED1642[1].rsrc);
}


static s8 setConfig(LedDrvr_Rsrc_T* pRsrc, u8 ch, u16 config){
	if(ch >= 2)	return -1;
	pRsrc->LED1642[ch].SetConfig(&pRsrc->LED1642[ch].rsrc, config);
	return 0;
}


static s8 getConfig(LedDrvr_Rsrc_T* pRsrc, u8 ch, u16* config){
	if(ch >= 2)	return -1;
	pRsrc->LED1642[ch].GetConfig(&pRsrc->LED1642[ch].rsrc, config);
	return 0;
}


static void setLedResolution(LedDrvr_Rsrc_T* pRsrc, u8 res){
	pRsrc->LED1642[0].SetResolution(&pRsrc->LED1642[0].rsrc, res);
	pRsrc->LED1642[1].SetResolution(&pRsrc->LED1642[1].rsrc, res);
}


//ECHO ON                              - Turn Echo on
//ECHO OFF                             - Turn Echo off

//WRITE M LED [CHAN] [LED] [DRIVE VAL] - Set LED Drive Value
//WRITE A LED [CHAN] [LED] [STATE]     - Enable/Disable LED with Calibrated Value

//WRITE CAL [CHAN] [LED] [DRIVE VALUE] - Write Calibration value to EEPROM
//READ CAL [CHAN] [LED]                - Read Calibration value from EEPROM

//WRITE EEPROM [ADDRESS] [DATA]        - Write (8 bit) DATA to ADDRESS
//READ EEPROM [ADDRESS]                - Read (8 bit) DATA from ADDRESS

//WRITE W EEPROM [ADDRESS] [DATA]      - Write (16 bit) DATA to ADDRESS
//READ W EEPROM [ADDRESS]              - Read (16 bit) DATA from ADDRESS

//WRITE L EEPROM [ADDRESS] [DATA]      - Write (32 bit) DATA to ADDRESS
//READ L EEPROM [ADDRESS]              - Read (32 bit) DATA from ADDRESS

//WRITE SN [SN]                        - Write Serial Number
//READ SN                              - Read Serial Number
//GET FW VERSION                       - Read the Firmware Version
//CHECK CRC                            - Check the calculated CRC vs stored
//WRITE CRC                            - Write calculated CRC to EEPROM

s8 ledCmd(LedDrvrDev_t *dev, const u8 *CMD, u8 *pRtn, u16 sz){
	u8 str[64] = {0};
	u16 config;
	s16 indx;
	u32 val;
	u32 rnt[4];
	//WRITE M LED [CHAN] [LED] [DRIVE VAL] - Set LED Drive Value
	indx = indexOf(CMD, (const u8*)"write m led ");
	if(indx == 0){
		if(fetchNum(&CMD[indx+strlen("write m led ")], rnt)==3){
			dev->SetBrightness(&dev->rsrc, rnt[0]*17 + rnt[1], rnt[2]/16);
			strCpy(pRtn, sz, (const u8*)" OK>");
			return 1;
		}
		else return -1;
	}
	
//	indx = indexOf(CMD, (const u8*)"write a led ");
//	if(indx == 0){
//		return 1;
//	}
//	
//	indx = indexOf(CMD, (const u8*)"write cal ");
//	if(indx == 0){
//		return 1;
//	}
//	
//	indx = indexOf(CMD, (const u8*)"read cal ");
//	if(indx == 0){
//		return 1;
//	}
	
	indx = indexOf(CMD, (const u8*)"write l eeprom ");
	if(indx == 0){
		if(fetchNum(&CMD[indx+strlen("write l eeprom ")], rnt)==2){
			dev->WriteNumber(&dev->rsrc, rnt[0], rnt[1]);
			strCpy(pRtn, sz, (const u8*)" OK\n>");
			return 1;
		}
		else return -1;
	}
	
	indx = indexOf(CMD, (const u8*)"read l eeprom ");
	if(indx == 0){
		if(fetchNum(&CMD[indx+strlen("read l eeprom ")], rnt) == 1){
			val = 0;
			dev->ReadNumber(&dev->rsrc, rnt[0], &val);
			CONVERT.Uint32ToDecStrN(pRtn, val, 0);
			strcat((char*)pRtn, " OK\n>");
			return 1;
		}
		else return -1;
	}
	
	indx = indexOf(CMD, (const u8*)"write sn ");
	if(indx == 0){
		return 1;
	}
	
	indx = indexOf(CMD, (const u8*)"read sn ");
	if(indx == 0){
		return 1;
	}
	
	indx = indexOf(CMD, (const u8*)"read cal ");
	if(indx == 0){
		return 1;
	}
	
	return -1;
}


static s8 fetchNum(const u8 *NUMSTR, u32* dat){
	s8 indx;
	u8 indx0 = 0;
	u32 tmp;

	indx = CONVERT.StrToUint32(NUMSTR, &tmp);
	if(indx >= 0){
		dat[0] = tmp;
		indx0 += indx+1;
	}
	else return 0;
//	dev.PrintStr(&dev.Rsrc, &NUMSTR[indx0]);

	indx = CONVERT.StrToUint32(&NUMSTR[indx0], &tmp);
	if(indx >= 0){
		dat[1] = tmp;
		indx0 += indx+1;
	}
	else return 1;
//	dev.PrintStr(&dev.Rsrc, &NUMSTR[indx0]);
	
	indx = CONVERT.StrToUint32(&NUMSTR[indx0], &tmp);
	if(indx >= 0){
		dat[2] = tmp;
		indx0 += indx+1;
	}
	else return 2;
//	dev.PrintStr(&dev.Rsrc, &NUMSTR[indx0]);
	
	indx = CONVERT.StrToUint32(&NUMSTR[indx0], &tmp);
	if(indx >= 0){
		dat[3] = tmp;
		return 4;
	}
	else return 3;
}


/**********************************************************
 == THE END ==
**********************************************************/
