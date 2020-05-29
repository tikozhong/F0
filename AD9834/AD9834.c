/**********************************************************
filename: ad9834.c
**********************************************************/

/************************包含头文件***********************/
#include "ad9834.h"
#include "string.h"
#include "gpiodecal.h"

#define MHz_IN_mHz	(1*1000*1000*1000)
#define FULL_SCALE	BIT(28)

/**********************************************************
 Private function
**********************************************************/
static void ad9834WriteReg(AD9834_Rsrc_T* pRsrc, u16 reg);
static void ad9834Sine(AD9834_Rsrc_T* pRsrc, u32 frq_mHz);
static void ad9834Triangle(AD9834_Rsrc_T* pRsrc, u32 frq_mHz);
static void ad9834Rename(AD9834_Rsrc_T* pRsrc, const char* NEW);
static void ad9834ScanSineWave(AD9834_Rsrc_T* pRsrc, u32 startFrq, u32 endFrq, s32 stepFrq, u16 stepTim);
static void ad9834ScanTriangleWave(AD9834_Rsrc_T* pRsrc, u32 startFrq, u32 endFrq, s32 stepFrq, u16 stepTim);
static void ad9834Polling(AD9834_Rsrc_T* pRsrc, u8 tick);
static void ad9834Powerdown(AD9834_Rsrc_T* pRsrc);
//static void ad9834Wakeup(AD9834_Rsrc_T* pRsrc);
static void ad9834WriteNxtFrq(AD9834_Rsrc_T* pRsrc, u32 frq_mHz);

/**********************************************************
 Public function
**********************************************************/
s8 AD9834DevSetup(
	AD9834_Dev_T *pDev, 
	PIN_T SCLK,
	PIN_T SDATA,
	PIN_T FSYNC,
	PIN_T RESET,
	u8 MCLK_MHZ,
	void (*print)(const char* FORMAT_ORG, ...)
){
	AD9834_Rsrc_T* pRsrc = &pDev->rsrc;
	
	pRsrc->FSYNC = FSYNC;
	pRsrc->SCLK = SCLK;
	pRsrc->SDAT = SDATA;
	pRsrc->RESET = RESET;
	u32 mHz=0,mod=0;
	u8 i;
	//计算 uHz/LSB
	for(i=0;i<MCLK_MHZ;i++){	//per 1MHZ
		mHz += MHz_IN_mHz/FULL_SCALE;
		mod += MHz_IN_mHz%FULL_SCALE;
		mHz += mod/FULL_SCALE;
		mod = mod%FULL_SCALE;
	}
	pRsrc->uHzPerLsb = mHz*1000 + mod*1000/FULL_SCALE;	//MCLK_MHZ*1000000/BIT(28);
	as_OUTPUT_PP_NOPULL_LOW(SCLK);
	as_OUTPUT_PP_NOPULL_LOW(SDATA);
	as_OUTPUT_PP_NOPULL_HIGH(FSYNC);
	as_OUTPUT_PP_PULLDWN_LOW(RESET);

	pDev->WriteReg = ad9834WriteReg;
	pDev->TriangleWave = ad9834Triangle;
	pDev->SineWave = ad9834Sine;
	pDev->ScanSineWave = ad9834ScanSineWave;
	pDev->ScanTriangleWave = ad9834ScanTriangleWave;
	pDev->Polling = ad9834Polling;
	pDev->Rename = ad9834Rename;
	pDev->Powerdown = ad9834Powerdown;
	
	pRsrc->frqRegSel = 0;
	pRsrc->phsRegSel = 0;
	pRsrc->squ = 0;
	pRsrc->errCode = 0;
	pRsrc->print = print;
	
	HAL_GPIO_WritePin(pRsrc->RESET.GPIOx, pRsrc->RESET.GPIO_Pin, GPIO_PIN_SET);
	ad9834Powerdown(pRsrc);
	
	return 0;
}

