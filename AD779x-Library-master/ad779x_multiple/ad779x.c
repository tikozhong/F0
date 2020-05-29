/**
  ******************************************************************************
  * @file    ad779x.c
  * @author  Khusainov Timur
  * @version 0.0.0.1
  * @date    10.10.2011
  * @brief   Analog Devices Sigma-Delta ADC AD7792/AD7793 driver (for multiple devices)
  ******************************************************************************
  * @attention
  * <h2><center>&copy; COPYRIGHT 2011 timypik@gmail.com </center></h2>
  ******************************************************************************
  */

#include "ad779x.h"
#include "publicRsrc.h"
#include "myuart.h"

static u32 AD779X_adcCode(tAD779X_Rsrc *pRsrc);
static u32 AD779X_adcVolt(tAD779X_Rsrc *pRsrc);

//static s32 AD779X_adcVolt(tAD779X_Rsrc *pRsrc, u8 ch, u8 gain, u8 pol, u8 fs);

static void AD779X_Init(tAD779X_Rsrc *pRsrc);
static void AD779X_Reset(tAD779X_Rsrc *pRsrc);
static void AD779X_WriteModeRegister(tAD779X_Rsrc *pRsrc, unsigned short Data);
static void AD779X_WriteConfigRegister(tAD779X_Rsrc *pRsrc, unsigned short Data);
static void AD779X_WriteIORegister(tAD779X_Rsrc *pRsrc, unsigned char Data);

static unsigned short AD779X_ReadModeRegister(tAD779X_Rsrc *pRsrc);
static unsigned short AD779X_ReadConfigRegister(tAD779X_Rsrc *pRsrc);
static unsigned short AD779X_ReadIORegister(tAD779X_Rsrc *pRsrc);

static void AD779X_HWDetect(tAD779X_Rsrc *pRsrc);
static unsigned char AD779X_GetStatus(tAD779X_Rsrc *pRsrc);
static unsigned char AD779X_CheckReadySW(tAD779X_Rsrc *pRsrc);

static void AD779X_SetMode(tAD779X_Rsrc *pRsrc, tAD779X_ModeSelect Mode);
static void AD779X_SetClkSource(tAD779X_Rsrc *pRsrc, tAD779X_ClkSourceSelect ClkSource);
static void AD779X_SetUpdateRate(tAD779X_Rsrc *pRsrc, tAD779X_FilterSelect UpdateRates);
static void AD779X_SetExCurrentValue(tAD779X_Rsrc *pRsrc, tAD779X_IEXCENSelect excValue);
static void AD779X_SetExCurrentDirection(tAD779X_Rsrc *pRsrc, tAD779X_IEXCDIRSelect excDirection);
static u32 AD779X_ReadDataRegister(tAD779X_Rsrc *pRsrc);
static void AD779X_StartZSCalibration(tAD779X_Rsrc *pRsrc);
static void AD779X_StartFSCalibration(tAD779X_Rsrc *pRsrc);
static void AD779X_SetBuffer(tAD779X_Rsrc *pRsrc, u8 en);
//static void AD779X_ContinueRead(tAD779X_Rsrc *pRsrc, u8 op);

static void AD779X_WaitUntilReadySW(tAD779X_Rsrc *pRsrc);
static void AD779X_WaitUntilReadyHW(tAD779X_Rsrc *pRsrc);

//lower driver
static void AD779X_CSControl(tAD779X_Rsrc *pRsrc, tAD779X_CSState sel);
static void AD779X_TxByte(tAD779X_Rsrc *pRsrc, u8 dat);
static u8 AD779X_RxByte(tAD779X_Rsrc *pRsrc);

/* Reset state of IO register */
//const tAD779X_IORegister gIOReg = tAD779X_IORegister_SET(csvDisable,csdNormal);
//{
//	.IEXCEN  = csvDisable, /*!< excitation currents disabled  */
//	.IEXCDIR = csdNormal,  /*!< default state */
//	.UNUSED  = 0           /*!< must be clear *
//};

