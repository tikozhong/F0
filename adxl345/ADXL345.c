/***************************************************************************//**
 *   @file   ADXL345.c
 *   @brief  Implementation of ADXL345 Driver.
 *   @author DBogdan (dragos.bogdan@analog.com)
*******************************************************************************/

/******************************************************************************/
/***************************** Include Files **********************************/
/******************************************************************************/
#include "ADXL345.h"
#include "string.h"

/******************************************************************************/
/************************ Variables Definitions *******************************/
/******************************************************************************/
//unsigned char ADXL345_COMMUNICATION = 0;
/******************************************************************************/
/************************ Functions Definitions *******************************/
/******************************************************************************/
/*! Writes data into a register. */
static void ADXL345_SetRegisterValue(
     ADXL345_Rsrc_T* pRsrc,
     unsigned char registerAddress,
	unsigned char registerValue);

/*<! Reads the value of a register. */
static unsigned char ADXL345_GetRegisterValue(
     ADXL345_Rsrc_T* pRsrc,
     unsigned char registerAddress);

/*! Initializes the I2C peripheral and checks if the ADXL345 part is present. */
static unsigned char ADXL345_Init(ADXL345_Rsrc_T* pRsrc);
static void ADXL345_WriteReg(ADXL345_Rsrc_T *pRsrc, u8 addr, u8 val);
static u8 ADXL345_ReadReg(ADXL345_Rsrc_T *pRsrc, u8 addr);

/*! Places the device into standby/measure mode. */
static void ADXL345_SetPowerMode(
     ADXL345_Rsrc_T* pRsrc,
     unsigned char pwrMode);

/*! Reads the output data of each axis. */
static void ADXL345_GetXyz(
     ADXL345_Rsrc_T* pRsrc,
     unsigned short* x,
	unsigned short* y,
	unsigned short* z);

/*! Enables/disables the tap detection. */
static void ADXL345_SetTapDetection(
     ADXL345_Rsrc_T* pRsrc,
     unsigned char tapType,
	unsigned char tapAxes,
	unsigned char tapDur,
     unsigned char tapLatent,
     unsigned char tapWindow,
     unsigned char tapThresh,
     unsigned char tapInt);

/*! Enables/disables the activity detection. */
static void ADXL345_SetActivityDetection(
     ADXL345_Rsrc_T* pRsrc,
     unsigned char actOnOff,
	unsigned char actAxes,
	unsigned char actAcDc,
	unsigned char actThresh,
	unsigned char actInt);

/*! Enables/disables the inactivity detection. */
static void ADXL345_SetInactivityDetection(
     ADXL345_Rsrc_T* pRsrc,
     unsigned char inactOnOff,
	unsigned char inactAxes,
	unsigned char inactAcDc,
	unsigned char inactThresh,
	unsigned char inactTime,
	unsigned char inactInt);

/*! Enables/disables the free-fall detection. */
static void ADXL345_SetFreeFallDetection(
     ADXL345_Rsrc_T* pRsrc,
     unsigned char ffOnOff,
	unsigned char ffThresh,
	unsigned char ffTime,
	unsigned char ffInt);

/*! Calibrates the accelerometer. */
static void ADXL345_SetOffset(
     ADXL345_Rsrc_T* pRsrc,
     unsigned char xOffset,
	unsigned char yOffset,
	unsigned char zOffset);

            