/*******************************************************************************
* Function Name  : lower operation
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void ad9834WriteReg(AD9834_Rsrc_T* pRsrc, u16 reg){
	u8 i;
	u16 buf = reg;
	HAL_GPIO_WritePin(pRsrc->RESET.GPIOx, pRsrc->RESET.GPIO_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(pRsrc->SCLK.GPIOx, pRsrc->SCLK.GPIO_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(pRsrc->FSYNC.GPIOx, pRsrc->FSYNC.GPIO_Pin, GPIO_PIN_RESET);
	for(i=0;i<16;i++){
		HAL_GPIO_WritePin(pRsrc->SDAT.GPIOx, pRsrc->SDAT.GPIO_Pin, GPIO_PIN_RESET);
		if(buf & 0x8000)	HAL_GPIO_WritePin(pRsrc->SDAT.GPIOx, pRsrc->SDAT.GPIO_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(pRsrc->SCLK.GPIOx, pRsrc->SCLK.GPIO_Pin, GPIO_PIN_RESET);
		buf <<=1;
		HAL_GPIO_WritePin(pRsrc->SCLK.GPIOx, pRsrc->SCLK.GPIO_Pin, GPIO_PIN_SET);
	}
	HAL_GPIO_WritePin(pRsrc->FSYNC.GPIOx, pRsrc->FSYNC.GPIO_Pin, GPIO_PIN_SET);
}

static void ad9834WriteNxtFrq(AD9834_Rsrc_T* pRsrc, u32 frq_mHz){
	u16 reg = 0, regSel;
	u32 max = 0xffffffff/1000;
	u32 frq, count = 0;

	if(pRsrc->frqRegSel == 0){
		regSel = 0x8000;
		pRsrc->frqRegSel = 1;
	}
	else{
		regSel = 0x4000;
		pRsrc->frqRegSel = 0;
	}
	
	for(frq = frq_mHz; 1;){
		if(frq <= max){
			count += frq*1000/pRsrc->uHzPerLsb;
			break;
		}
		frq -= max;
		count += max*1000/pRsrc->uHzPerLsb;
	}
		
	//set frq0.lsb
	reg = 0;
	ad9834WriteReg(pRsrc, reg);	
	reg = (count&0x3fff) | regSel;
	ad9834WriteReg(pRsrc, reg);
	
	//set frq0.msb
	reg = BIT(12);
	ad9834WriteReg(pRsrc, reg);
	reg = ((count>>14)&0x3fff) | regSel;
	ad9834WriteReg(pRsrc, reg);
}

static void ad9834Sine(AD9834_Rsrc_T* pRsrc, u32 frq_mHz){
	ad9834WriteNxtFrq(pRsrc, frq_mHz);
	if(pRsrc->frqRegSel == 0){
		ad9834WriteReg(pRsrc, 0);	
	}
	else{
		ad9834WriteReg(pRsrc, BIT(11));
	}
	
}

static void ad9834Triangle(AD9834_Rsrc_T* pRsrc, u32 frq_mHz){
	ad9834WriteNxtFrq(pRsrc, frq_mHz);
	if(pRsrc->frqRegSel == 0){
		ad9834WriteReg(pRsrc, BIT(1));	
	}
	else{
		ad9834WriteReg(pRsrc, BIT(11)|BIT(1));	
	}
}

static void ad9834ScanSineWave(AD9834_Rsrc_T* pRsrc, u32 startFrq, u32 endFrq, s32 stepFrq, u16 stepTim){
	pRsrc->curFrq = startFrq;
	pRsrc->endFrq = endFrq;
	pRsrc->stepFrq = stepFrq;
	pRsrc->stepTime = stepTim;
	pRsrc->squ = 1;
	pRsrc->tmr = 0;
	pRsrc->tick = 0;
	ad9834Sine(pRsrc, startFrq);
	//pRsrc->print("start ad9834ScanSineWave...\n");
}

static void ad9834ScanTriangleWave(AD9834_Rsrc_T* pRsrc, u32 startFrq, u32 endFrq, s32 stepFrq, u16 stepTim){
	pRsrc->curFrq = startFrq;
	pRsrc->endFrq = endFrq;
	pRsrc->stepFrq = stepFrq;
	pRsrc->stepTime = stepTim;
	pRsrc->squ = 2;
	pRsrc->tmr = 0;
	pRsrc->tick = 0;
	ad9834Sine(pRsrc, startFrq);
	//pRsrc->print("start ad9834ScanTriangleWave...\n");
}

static void ad9834Rename(AD9834_Rsrc_T* pRsrc, const char* NEW){
	memset(pRsrc->name, 0, DEV_NAME_LEN);
	strcpy(pRsrc->name, NEW);
}

static void ad9834Polling(AD9834_Rsrc_T* pRsrc, u8 tick){
	pRsrc->tick += tick;
	if(pRsrc->tick >= 4){
		//pRsrc->print("squ:%d\n", pRsrc->squ);
		pRsrc->tick = 0;
		switch(pRsrc->squ){
			//scan sine process
			case 1:
				//increase case
				if(pRsrc->stepFrq >=0 && pRsrc->curFrq >= pRsrc->endFrq){
					pRsrc->squ = 0;
					ad9834Powerdown(pRsrc);
					//if(pRsrc->print) pRsrc->print("+msg@%s.scancomplete()\r\n",pRsrc->name);
					break;
				}
				//decrease case
				else if(pRsrc->stepFrq <0 && pRsrc->curFrq <= pRsrc->endFrq){
					pRsrc->squ = 0;
					ad9834Powerdown(pRsrc);
					if(pRsrc->print) pRsrc->print("+msg@%s.scancomplete()\r\n",pRsrc->name);
					break;
				}
				pRsrc->tmr += 4;
				if(pRsrc->tmr >= pRsrc->stepTime){
					pRsrc->curFrq += pRsrc->stepFrq;
					ad9834Sine(pRsrc, pRsrc->curFrq);
					pRsrc->tmr = 0;
				}
				break;

			default:
				break;
		}
	}
}

static void ad9834Powerdown(AD9834_Rsrc_T* pRsrc){
	ad9834WriteReg(pRsrc, BIT(6));
}

//static void ad9834Wakeup(AD9834_Rsrc_T* pRsrc){
//	ad9834WriteReg(pRsrc, BIT(0));
//}

/**********************************************************
 == THE END ==
**********************************************************/