/* Reset state of MODE register */
//const tAD779X_ModeRegister gModeReg = tAD779X_ModeRegister_SET(mdsContinuous,cssIntOut,fs16_7_65dB);
//{
//	.MODE = mdsIdle,     /*!< set Idle mode */
//	.CLKS = cssInt,      /*!< set internal clock source */
//	.FS   = fs16_7_65dB, /*!< 16.7Hz (65dB - 50/60Hz) */
//	.UNUSED0 = 0,        /*!< must be clear */
//	.UNUSED1 = 0         /*!< must be clear */
//};


void AD779X_Setup(
	tAD779X_Dev* pDev,	//device
	const PIN_T CS, 		//chip select pin
	const PIN_T SCK, 		//serial clok pin
	const PIN_T MOSI, 	//host out, device in
	const PIN_T MISO,		//host in, device out
	u8 clkSel,					//ADC clock select 0:interal clock, 1:internal clock and output, 2:external clock
	u16 refVol					//refvol select 0:ext. 1:interal 1.17v
){
	tAD779X_Rsrc* pRsrc = &pDev->rsrc;
	memset(pRsrc, 0, sizeof(tAD779X_Rsrc));
	pRsrc->CS = CS;
	pRsrc->SCK = SCK;
	pRsrc->MISO = MISO;
	pRsrc->MOSI = MOSI;
	//cofig IO
	PIN_OP.As_OUTPUT_PP_NOPULL_HIGH(CS);
	PIN_OP.As_OUTPUT_PP_NOPULL_HIGH(SCK);
	PIN_OP.As_OUTPUT_PP_NOPULL_HIGH(MOSI);
	PIN_OP.As_INPUT_NOPULL(MISO);
	pRsrc->refVol_mv = refVol;
	
	pRsrc->IOReg = 0x00;
	pRsrc->modeReg = 0x000a;
	pRsrc->configReg = 0x0090;

	pRsrc->modeReg = tAD779X_ModeRegister_SetClk(pRsrc->modeReg,clkSel);
	if(refVol == 1170)	pRsrc->configReg = tAD779X_ConfigRegister_SetRef(pRsrc->configReg, refInt);
	pRsrc->configReg = tAD779X_ConfigRegister_SetPol(pRsrc->configReg, ubUnipolar);	//unpolar
	
	print("mode:0x%x\n",pRsrc->modeReg);
	print("config:0x%x\n",pRsrc->configReg);
	
	pDev->adcCode = AD779X_adcCode;
	pDev->adcVolt = AD779X_adcVolt;
	
	pDev->Init = AD779X_Init;
	pDev->Reset = AD779X_Reset;
	pDev->WriteModeRegister = AD779X_WriteModeRegister;
	pDev->WriteConfigRegister = AD779X_WriteConfigRegister;
	pDev->WriteIORegister = AD779X_WriteIORegister;
	pDev->GetStatus = AD779X_GetStatus;
     pDev->SetBuffer = AD779X_SetBuffer;
     
     pDev->ReadModeReg = AD779X_ReadModeRegister;
     pDev->ReadConfigReg = AD779X_ReadConfigRegister;

	pDev->SetExCurrentValue = AD779X_SetExCurrentValue;
	pDev->SetExCurrentDirection = AD779X_SetExCurrentDirection;
	pDev->StartZSCalibration = AD779X_StartZSCalibration;
	pDev->StartFSCalibration = AD779X_StartFSCalibration;
	pDev->ReadDataRegister = AD779X_ReadDataRegister;

	pDev->GetVol = AD779X_adcVolt;


//	pDev->SetContinueRead = AD779X_ContinueRead;
//	pDev->HWDetect = AD779X_HWDetect;	
//	pDev->CheckReadySW = AD779X_CheckReadySW;
//	pDev->CheckReadyHW = AD779X_CheckReadyHW;
//	pDev->SetMode = AD779X_SetMode;
//	pDev->SetClkSource = AD779X_SetClkSource;
//	pDev->SetUpdateRate = AD779X_SetUpdateRate;

	
	//initial
	AD779X_Init(pRsrc);



//	AD779X_CSControl(pRsrc,cssEnable);
}