/**************************************************************************//**
 * @brief Writes data into a register.
 *
 * @param registerAddress - Address of the register.
 * @param registerValue - Data value to write.
 *
 * @return None.
******************************************************************************/
void ADXL345_Setup(
	ADXL345_Dev_T *pDev, 
	const char* NAME, 
  SPI_HandleTypeDef* SPI,
	PIN_T CS,
	void (*readCalCfg)	(CAL32_T*, CAL32_T*, CAL32_T*),
	void (*writeCalCfg)	(CAL32_T, CAL32_T, CAL32_T)
){
     //HAL_SPI_Init(SPI);
     pDev->rsrc.spi = SPI;
     pDev->rsrc.CS = CS;
		 memcpy(pDev->rsrc.name, NAME, (strlen(NAME)<DEV_NAME_LEN?strlen(NAME):DEV_NAME_LEN));
     HAL_GPIO_WritePin(pDev->rsrc.CS.GPIOx, pDev->rsrc.CS.GPIO_Pin, GPIO_PIN_SET);
     
     pDev->init = ADXL345_Init;
     pDev->setPowerMode = ADXL345_SetPowerMode;
     pDev->getXyz = ADXL345_GetXyz;
     pDev->setTapDetection = ADXL345_SetTapDetection;
     pDev->setActivityDetection = ADXL345_SetActivityDetection;
     pDev->setInactivityDetection = ADXL345_SetInactivityDetection;
     pDev->setFreeFallDetection = ADXL345_SetFreeFallDetection;
     pDev->setOffset = ADXL345_SetOffset;
     pDev->writeReg = ADXL345_WriteReg;
     pDev->readReg = ADXL345_ReadReg;
     
     pDev->rsrc.id = pDev->init(&pDev->rsrc);
}

/**************************************************************************//**
 * @brief Writes data into a register.
 *
 * @param registerAddress - Address of the register.
 * @param registerValue - Data value to write.
 *
 * @return None.
******************************************************************************/
static void ADXL345_SetRegisterValue(
     ADXL345_Rsrc_T* pRsrc,
     unsigned char registerAddress,
	unsigned char registerValue)
{
    unsigned char writeData[2] = {0, 0};
     
   writeData[0] = registerAddress;
   writeData[1] = registerValue;
   //while (HAL_SPI_GetState(pRsrc->spi) != HAL_SPI_STATE_READY) {} 
   HAL_GPIO_WritePin(pRsrc->CS.GPIOx, pRsrc->CS.GPIO_Pin, GPIO_PIN_RESET);
   HAL_SPI_Transmit(pRsrc->spi, writeData, 2, 1);
   HAL_GPIO_WritePin(pRsrc->CS.GPIOx, pRsrc->CS.GPIO_Pin, GPIO_PIN_SET);
}

/***************************************************************************//**
 * @brief Reads the value of a register.
 *
 * @param registerAddress - Address of the register.
 *
 * @return registerValue - Value of the register.
*******************************************************************************/
static unsigned char ADXL345_GetRegisterValue(ADXL345_Rsrc_T* pRsrc,unsigned char registerAddress)
{
    unsigned char readData[2] = {0, 0};
    unsigned char writeData[2] = {0, 0};
    unsigned char registerValue = 0;

   writeData[0] = 0x80 + registerAddress;
   writeData[1] = 0;
   //while (HAL_SPI_GetState(pRsrc->spi) != HAL_SPI_STATE_READY) {} 
   HAL_GPIO_WritePin(pRsrc->CS.GPIOx, pRsrc->CS.GPIO_Pin, GPIO_PIN_RESET);
   HAL_SPI_TransmitReceive(pRsrc->spi, writeData, readData, 2, 1);
   HAL_GPIO_WritePin(pRsrc->CS.GPIOx, pRsrc->CS.GPIO_Pin, GPIO_PIN_SET);
   registerValue = readData[1];
    
   return(registerValue);
}

/***************************************************************************//**
 * @brief Initializes the communication peripheral and checks if the ADXL345
 *          part is present.
 *
 * @return status - Result of the initialization procedure.
 *                  Example: 0x0 - I2C peripheral was not initialized or
 *                                 ADXL345 part is not present.
 *                           0x1 - I2C peripheral is initialized and ADXL345
 *                                 part is present.
*******************************************************************************/
static unsigned char ADXL345_Init(ADXL345_Rsrc_T* pRsrc)
{
    return(ADXL345_GetRegisterValue(pRsrc, ADXL345_DEVID));
}

