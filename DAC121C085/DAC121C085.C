/**********************************************************
filename: DAC121C085.c
**********************************************************/

/************************包含头文件***************************************************/
#include "DAC121C085.h"
#include "publicRsrc.h"

typedef enum
{
	csdNormal 	= 0,  /*!< IEXC1 -> IOUT1, IEXC2 -> IOUT2 */
	csdInverse	= 1, /*!< IEXC1 -> IOUT2, IEXC2 -> IOUT1 */
	csdBcsOut1 	= 2, /*!< Both current sources -> IOUT1. Current sources: 10uA or 210uA only */
	csdBcsOut2	= 3  /*!< Both current sources -> IOUT2. Current sources: 10uA or 210uA only */
} IEXCDIRSelect;

/**********************************************************
 Private function
**********************************************************/
static s8	DAC121C085_writeReg(DAC121C085_Rsrc_T*, u16 dat);
static void DAC121C085_powerDown(DAC121C085_Rsrc_T*, u8 mod);
static void DAC121C085_setVol(DAC121C085_Rsrc_T* pRsrc, u16 mv);
static u16 DAC121C085_getRef(DAC121C085_Rsrc_T* pRsrc);
static void DAC121C085_setRef(DAC121C085_Rsrc_T* pRsrc, u16 mv);
static s8 DAC121C085_IIC_WriteByte(DAC121C085_Rsrc_T* pRsrc, u8 byte);
/**********************************************************
 Public function
**********************************************************/
DEV_STATUS DAC121C085_Setup(
	DAC121C085_Dev_T *pDev, 
	const PIN_T SCL, 
	const PIN_T SDA, 
	const u8 devAddr,
	u16 refVol,
	u16 initVal
){
	DAC121C085_Rsrc_T* pRsrc = &pDev->rsrc;
//	IIC_IO_Setup(&pRsrc->iic, SCL, SDA);
	pRsrc->devAddr = devAddr;
	pRsrc->refVol = refVol;
	pDev->SetVol = DAC121C085_setVol;
	pDev->PowerDown = DAC121C085_powerDown;
	pDev->GetRef = DAC121C085_getRef;
	pDev->SetRef = DAC121C085_setRef;
	//initial
	pRsrc->scl = SCL;
	pRsrc->sda = SDA;
	PIN_OP.As_OUTPUT_OD_NOPULL_HIGH(SDA);
	PIN_OP.As_OUTPUT_OD_NOPULL_HIGH(SCL);
	if(initVal == 0){
		pRsrc->lastUpdate = 0;
		DAC121C085_powerDown(pRsrc,3);
	}
	else{
		DAC121C085_writeReg(pRsrc, initVal);
		pRsrc->lastUpdate = initVal;
	}
	return DEV_SUCCESS;
}