static u32 AD779X_adcCode(tAD779X_Rsrc *pRsrc){
//	u16 tmp;
	s32 code;
//	u8 g;
//	for(g=0;g<8;g++){
//		if(gain&0x01)	break;
//		gain >>= 1;
//	}
//	//config
//	tmp = pRsrc->configReg;
//	tmp = tAD779X_ConfigRegister_SetCh(tmp,ch);
//	tmp = tAD779X_ConfigRegister_SetGain(tmp,g);
//	tmp = tAD779X_ConfigRegister_SetPol(tmp,pol);	
//	AD779X_WriteConfigRegister(pRsrc,tmp);
//	//mode
//	tmp = pRsrc->modeReg;
//	tmp = tAD779X_ModeRegister_SetFS(tmp, fs);
//	AD779X_WriteModeRegister(pRsrc, tmp);
	//read convert result
	code = AD779X_ReadDataRegister(pRsrc);
//	print("code=%d\n",code);
//	print("gain=%d\n",g);
	return code;
}

static u32 AD779X_adcVolt(tAD779X_Rsrc *pRsrc){
	u32 mv;
	u16 i;
	u32 x,y;
     u8 gain = 0x01<<((pRsrc->configReg>>8)&0x07);

     mv = AD779X_ReadDataRegister(pRsrc);
	print("code=0x%x\tgain=%d\t",mv,gain);     
     
	if( ((pRsrc->configReg >> 12) & 0x01 )== ubBipolar){	//双极性
		mv = (mv-0x007fffff);
		for(x=0,y=0,i=0;i<pRsrc->refVol_mv;i++){
			x += mv;
			if(x >= 0x007fffff*gain){
				x -= 0x007fffff*gain;
				y++;
			}
		}
		mv = y*1000+x/10000;
		printS("0..0x007fffff\t");	
	}
	else{	//单极性
		for(x=0,y=0,i=0;i<pRsrc->refVol_mv;i++){
			x += mv;
			if(x >= 0x00ffffff*gain){
				x -= 0x00ffffff*gain;
				y++;
			}
		}
		if(x > 0x00ffffff*gain/2)	y++;
		mv = y*1000+x/10000;
	}
	return mv;
}

/**
 * @brief  Init HW and reset ADC
 * @param  None
 * @return None
 */
static void AD779X_Init(tAD779X_Rsrc *pRsrc)
{
	/* reset ADC */
	AD779X_Reset(pRsrc);
	/* Detect type of ADC */
	AD779X_HWDetect(pRsrc);
	
	AD779X_WriteModeRegister(pRsrc, pRsrc->modeReg);
	AD779X_WriteConfigRegister(pRsrc, pRsrc->configReg);
}

/**
 * @brief  Reset ADC
 * @param  None
 * @return None
 */
static void AD779X_Reset(tAD779X_Rsrc *pRsrc)
{
	/* active cs line */
	AD779X_CSControl(pRsrc,cssEnable);
	
	/* make 32 clk, while dout -> 1 */
	AD779X_TxByte(pRsrc,0xff);
	AD779X_TxByte(pRsrc,0xff);
	AD779X_TxByte(pRsrc,0xff);
	AD779X_TxByte(pRsrc,0xff);

	/* inactive cs line */
	AD779X_CSControl(pRsrc,cssDisable);
}

/**
 * @brief  Write value in MODE register
 * @param  Data - need write
 * @return None
 */
static void AD779X_WriteModeRegister(tAD779X_Rsrc *pRsrc, unsigned short Data)
{
     u16 mode = pRsrc->modeReg >> 13;
	/* active cs line */
	AD779X_CSControl(pRsrc,cssEnable);
     if(mode == 0 || mode==1)
          AD779X_WaitUntilReadyHW(pRsrc);
	/* send cmd: write MODE register */
	AD779X_TxByte(pRsrc,AD779X_WRR_MODE);
	
	/* write data to register */
	AD779X_TxByte(pRsrc,Data >> 8);
	AD779X_TxByte(pRsrc,Data & 0x00FF);
	
	/* inactive cs line */
	AD779X_CSControl(pRsrc, cssDisable);
	pRsrc->modeReg = Data;
}

/**
 * @brief  read value from MODE register
 * @param  Data - need write
 * @return None
 */