/***************************************************************************//**
 * @brief Initializes the communication peripheral and checks if the ADXL345
 *          part is present.
 *
 * @return status - Result of the initialization procedure.
 *                  Example: 0x0 - I2C peripheral was not initialized or
 *                                 ADXL345 part is not present.
 *                           0x1 - I2C peripheral is initialized and ADXL345
 *                                 part is present.
*******************************************************************************/
static void ADXL345_WriteReg(ADXL345_Rsrc_T *pRsrc, u8 addr, u8 val)
{
    ADXL345_SetRegisterValue(pRsrc, addr, val);
}

static u8 ADXL345_ReadReg(ADXL345_Rsrc_T *pRsrc, u8 addr)
{
     return (ADXL345_GetRegisterValue(pRsrc, addr));
}


/***************************************************************************//**
 * @brief Places the device into standby/measure mode.
 *
 * @param pwrMode - Power mode.
 *                    Example: 0x0 - standby mode.
 *                             0x1 - measure mode.
 *
 * @return None.
*******************************************************************************/
static void ADXL345_SetPowerMode(ADXL345_Rsrc_T* pRsrc, unsigned char pwrMode)
{
    unsigned char oldPowerCtl = 0;
    unsigned char newPowerCtl = 0;
    
    oldPowerCtl = ADXL345_GetRegisterValue(pRsrc, ADXL345_POWER_CTL);
    newPowerCtl = oldPowerCtl & ~ADXL345_PCTL_MEASURE;
    newPowerCtl = newPowerCtl | (pwrMode * ADXL345_PCTL_MEASURE);
    ADXL345_SetRegisterValue(pRsrc, ADXL345_POWER_CTL, newPowerCtl);
}

/***************************************************************************//**
 * @brief Reads the output data of each axis.
 *
 * @param x - X-axis's output data.
 * @param y - Y-axis's output data.
 * @param z - Z-axis's output data.
 *
 * @return None.
*******************************************************************************/
static void ADXL345_GetXyz(
     ADXL345_Rsrc_T* pRsrc, 
     unsigned short* x,
     unsigned short* y,
     unsigned short* z)
{
//    *x = ADXL345_GetRegisterValue(pRsrc, ADXL345_DATAX1) << 8;
//    *x += ADXL345_GetRegisterValue(pRsrc, ADXL345_DATAX0);
//    *y = ADXL345_GetRegisterValue(pRsrc, ADXL345_DATAY1) << 8;
//    *y += ADXL345_GetRegisterValue(pRsrc, ADXL345_DATAY0);
//    *z = ADXL345_GetRegisterValue(pRsrc, ADXL345_DATAZ1) << 8;
//    *z += ADXL345_GetRegisterValue(pRsrc, ADXL345_DATAZ0);
     
     
    unsigned char readData[7] = {0};
    unsigned char writeData[7] = {0};

   writeData[0] = 0xc0 + ADXL345_DATAX0;
   HAL_GPIO_WritePin(pRsrc->CS.GPIOx, pRsrc->CS.GPIO_Pin, GPIO_PIN_RESET);
   HAL_SPI_TransmitReceive(pRsrc->spi, writeData, readData, 7, 2);
   HAL_GPIO_WritePin(pRsrc->CS.GPIOx, pRsrc->CS.GPIO_Pin, GPIO_PIN_SET);
    *x = readData[2] << 8;
    *x += readData[1];
    *y = readData[4] << 8;
    *y += readData[3];
    *z = readData[6] << 8;
    *z += readData[5];
   
}