/**********************************************************
 Public function
**********************************************************/
static s8	DAC121C085_writeReg(DAC121C085_Rsrc_T* pRsrc, u16 dat){
	u8 byte;
	u16 tmp = dat;
	PIN_T scl,sda;
//	s8 rtn=0;
	sda = pRsrc->sda;
	scl = pRsrc->scl;

	//start
	PIN_OP.Write(sda, GPIO_PIN_RESET);	NOP();	NOP();	
	PIN_OP.Write(scl, GPIO_PIN_RESET);	
	
	//slave addr and wait ack
	byte = (pRsrc->devAddr<<1);
	DAC121C085_IIC_WriteByte(pRsrc, byte);
	//send byte MSB
	byte = (tmp>>8);
	DAC121C085_IIC_WriteByte(pRsrc, byte);
	//send byte LSB
	byte = (tmp&0xff);
	DAC121C085_IIC_WriteByte(pRsrc, byte);

	//stop
	HAL_GPIO_WritePin(sda.GPIOx, sda.GPIO_Pin, GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(scl.GPIOx, scl.GPIO_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(sda.GPIOx, sda.GPIO_Pin, GPIO_PIN_SET);	
	
	return 0;
}

//static void readReg(DAC121C085_Rsrc_T* pRsrc, u16* dat){
//	u8 i;
//	IIC_IO_Dev_T* iicDev = &pRsrc->iic;
//	IIC_IO_Rsrc_T *iicRsrc = &iicDev->rsrc;
//	//start
//	iicDev->Start(iicRsrc);
//	//slave addr
//	i = (pRsrc->devAddr<<1) & 0x01;	//read
//	iicDev->WriteByte(iicRsrc, i);
//	if(iicDev->WaitAck(iicRsrc)==1){	iicDev->Stop(iicRsrc);	return;}
//	//read byte
//	*dat = iicDev->ReadByte(iicRsrc);
//	iicDev->WriteAck(iicRsrc);
//	*dat <<= 8;
//	*dat |= iicDev->ReadByte(iicRsrc);
//	iicDev->WriteNoAck(iicRsrc);
//	//stop
//	iicDev->Stop(iicRsrc);
//}

static void DAC121C085_setVol(DAC121C085_Rsrc_T* pRsrc, u16 mv){
	u32 vol = mv*4096/pRsrc->refVol;
	if(vol & 0xfffff000)	vol = 0x00000fff;
	pRsrc->lastUpdate = vol;
	DAC121C085_writeReg(pRsrc, pRsrc->lastUpdate);
}

static void DAC121C085_setRef(DAC121C085_Rsrc_T* pRsrc, u16 mv){
	pRsrc->refVol = mv;
}

static u16 DAC121C085_getRef(DAC121C085_Rsrc_T* pRsrc){
	return pRsrc->refVol;
}

static void DAC121C085_powerDown(DAC121C085_Rsrc_T* pRsrc, u8 mod){
	if(mod == 1)			DAC121C085_writeReg(pRsrc, 0x1000|pRsrc->lastUpdate);	//pull down a 2k5 resister
	else if(mod == 2)	DAC121C085_writeReg(pRsrc, 0x2000|pRsrc->lastUpdate);	//pull down a 100k resister
	else							DAC121C085_writeReg(pRsrc, 0x3000|pRsrc->lastUpdate);	//High imdepance 
}

static s8 DAC121C085_IIC_WriteByte(DAC121C085_Rsrc_T* pRsrc, u8 dat){
	u8 i,byte;
	s8 rtn;
	PIN_T scl,sda;
	sda = pRsrc->sda;
	scl = pRsrc->scl;
	byte = dat;
	for(i=0;i<8;i++){	//write data
		if(byte & 0x80)	HAL_GPIO_WritePin(sda.GPIOx, sda.GPIO_Pin, GPIO_PIN_SET);
		else	HAL_GPIO_WritePin(sda.GPIOx, sda.GPIO_Pin, GPIO_PIN_RESET);	
		HAL_GPIO_WritePin(scl.GPIOx, scl.GPIO_Pin, GPIO_PIN_SET);
		byte <<= 1;
		HAL_GPIO_WritePin(scl.GPIOx, scl.GPIO_Pin, GPIO_PIN_RESET);
	}
	HAL_GPIO_WritePin(sda.GPIOx, sda.GPIO_Pin, GPIO_PIN_SET);	//release sda
	HAL_GPIO_WritePin(scl.GPIOx, scl.GPIO_Pin, GPIO_PIN_SET);
	for(i=0,rtn=-1;i<=5;i++){	//wait a ack from slave
		if(HAL_GPIO_ReadPin(sda.GPIOx, sda.GPIO_Pin) == GPIO_PIN_RESET){
			rtn = 0;
			break;
		}
	}
	HAL_GPIO_WritePin(scl.GPIOx, scl.GPIO_Pin, GPIO_PIN_RESET);
	if(rtn<0){	//if no ack, send a stop anyway
		HAL_GPIO_WritePin(sda.GPIOx, sda.GPIO_Pin, GPIO_PIN_RESET);	
		HAL_GPIO_WritePin(scl.GPIOx, scl.GPIO_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(sda.GPIOx, sda.GPIO_Pin, GPIO_PIN_SET);	
		return rtn;
	}
	return 0;
}

/**********************************************************
 == THE END ==
**********************************************************/