static unsigned short AD779X_ReadModeRegister(tAD779X_Rsrc *pRsrc)
{
     u8 tmp[2];
	/* active cs line */
	AD779X_CSControl(pRsrc,cssEnable);
	AD779X_WaitUntilReadyHW(pRsrc);
	/* send cmd: write CONFIG register */
	AD779X_TxByte(pRsrc, AD779X_RDR_MODE);
	
	/* write data to register */
	tmp[1] = AD779X_RxByte(pRsrc);
	tmp[0] = AD779X_RxByte(pRsrc);
	
	/* inactive cs line */
	AD779X_CSControl(pRsrc, cssDisable);
	pRsrc->modeReg = tmp[1];
     pRsrc->modeReg <<= 8;
     pRsrc->modeReg |= tmp[0];
     return pRsrc->modeReg;
}

/**
 * @brief  Write value in CONFIG register
 * @param  Data - need write
 * @return None
 */
static void AD779X_WriteConfigRegister(tAD779X_Rsrc *pRsrc, unsigned short Data)
{
	/* active cs line */
	AD779X_CSControl(pRsrc,cssEnable);
	AD779X_WaitUntilReadyHW(pRsrc);
	/* send cmd: write CONFIG register */
	AD779X_TxByte(pRsrc,AD779X_WRR_CONFIG);
	
	/* write data to register */
	AD779X_TxByte(pRsrc,Data >> 8);
	AD779X_TxByte(pRsrc,Data & 0x00FF);
	
	/* inactive cs line */
	AD779X_CSControl(pRsrc, cssDisable);
	pRsrc->configReg = Data;
}

/**
 * @brief  read value from CONFIG register
 * @param  Data - need write
 * @return None
 */
static unsigned short AD779X_ReadConfigRegister(tAD779X_Rsrc *pRsrc)
{
     u8 tmp[2];
	/* active cs line */
	AD779X_CSControl(pRsrc,cssEnable);
	AD779X_WaitUntilReadyHW(pRsrc);
	/* send cmd: write CONFIG register */
	AD779X_TxByte(pRsrc,AD779X_RDR_CONFIG);
	
	/* write data to register */
	tmp[1] = AD779X_RxByte(pRsrc);
	tmp[0] = AD779X_RxByte(pRsrc);
	
	/* inactive cs line */
	AD779X_CSControl(pRsrc, cssDisable);
	pRsrc->configReg = tmp[1];
     pRsrc->configReg <<= 8;
     pRsrc->configReg |= tmp[0];
     return pRsrc->configReg;
}

/**
 * @brief  Write value in IO register
 * @param  Data - need write
 * @return None
 */
static void AD779X_WriteIORegister(tAD779X_Rsrc *pRsrc, unsigned char Data)
{
	/* active cs line */
	AD779X_CSControl(pRsrc,cssEnable);
	AD779X_WaitUntilReadyHW(pRsrc);
	/* send cmd: write IO register */
	AD779X_TxByte(pRsrc,AD779X_WRR_IO);
	
	/* write data to register */
	AD779X_TxByte(pRsrc,Data);
	
	/* inactive cs line */
	AD779X_CSControl(pRsrc, cssDisable);
	pRsrc->IOReg = Data;
}

/**
 * @brief  Detect ADC type
 * @param  None
 * @return Model as status of detecting
 */
static void AD779X_HWDetect(tAD779X_Rsrc *pRsrc)
{
	unsigned short m_adc_model;
	
	/* active cs line */
	AD779X_CSControl(pRsrc,cssEnable);
	
	/* send cmd: read ID register */
	AD779X_TxByte(pRsrc,AD779X_RDR_ID);
	
	/* get ID */
	m_adc_model = AD779X_RxByte(pRsrc);
	
	/* inactive cs line */
	AD779X_CSControl(pRsrc, cssDisable);

	switch (m_adc_model & 0xF)
	{
		case AD7792_PARTID:	//0x0a
			pRsrc->Model =  ad7792;
		break;
		
		case AD7793_PARTID:	//0x0b
			pRsrc->Model =  ad7793;
		break;
		
		default:
			pRsrc->Model =  adNone;
			break;
	}
}

///**
// * @brief  Exit from continue read
// * @param  None
// * @return none
// */
//static void AD779X_ContinueRead(tAD779X_Rsrc *pRsrc, u8 op)
//{
//	/* active cs line */
//	AD779X_CSControl(pRsrc,cssEnable);
//	while(HAL_GPIO_ReadPin(pRsrc->MISO.GPIOx, pRsrc->MISO.GPIO_Pin) == GPIO_PIN_SET){}
//	if(op == 0)	//disable
//		AD779X_TxByte(pRsrc,0x58);
//	else
//		AD779X_TxByte(pRsrc,0x5c);
//	/* inactive cs line */
//	AD779X_CSControl(pRsrc, cssDisable);	
//}