/***************************************************************************//**
 * @brief Enables/disables the tap detection.
 *
 * @param tapType - Tap type (none, single, double).
 *                    Example: 0x0 - disables tap detection.
 *                             ADXL345_SINGLE_TAP - enables single tap detection.
 *                             ADXL345_DOUBLE_TAP - enables double tap detection.
 * @param tapAxes - Axes which participate in tap detection.
 *                    Example: 0x0 - disables axes participation.
 *                             ADXL345_TAP_X_EN - enables x-axis participation.
 *                             ADXL345_TAP_Y_EN - enables y-axis participation.
 *                             ADXL345_TAP_Z_EN - enables z-axis participation.
 * @param tapDur - Tap duration.
 * @param tapLatent - Tap latency.
 * @param tapWindow - Tap window. 
 * @param tapThresh - Tap threshold.
 * @param tapInt - Interrupts pin.
 *                   Example: 0x0 - interrupts on INT1 pin.
 *                            ADXL345_SINGLE_TAP - single tap interrupts on
 *                                                 INT2 pin.
 *                            ADXL345_DOUBLE_TAP - double tap interrupts on
 *                                                 INT2 pin.
 *
 * @return None.
*******************************************************************************/
static void ADXL345_SetTapDetection(
     ADXL345_Rsrc_T* pRsrc, 
     unsigned char tapType,
     unsigned char tapAxes,
     unsigned char tapDur,
     unsigned char tapLatent,
     unsigned char tapWindow,
     unsigned char tapThresh,
     unsigned char tapInt)
{
    unsigned char oldTapAxes    = 0;
    unsigned char newTapAxes    = 0;
    unsigned char oldIntMap     = 0;
    unsigned char newIntMap     = 0;
    unsigned char oldIntEnable  = 0;
    unsigned char newIntEnable  = 0;
    
    oldTapAxes = ADXL345_GetRegisterValue(pRsrc, ADXL345_TAP_AXES);
    newTapAxes = oldTapAxes & ~(ADXL345_TAP_X_EN |
                                ADXL345_TAP_Y_EN |
                                ADXL345_TAP_Z_EN);
    newTapAxes = newTapAxes | tapAxes;
    ADXL345_SetRegisterValue(pRsrc, ADXL345_TAP_AXES, newTapAxes);
    ADXL345_SetRegisterValue(pRsrc, ADXL345_DUR, tapDur);
    ADXL345_SetRegisterValue(pRsrc, ADXL345_LATENT, tapLatent);
    ADXL345_SetRegisterValue(pRsrc, ADXL345_WINDOW, tapWindow);
    ADXL345_SetRegisterValue(pRsrc, ADXL345_THRESH_TAP, tapThresh);
    oldIntMap = ADXL345_GetRegisterValue(pRsrc, ADXL345_INT_MAP);
    newIntMap = oldIntMap & ~(ADXL345_SINGLE_TAP | ADXL345_DOUBLE_TAP);
    newIntMap = newIntMap | tapInt;
    ADXL345_SetRegisterValue(pRsrc, ADXL345_INT_MAP, newIntMap);
    oldIntEnable = ADXL345_GetRegisterValue(pRsrc, ADXL345_INT_ENABLE);
    newIntEnable = oldIntEnable & ~(ADXL345_SINGLE_TAP | ADXL345_DOUBLE_TAP);
    newIntEnable = newIntEnable | tapType;
    ADXL345_SetRegisterValue(pRsrc, ADXL345_INT_ENABLE, newIntEnable);
}

/***************************************************************************//**
 * @brief Enables/disables the activity detection.
 *
 * @param actOnOff - Enables/disables the activity detection.
 *                     Example: 0x0 - disables the activity detection.
 *                              0x1 - enables the activity detection.
 * @param actAxes - Axes which participate in detecting activity.
 *                    Example: 0x0 - disables axes participation.
 *                             ADXL345_ACT_X_EN - enables x-axis participation.
 *                             ADXL345_ACT_Y_EN - enables y-axis participation.
 *                             ADXL345_ACT_Z_EN - enables z-axis participation.
 * @param actAcDc - Selects dc-coupled or ac-coupled operation.
 *                    Example: 0x0 - dc-coupled operation.
 *                             ADXL345_ACT_ACDC - ac-coupled operation.
 * @param actThresh - Threshold value for detecting activity.
 * @patam actInt - Interrupts pin.
 *                   Example: 0x0 - activity interrupts on INT1 pin.
 *                            ADXL345_ACTIVITY - activity interrupts on INT2 pin.
 *
 * @return None.
*******************************************************************************/
static void ADXL345_SetActivityDetection(
     ADXL345_Rsrc_T* pRsrc, 
     unsigned char actOnOff,
     unsigned char actAxes,
     unsigned char actAcDc,
     unsigned char actThresh,
     unsigned char actInt)
{
    unsigned char oldActInactCtl    = 0;
    unsigned char newActInactCtl    = 0;
    unsigned char oldIntMap         = 0;
    unsigned char newIntMap         = 0;
    unsigned char oldIntEnable      = 0;
    unsigned char newIntEnable      = 0;
    
    oldActInactCtl = ADXL345_GetRegisterValue(pRsrc, ADXL345_INT_ENABLE);
    newActInactCtl = oldActInactCtl & ~(ADXL345_ACT_ACDC |
                                        ADXL345_ACT_X_EN |
                                        ADXL345_ACT_Y_EN |
                                        ADXL345_ACT_Z_EN);
    newActInactCtl = newActInactCtl | (actAcDc | actAxes);
    ADXL345_SetRegisterValue(pRsrc, ADXL345_ACT_INACT_CTL, newActInactCtl);
    ADXL345_SetRegisterValue(pRsrc, ADXL345_THRESH_ACT, actThresh);
    oldIntMap = ADXL345_GetRegisterValue(pRsrc, ADXL345_INT_MAP);
    newIntMap = oldIntMap & ~(ADXL345_ACTIVITY);
    newIntMap = newIntMap | actInt;
    ADXL345_SetRegisterValue(pRsrc, ADXL345_INT_MAP, newIntMap);
    oldIntEnable = ADXL345_GetRegisterValue(pRsrc, ADXL345_INT_ENABLE);
    newIntEnable = oldIntEnable & ~(ADXL345_ACTIVITY);
    newIntEnable = newIntEnable | (ADXL345_ACTIVITY * actOnOff);
    ADXL345_SetRegisterValue(pRsrc, ADXL345_INT_ENABLE, newIntEnable);
}

/***************************************************************************//**
 * @brief Enables/disables the inactivity detection.
 *
 * @param inactOnOff - Enables/disables the inactivity detection.
 *                       Example: 0x0 - disables the inactivity detection.
 *                                0x1 - enables the inactivity detection.
 * @param inactAxes - Axes which participate in detecting inactivity.
 *                      Example: 0x0 - disables axes participation.
 *                               ADXL345_INACT_X_EN - enables x-axis.
 *                               ADXL345_INACT_Y_EN - enables y-axis.
 *                               ADXL345_INACT_Z_EN - enables z-axis.
 * @param inactAcDc - Selects dc-coupled or ac-coupled operation.
 *                      Example: 0x0 - dc-coupled operation.
 *                               ADXL345_INACT_ACDC - ac-coupled operation.
 * @param inactThresh - Threshold value for detecting inactivity.
 * @param inactTime - Inactivity time.
 * @patam inactInt - Interrupts pin.
 *                     Example: 0x0 - inactivity interrupts on INT1 pin.
 *                              ADXL345_INACTIVITY - inactivity interrupts on
 *                                                   INT2 pin.
 *
 * @return None.
*******************************************************************************/
static void ADXL345_SetInactivityDetection(
     ADXL345_Rsrc_T* pRsrc, 
     unsigned char inactOnOff,
     unsigned char inactAxes,
     unsigned char inactAcDc,
     unsigned char inactThresh,
     unsigned char inactTime,
     unsigned char inactInt)
{
    unsigned char oldActInactCtl    = 0;
    unsigned char newActInactCtl    = 0;
    unsigned char oldIntMap         = 0;
    unsigned char newIntMap         = 0;
    unsigned char oldIntEnable      = 0;
    unsigned char newIntEnable      = 0;
    
    oldActInactCtl = ADXL345_GetRegisterValue(pRsrc, ADXL345_INT_ENABLE);
    newActInactCtl = oldActInactCtl & ~(ADXL345_INACT_ACDC |
                                        ADXL345_INACT_X_EN |
                                        ADXL345_INACT_Y_EN |
                                        ADXL345_INACT_Z_EN);
    newActInactCtl = newActInactCtl | (inactAcDc | inactAxes);
    ADXL345_SetRegisterValue(pRsrc, ADXL345_ACT_INACT_CTL, newActInactCtl);
    ADXL345_SetRegisterValue(pRsrc, ADXL345_THRESH_INACT, inactThresh);
    ADXL345_SetRegisterValue(pRsrc, ADXL345_TIME_INACT, inactTime);
    oldIntMap = ADXL345_GetRegisterValue(pRsrc, ADXL345_INT_MAP);
    newIntMap = oldIntMap & ~(ADXL345_INACTIVITY);
    newIntMap = newIntMap | inactInt;
    ADXL345_SetRegisterValue(pRsrc, ADXL345_INT_MAP, newIntMap);
    oldIntEnable = ADXL345_GetRegisterValue(pRsrc, ADXL345_INT_ENABLE);
    newIntEnable = oldIntEnable & ~(ADXL345_INACTIVITY);
    newIntEnable = newIntEnable | (ADXL345_INACTIVITY * inactOnOff);
    ADXL345_SetRegisterValue(pRsrc, ADXL345_INT_ENABLE, newIntEnable);
}