///**
// * @brief  Exit from continue read
// * @param  None
// * @return none
// */
//static void AD779X_EnterContinueRead(tAD779X_Rsrc *pRsrc)
//{
//	/* active cs line */
//	AD779X_CSControl(pRsrc,cssEnable);
//	while(HAL_GPIO_ReadPin(pRsrc->MISO.GPIOx, pRsrc->MISO.GPIO_Pin) == GPIO_PIN_SET){}
//	AD779X_TxByte(pRsrc,0x58);
//	/* inactive cs line */
//	AD779X_CSControl(pRsrc, cssDisable);	
//}

/**
 * @brief  Get ADC status
 * @param  None
 * @return Value as status
 */
static unsigned char AD779X_GetStatus(tAD779X_Rsrc *pRsrc)
{
	unsigned char m_adc_status;

	/* active cs line */
	AD779X_CSControl(pRsrc,cssEnable);
	while(HAL_GPIO_ReadPin(pRsrc->MISO.GPIOx, pRsrc->MISO.GPIO_Pin) == GPIO_PIN_SET){}
	/* send cmd: read STATUS register */
	AD779X_TxByte(pRsrc,AD779X_RDR_STATUS);
	
	/* get value */
	m_adc_status = AD779X_RxByte(pRsrc);
	
	/* inactive cs line */
	AD779X_CSControl(pRsrc, cssDisable);
	
	return m_adc_status;
}

/**
 * @brief  Check ready status by STATUS_REG
 * @param  None
 * @return true - ready, false - not ready
 */
static void AD779X_WaitUntilReadySW(tAD779X_Rsrc *pRsrc)
{
	while(AD779X_GetStatus(pRsrc) & AD779X_SR_RDY){
		if(pRsrc->delay)	pRsrc->delay(1);
	}
}

/**
 * @brief  Check ready state by RDI pin
 * @param  None
 * @return true - ready, false - not ready
 */
static void AD779X_WaitUntilReadyHW(tAD779X_Rsrc *pRsrc)
{
	/* active cs line */
//	AD779X_CSControl(pRsrc,cssEnable);
	while(HAL_GPIO_ReadPin(pRsrc->MISO.GPIOx, pRsrc->MISO.GPIO_Pin) == GPIO_PIN_SET){
		if(pRsrc->delay)	pRsrc->delay(1);
	}
	/* inactive cs line */
//	AD779X_CSControl(pRsrc, cssDisable);
	
}

/**
 * @brief  Set ADC mode
 * @param  Mode - need set
 * @return None
 */
static void AD779X_SetMode(tAD779X_Rsrc *pRsrc, tAD779X_ModeSelect Mode)
{
	/* write value in MODE register */
	AD779X_WriteModeRegister(pRsrc, tAD779X_ModeRegister_SetMod(pRsrc->modeReg, Mode));
}

/**
 * @brief  Set ADC clock source
 * @param  ClkSource - need set
 * @return None
 */
static void AD779X_SetClkSource(tAD779X_Rsrc *pRsrc, tAD779X_ClkSourceSelect ClkSource)
{
	/* write value in MODE register */
	AD779X_WriteModeRegister(pRsrc, tAD779X_ModeRegister_SetClk(pRsrc->modeReg,ClkSource));
}

/**
 * @brief  Set ADC clock update rates
 * @param  ClkSource - need set
 * @return None
 */
static void AD779X_SetUpdateRate(tAD779X_Rsrc *pRsrc, tAD779X_FilterSelect UpdateRates)
{
	/* write value in MODE register */
	AD779X_WriteModeRegister(pRsrc, tAD779X_ModeRegister_SetFS(pRsrc->modeReg,UpdateRates));
}

/**
 * @brief  Set buffer enable
 * @param  
 * @return None
 */
static void AD779X_SetBuffer(tAD779X_Rsrc *pRsrc, u8 en)
{
	/* write value in MODE register */
	AD779X_WriteConfigRegister(pRsrc, tAD779X_ConfigRegister_SetBuf(pRsrc->configReg,en));
}