/***************************************************************************//**
 * @brief Enables/disables the free-fall detection.
 *
 * @param ffOnOff - Enables/disables the free-fall detection.
 *                    Example: 0x0 - disables the free-fall detection.
 *                             0x1 - enables the free-fall detection.
 * @param ffThresh - Threshold value for free-fall detection.
 * @param ffTime - Time value for free-fall detection.
 * @param ffInt - Interrupts pin.
 *                  Example: 0x0 - free-fall interrupts on INT1 pin.
 *                           ADXL345_FREE_FALL - free-fall interrupts on INT2 pin.
 *
 * @return None.
*******************************************************************************/
static void ADXL345_SetFreeFallDetection(
     ADXL345_Rsrc_T* pRsrc, 
     unsigned char ffOnOff,
     unsigned char ffThresh,
     unsigned char ffTime,
     unsigned char ffInt)
{
    unsigned char oldIntMap     = 0;
    unsigned char newIntMap     = 0;
    unsigned char oldIntEnable  = 0;
    unsigned char newIntEnable  = 0;
    
    ADXL345_SetRegisterValue(pRsrc, ADXL345_THRESH_FF, ffThresh);
    ADXL345_SetRegisterValue(pRsrc, ADXL345_TIME_FF, ffTime);
    oldIntMap = ADXL345_GetRegisterValue(pRsrc, ADXL345_INT_MAP);
    newIntMap = oldIntMap & ~(ADXL345_FREE_FALL);
    newIntMap = newIntMap | ffInt;
    ADXL345_SetRegisterValue(pRsrc, ADXL345_INT_MAP, newIntMap);
    oldIntEnable = ADXL345_GetRegisterValue(pRsrc, ADXL345_INT_ENABLE);
    newIntEnable = oldIntEnable & ~ADXL345_FREE_FALL;
    newIntEnable = newIntEnable | (ADXL345_FREE_FALL * ffOnOff);
    ADXL345_SetRegisterValue(pRsrc, ADXL345_INT_ENABLE, newIntEnable);
}

/***************************************************************************//**
 * @brief Calibrates the accelerometer.
 *
 * @param xOffset - X-axis's offset.
 * @param yOffset - Y-axis's offset.
 * @param zOffset - Z-axis's offset.
 *
 * @return None.
*******************************************************************************/
static void ADXL345_SetOffset(
     ADXL345_Rsrc_T* pRsrc, 
     unsigned char xOffset,
     unsigned char yOffset,
     unsigned char zOffset)
{
    ADXL345_SetRegisterValue(pRsrc, ADXL345_OFSX, xOffset);
    ADXL345_SetRegisterValue(pRsrc, ADXL345_OFSY, yOffset);
    ADXL345_SetRegisterValue(pRsrc, ADXL345_OFSZ, yOffset);
}