/**
 * @brief  Set ADC excitation currents value
 * @param  excValue - need sat
 * @return None
 */
static void AD779X_SetExCurrentValue(tAD779X_Rsrc *pRsrc, tAD779X_IEXCENSelect excValue)
{
	/* write value in IO register */
	AD779X_WriteIORegister(pRsrc, tAD779X_IORegister_SetIEXCEN(pRsrc->IOReg,excValue));
}

/**
 * @brief  Set ADC excitation currents direction
 * @param  excDirection - need sat
 * @return None
 */
static void AD779X_SetExCurrentDirection(tAD779X_Rsrc *pRsrc, tAD779X_IEXCDIRSelect excDirection)
{	
	/* write value in IO register */
	AD779X_WriteIORegister(pRsrc, tAD779X_IORegister_SetIEXCDIR(pRsrc->IOReg,excDirection));
}

/**
 * @brief  Start ADC Zero-Scale Calibration
 * @param  None
 * @return None
 */
static void AD779X_StartZSCalibration(tAD779X_Rsrc *pRsrc)
{
	AD779X_SetMode(pRsrc, mdsIntZeroCal);
}

/**
 * @brief  Start ADC Full-Scale Calibration
 * @param  None
 * @return None
 */
static void AD779X_StartFSCalibration(tAD779X_Rsrc *pRsrc)
{
	AD779X_SetMode(pRsrc, mdsIntFullCal);
}


/**
 * @brief  Read data from ADC (24-bit)
 * @param  None
 * @return Data register value
 */
static u32 AD779X_ReadDataRegister(tAD779X_Rsrc *pRsrc)
{
	unsigned char m_data[3] = {0};

	if(pRsrc->Model == adNone)	return 0xffffffff;
	/* active cs line */
	AD779X_CSControl(pRsrc,cssEnable);
	AD779X_WaitUntilReadyHW(pRsrc);
	/* send cmd: read DATA register */
	AD779X_TxByte(pRsrc,AD779X_RDR_DATA);
	
	/* get value */
	HAL_GPIO_WritePin(pRsrc->MOSI.GPIOx, pRsrc->MOSI.GPIO_Pin, GPIO_PIN_RESET);
	if(pRsrc->Model == ad7793)	m_data[2] = AD779X_RxByte(pRsrc);
	m_data[1] = AD779X_RxByte(pRsrc);
	m_data[0] = AD779X_RxByte(pRsrc);
	
	/* inactive cs line */
	AD779X_CSControl(pRsrc, cssDisable);
	
	return (((unsigned long)((m_data[2]<<8)|m_data[1]))<<8)|m_data[0]; // NOTE: iar compiler hell
}


static void AD779X_CSControl(tAD779X_Rsrc *pRsrc, tAD779X_CSState sel){
	if(sel == cssDisable)	HAL_GPIO_WritePin(pRsrc->CS.GPIOx, pRsrc->CS.GPIO_Pin, GPIO_PIN_SET);
	else	HAL_GPIO_WritePin(pRsrc->CS.GPIOx, pRsrc->CS.GPIO_Pin, GPIO_PIN_RESET);		
}

static void AD779X_TxByte(tAD779X_Rsrc *pRsrc, u8 dat){
	u8 i,tmp = dat;
	for(i=0;i<8;i++){
			HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_RESET);		
			if(tmp & 0x80) HAL_GPIO_WritePin(pRsrc->MOSI.GPIOx, pRsrc->MOSI.GPIO_Pin, GPIO_PIN_SET);
			else HAL_GPIO_WritePin(pRsrc->MOSI.GPIOx, pRsrc->MOSI.GPIO_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_SET);
			tmp <<= 1;
	}
}

static u8 AD779X_RxByte(tAD779X_Rsrc *pRsrc){
	u8 dat;

	for(u8 i=0; i<8; i++){
			 HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_RESET);		
			 dat <<=1;
			 if(HAL_GPIO_ReadPin(pRsrc->MISO.GPIOx, pRsrc->MISO.GPIO_Pin) == GPIO_PIN_SET)     dat |= 0x01;  
			 HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_SET);
	}
	return dat;
}



