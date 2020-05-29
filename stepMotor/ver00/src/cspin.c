/**
  ******************************************************************************
  * @file    cspin.c 
  * @author  IPC Rennes
  * @version V2.1
  * @date    October 15, 2013
  * @brief   cSPIN (L6480 and L6482) product related routines
  * @note    (C) COPYRIGHT 2013 STMicroelectronics
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  */ 


/* Includes ------------------------------------------------------------------*/
#include "cspin.h"
#include "cspin_config.h"

/** @addtogroup cSPIN FW library interface
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void cSPIN_Delay(__IO uint32_t nCount);
void cSPIN_Reset_And_Standby(cSPIN_Rsrc_T* pRsrc);
void cSPIN_Regs_Struct_Reset(cSPIN_Rsrc_T* pRsrc);
void cSPIN_Registers_Set(cSPIN_Rsrc_T* pRsrc);
void cSPIN_Reset_Device(cSPIN_Rsrc_T* pRsrc);
uint8_t cSPIN_Write_Byte(cSPIN_Rsrc_T* pRsrc, uint8_t byte);




/**
  * @brief  Setup a motor to be ready to run
  * @param  None
  * @retval None
  */
void cSPIN_Setup(cSPIN_Rsrc_T* pRsrc){
	
	/* Resets and puts cSPIN into standby mode */
#ifdef ONLY_USING_SPI
	cSPIN_Reset_Device(pRsrc);
#else
	cSPIN_Reset_And_Standby(pRsrc);	
#endif	
	
	/* Structure initialization by default values, in order to avoid blank records */
	cSPIN_Regs_Struct_Reset(pRsrc);

	/***********************************************
	@ Load register val from cspin_config.h
	***********************************************/
	/* Acceleration rate settings to cSPIN_CONF_PARAM_ACC in steps/s2, range 14.55 to 59590 steps/s2 */
	pRsrc->reg.ACC 		= AccDec_Steps_to_Par(cSPIN_CONF_PARAM_ACC);
	/* Deceleration rate settings to cSPIN_CONF_PARAM_DEC in steps/s2, range 14.55 to 59590 steps/s2 */
	pRsrc->reg.DEC 		= AccDec_Steps_to_Par(cSPIN_CONF_PARAM_DEC); 
	/* Maximum speed settings to cSPIN_CONF_PARAM_MAX_SPEED in steps/s, range 15.25 to 15610 steps/s */
	pRsrc->reg.MAX_SPEED 	= MaxSpd_Steps_to_Par(cSPIN_CONF_PARAM_MAX_SPEED);
	/* Full step speed settings cSPIN_CONF_PARAM_FS_SPD in steps/s, range 7.63 to 15625 steps/s */
	pRsrc->reg.FS_SPD 	= FSSpd_Steps_to_Par(cSPIN_CONF_PARAM_FS_SPD);
#if defined(L6480)
	/* Minimum speed settings to cSPIN_CONF_PARAM_MIN_SPEED in steps/s, range 0 to 976.3 steps/s */
	pRsrc->reg.MIN_SPEED	= cSPIN_CONF_PARAM_LSPD_BIT|MinSpd_Steps_to_Par(cSPIN_CONF_PARAM_MIN_SPEED);
  /* Acceleration duty cycle (torque) settings to cSPIN_CONF_PARAM_KVAL_ACC in %, range 0 to 99.6% */
	pRsrc->reg.KVAL_ACC 	= Kval_Perc_to_Par(cSPIN_CONF_PARAM_KVAL_ACC);
  /* Deceleration duty cycle (torque) settings to cSPIN_CONF_PARAM_KVAL_DEC in %, range 0 to 99.6% */
	pRsrc->reg.KVAL_DEC 	= Kval_Perc_to_Par(cSPIN_CONF_PARAM_KVAL_DEC);		
  /* Run duty cycle (torque) settings to cSPIN_CONF_PARAM_KVAL_RUN in %, range 0 to 99.6% */
	pRsrc->reg.KVAL_RUN 	= Kval_Perc_to_Par(cSPIN_CONF_PARAM_KVAL_RUN);
  /* Hold duty cycle (torque) settings to cSPIN_CONF_PARAM_KVAL_HOLD in %, range 0 to 99.6% */
	pRsrc->reg.KVAL_HOLD 	= Kval_Perc_to_Par(cSPIN_CONF_PARAM_KVAL_HOLD);
	/* Thermal compensation param settings to cSPIN_CONF_PARAM_K_THERM, range 1 to 1.46875 */
	pRsrc->reg.K_THERM 	= KTherm_to_Par(cSPIN_CONF_PARAM_K_THERM);
	/* Intersect speed settings for BEMF compensation to cSPIN_CONF_PARAM_INT_SPD in steps/s, range 0 to 3906 steps/s */
	pRsrc->reg.INT_SPD 	= IntSpd_Steps_to_Par(cSPIN_CONF_PARAM_INT_SPD);
	/* BEMF start slope settings for BEMF compensation to cSPIN_CONF_PARAM_ST_SLP in % step/s, range 0 to 0.4% s/step */
	pRsrc->reg.ST_SLP 	= BEMF_Slope_Perc_to_Par(cSPIN_CONF_PARAM_ST_SLP);
	/* BEMF final acc slope settings for BEMF compensation to cSPIN_CONF_PARAM_FN_SLP_ACC in% step/s, range 0 to 0.4% s/step */
	pRsrc->reg.FN_SLP_ACC = BEMF_Slope_Perc_to_Par(cSPIN_CONF_PARAM_FN_SLP_ACC);
	/* BEMF final dec slope settings for BEMF compensation to cSPIN_CONF_PARAM_FN_SLP_DEC in% step/s, range 0 to 0.4% s/step */
	pRsrc->reg.FN_SLP_DEC = BEMF_Slope_Perc_to_Par(cSPIN_CONF_PARAM_FN_SLP_DEC);
	/* Stall threshold settings to cSPIN_CONF_PARAM_STALL_TH in mV, range 31.25 to 1000mV */
	pRsrc->reg.STALL_TH 	= StallTh_to_Par(cSPIN_CONF_PARAM_STALL_TH);
  /* Set Config register according to config parameters */
  /* clock setting, switch hard stop interrupt mode, */
  /*  supply voltage compensation, overcurrent shutdown */
  /* UVLO threshold, VCC reg output voltage , PWM frequency */
	pRsrc->reg.CONFIG 	= (uint16_t)cSPIN_CONF_PARAM_CLOCK_SETTING | \
                        (uint16_t)cSPIN_CONF_PARAM_SW_MODE	  | \
                        (uint16_t)cSPIN_CONF_PARAM_VS_COMP		| \
                        (uint16_t)cSPIN_CONF_PARAM_OC_SD      | \
                        (uint16_t)cSPIN_CONF_PARAM_UVLOVAL    | \
                        (uint16_t)cSPIN_CONF_PARAM_VCCVAL	   	| \
                        (uint16_t)cSPIN_CONF_PARAM_PWM_DIV    | \
                        (uint16_t)cSPIN_CONF_PARAM_PWM_MUL;
#endif /* defined(L6480) */
#if defined(L6482)
        /* Minimum speed settings to cSPIN_CONF_PARAM_MIN_SPEED in steps/s, range 0 to 976.3 steps/s */
	pRsrc->reg.MIN_SPEED	= MinSpd_Steps_to_Par(cSPIN_CONF_PARAM_MIN_SPEED);
        /* Reference voltage assigned to the torque regulation DAC during motor acceleration, range 7.8mV to 1000mV */
	pRsrc->reg.TVAL_ACC 	= Tval_Current_to_Par(cSPIN_CONF_PARAM_TVAL_ACC);
        /* Reference voltage assigned to the torque regulation DAC during motor deceleration, range 7.8mV to 1000mV */
	pRsrc->reg.TVAL_DEC 	= Tval_Current_to_Par(cSPIN_CONF_PARAM_TVAL_DEC);		
        /* Reference voltage assigned to the torque regulation DAC when motor is running, range 7.8mV to 1000mV */
	pRsrc->reg.TVAL_RUN 	= Tval_Current_to_Par(cSPIN_CONF_PARAM_TVAL_RUN);
        /* Reference voltage assigned to the torque regulation DAC when motor is stopped, range 7.8mV to 1000mV */
	pRsrc->reg.TVAL_HOLD 	= Tval_Current_to_Par(cSPIN_CONF_PARAM_TVAL_HOLD);
				/* Maximum fast decay and fall step times used by the current control system, range 2us to 32us */
	pRsrc->reg.T_FAST 	= (uint8_t)cSPIN_CONF_PARAM_TOFF_FAST | (uint8_t)cSPIN_CONF_PARAM_FAST_STEP;
				/* Minimum ON time value used by the current control system, range 0.5us to 64us */
	pRsrc->reg.TON_MIN 	= Tmin_Time_to_Par(cSPIN_CONF_PARAM_TON_MIN);
				/* Minimum OFF time value used by the current control system, range 0.5us to 64us */
	pRsrc->reg.TOFF_MIN	= Tmin_Time_to_Par(cSPIN_CONF_PARAM_TOFF_MIN);
				/* Set Config register according to config parameters */
				/* clock setting, switch hard stop interrupt mode, */
				/*  supply voltage compensation, overcurrent shutdown */
        /* UVLO threshold, VCC reg output voltage , target switching period, predictive current control */
	pRsrc->reg.CONFIG 	= (uint16_t)cSPIN_CONF_PARAM_CLOCK_SETTING | \
                                          (uint16_t)cSPIN_CONF_PARAM_SW_MODE	   | \
                                          (uint16_t)cSPIN_CONF_PARAM_TQ_REG        | \
                                          (uint16_t)cSPIN_CONF_PARAM_OC_SD         | \
                                          (uint16_t)cSPIN_CONF_PARAM_UVLOVAL       | \
                                          (uint16_t)cSPIN_CONF_PARAM_VCCVAL	   | \
                                          (uint16_t)cSPIN_CONF_PARAM_TSW           | \
                                          (uint16_t)cSPIN_CONF_PARAM_PRED;        
#endif /* defined(L6482) */
	/* Overcurrent threshold settings to cSPIN_CONF_PARAM_OCD_TH, range 31.25 to 1000mV */
	pRsrc->reg.OCD_TH 	= cSPIN_CONF_PARAM_OCD_TH;        
  /* Alarm settings to cSPIN_CONF_PARAM_ALARM_EN */
	pRsrc->reg.ALARM_EN 	= cSPIN_CONF_PARAM_ALARM_EN;
  /* Step mode and sycn mode settings via cSPIN_CONF_PARAM_SYNC_MODE and cSPIN_CONF_PARAM_STEP_MODE */
	pRsrc->reg.STEP_MODE 	= (uint8_t)cSPIN_CONF_PARAM_SYNC_MODE | \
                          (uint8_t)cSPIN_CONF_PARAM_STEP_MODE;
  /* Sink/source current, duration of constant current phases, duration of overboost phase settings */
  pRsrc->reg.GATECFG1   = (uint16_t)cSPIN_CONF_PARAM_IGATE | \
													(uint16_t)cSPIN_CONF_PARAM_TCC   | \
													(uint16_t)cSPIN_CONF_PARAM_TBOOST;
	/* Blank time, Dead time stiings */
	 pRsrc->reg.GATECFG2  = (uint16_t)cSPIN_CONF_PARAM_TBLANK | \
													(uint16_t)cSPIN_CONF_PARAM_TDT;
  /* Program all cSPIN registers */
	cSPIN_Registers_Set(pRsrc);
             
	/* Clear Flag pin */
	pRsrc->reg.STATUS = cSPIN_Get_Status(pRsrc); 
}

/**
  * @brief  Issues cSPIN NOP command.
  * @param  None
  * @retval None
  */
//void cSPIN_Nop(cSPIN_Rsrc_T* pRsrc)
//{
//	/* Send NOP operation code to cSPIN */
//	cSPIN_Write_Byte(pRsrc,cSPIN_NOP);
//}

/**
  * @brief  Issues cSPIN Set Param command.
  * @param  param cSPIN register address
  * @param  value to be set
  * @retval None
  */
void cSPIN_Set_Param(cSPIN_Rsrc_T* pRsrc, cSPIN_Registers_TypeDef param, uint32_t value)
{
	/* Send SetParam operation code to cSPIN */
	cSPIN_Write_Byte(pRsrc, (uint8_t)cSPIN_SET_PARAM | (uint8_t)param);
	switch (param)
	{
		/* These following REG need 3 bytes */
		case cSPIN_ABS_POS: ;
		case cSPIN_MARK: ;
			/* Send parameter - byte 2 to cSPIN */
			cSPIN_Write_Byte(pRsrc, (uint8_t)(value >> 16));
		/* These following REG need 2 bytes */
		case cSPIN_EL_POS: ;
		case cSPIN_ACC: ;
		case cSPIN_DEC: ;
		case cSPIN_MAX_SPEED: ;
		case cSPIN_MIN_SPEED: ;
		case cSPIN_FS_SPD: ;
#if defined(L6480)
		case cSPIN_INT_SPD: ;
#endif /* defined(L6480) */
		case cSPIN_CONFIG: ;
		case cSPIN_STATUS:
			/* Send parameter - byte 1 to cSPIN */
		   	cSPIN_Write_Byte(pRsrc, (uint8_t)(value >> 8));
		/* These following REG need 1 bytes */
		default:
			/* Send parameter - byte 0 to cSPIN */
		   	cSPIN_Write_Byte(pRsrc, (uint8_t)(value));
	}
}

/**
  * @brief  Issues cSPIN Get Param command.
  * @param  param cSPIN register address
  * @retval Register value - 1 to 3 bytes (depends on register)
  */
uint32_t cSPIN_Get_Param(cSPIN_Rsrc_T* pRsrc, cSPIN_Registers_TypeDef param)
{
	uint32_t temp = 0;
	uint32_t rx = 0;

	/* Send GetParam operation code to cSPIN */
	temp = cSPIN_Write_Byte(pRsrc, (uint8_t)cSPIN_GET_PARAM | (uint8_t)param);
	/* MSB which should be 0 */
	temp = temp << 24;
	rx |= temp;
	switch (param)
	{
		case cSPIN_ABS_POS: ;
		case cSPIN_MARK: ;
		case cSPIN_SPEED:
		  temp = cSPIN_Write_Byte(pRsrc, (uint8_t)(0x00));
			temp = temp << 16;
			rx |= temp;
		case cSPIN_EL_POS: ;
		case cSPIN_ACC: ;
		case cSPIN_DEC: ;
		case cSPIN_MAX_SPEED: ;
		case cSPIN_MIN_SPEED: ;
		case cSPIN_FS_SPD: ;
#if defined(L6480)
		case cSPIN_INT_SPD: ;
#endif /* defined(L6480) */
		case cSPIN_CONFIG: ;
		case cSPIN_STATUS:
		   	temp = cSPIN_Write_Byte(pRsrc, (uint8_t)(0x00));
			temp = temp << 8;
			rx |= temp;
		default:
		   	temp = cSPIN_Write_Byte(pRsrc, (uint8_t)(0x00));
			rx |= temp;
	}

	return rx;
}

/**
  * @brief  Issues cSPIN Run command.
  * @param  direction Movement direction (FWD, REV)
  * @param  speed over 3 bytes
  * @retval None
  */
void cSPIN_Run(cSPIN_Rsrc_T* pRsrc, cSPIN_Direction_TypeDef direction, uint32_t speed)
{
	/* Send RUN operation code to cSPIN */
	cSPIN_Write_Byte(pRsrc, (uint8_t)cSPIN_RUN | (uint8_t)direction);
	/* Send speed - byte 2 data cSPIN */
	cSPIN_Write_Byte(pRsrc, (uint8_t)(speed >> 16));
	/* Send speed - byte 1 data cSPIN */
	cSPIN_Write_Byte(pRsrc, (uint8_t)(speed >> 8));
	/* Send speed - byte 0 data cSPIN */
	cSPIN_Write_Byte(pRsrc, (uint8_t)(speed));
}

/**
  * @brief  Issues cSPIN Step Clock command.
  * @param  direction Movement direction (FWD, REV)
  * @retval None
  */
void cSPIN_Step_Clock(cSPIN_Rsrc_T* pRsrc, cSPIN_Direction_TypeDef direction)
{
	/* Send StepClock operation code to cSPIN */
	cSPIN_Write_Byte(pRsrc, (uint8_t)cSPIN_STEP_CLOCK | (uint8_t)direction);
}

/**
  * @brief  Issues cSPIN Move command.
  * @param  direction Movement direction
  * @param  n_step number of steps
  * @retval None
  */
void cSPIN_Move(cSPIN_Rsrc_T* pRsrc, cSPIN_Direction_TypeDef direction, uint32_t n_step)
{
	/* Send Move operation code to cSPIN */
	cSPIN_Write_Byte(pRsrc, (uint8_t)cSPIN_MOVE | (uint8_t)direction);
	/* Send n_step - byte 2 data cSPIN */
	cSPIN_Write_Byte(pRsrc, (uint8_t)(n_step >> 16));
	/* Send n_step - byte 1 data cSPIN */
	cSPIN_Write_Byte(pRsrc, (uint8_t)(n_step >> 8));
	/* Send n_step - byte 0 data cSPIN */
	cSPIN_Write_Byte(pRsrc, (uint8_t)(n_step));
}

/**
  * @brief  Issues cSPIN Go To command.
  * @param  abs_pos absolute position where requested to move
  * @retval None
  */
void cSPIN_Go_To(cSPIN_Rsrc_T* pRsrc, uint32_t abs_pos)
{
	/* Send GoTo operation code to cSPIN */
	cSPIN_Write_Byte(pRsrc, cSPIN_GO_TO);
	/* Send absolute position parameter - byte 2 data to cSPIN */
	cSPIN_Write_Byte(pRsrc, (uint8_t)(abs_pos >> 16));
	/* Send absolute position parameter - byte 1 data to cSPIN */
	cSPIN_Write_Byte(pRsrc, (uint8_t)(abs_pos >> 8));
	/* Send absolute position parameter - byte 0 data to cSPIN */
	cSPIN_Write_Byte(pRsrc, (uint8_t)(abs_pos));
}

/**
  * @brief  Issues cSPIN Go To Dir command.
  * @param  direction movement direction
  * @param  abs_pos absolute position where requested to move
  * @retval None
  */
void cSPIN_Go_To_Dir(cSPIN_Rsrc_T* pRsrc, cSPIN_Direction_TypeDef direction, uint32_t abs_pos)
{
	/* Send GoTo_DIR operation code to cSPIN */
	cSPIN_Write_Byte(pRsrc, (uint8_t)cSPIN_GO_TO_DIR | (uint8_t)direction);
	/* Send absolute position parameter - byte 2 data to cSPIN */
	cSPIN_Write_Byte(pRsrc, (uint8_t)(abs_pos >> 16));
	/* Send absolute position parameter - byte 1 data to cSPIN */
	cSPIN_Write_Byte(pRsrc, (uint8_t)(abs_pos >> 8));
	/* Send absolute position parameter - byte 0 data to cSPIN */
	cSPIN_Write_Byte(pRsrc, (uint8_t)(abs_pos));
}

/**
  * @brief  Issues cSPIN Go Until command.
  * @param  action
  * @param  direction movement direction
  * @param  speed
  * @retval None
  */
void cSPIN_Go_Until(cSPIN_Rsrc_T* pRsrc, cSPIN_Action_TypeDef action, cSPIN_Direction_TypeDef direction, uint32_t speed)
{
	/* Send GoUntil operation code to cSPIN */
	cSPIN_Write_Byte(pRsrc, (uint8_t)cSPIN_GO_UNTIL | (uint8_t)action | (uint8_t)direction);
	/* Send speed parameter - byte 2 data to cSPIN */
	cSPIN_Write_Byte(pRsrc, (uint8_t)(speed >> 16));
	/* Send speed parameter - byte 1 data to cSPIN */
	cSPIN_Write_Byte(pRsrc, (uint8_t)(speed >> 8));
	/* Send speed parameter - byte 0 data to cSPIN */
	cSPIN_Write_Byte(pRsrc, (uint8_t)(speed));
}

/**
  * @brief  Issues cSPIN Release SW command.
  * @param  action
  * @param  direction movement direction
  * @retval None
  */
void cSPIN_Release_SW(cSPIN_Rsrc_T* pRsrc, cSPIN_Action_TypeDef action, cSPIN_Direction_TypeDef direction)
{
	/* Send ReleaseSW operation code to cSPIN */
	cSPIN_Write_Byte(pRsrc, (uint8_t)cSPIN_RELEASE_SW | (uint8_t)action | (uint8_t)direction);
}

/**
  * @brief  Issues cSPIN Go Home command. (Shorted path to zero position)
  * @param  None
  * @retval None
  */
void cSPIN_Go_Home(cSPIN_Rsrc_T* pRsrc)
{
	/* Send GoHome operation code to cSPIN */
	cSPIN_Write_Byte(pRsrc, cSPIN_GO_HOME);
}

/**
  * @brief  Issues cSPIN Go Mark command.
  * @param  None
  * @retval None
  */
void cSPIN_Go_Mark(cSPIN_Rsrc_T* pRsrc)
{
	/* Send GoMark operation code to cSPIN */
	cSPIN_Write_Byte(pRsrc, cSPIN_GO_MARK);
}

/**
  * @brief  Issues cSPIN Reset Pos command.
  * @param  None
  * @retval None
  */
void cSPIN_Reset_Pos(cSPIN_Rsrc_T* pRsrc)
{
	/* Send ResetPos operation code to cSPIN */
	cSPIN_Write_Byte(pRsrc, cSPIN_RESET_POS);
}

/**
  * @brief  Issues cSPIN Reset Device command.
  * @param  None
  * @retval None
  */
void cSPIN_Reset_Device(cSPIN_Rsrc_T* pRsrc)
{
	/* Send ResetDevice operation code to cSPIN */
	cSPIN_Write_Byte(pRsrc, cSPIN_RESET_DEVICE);
}

/**
  * @brief  Issues cSPIN Soft Stop command.
  * @param  None
  * @retval None
  */
void cSPIN_Soft_Stop(cSPIN_Rsrc_T* pRsrc)
{
	/* Send SoftStop operation code to cSPIN */
	cSPIN_Write_Byte(pRsrc, cSPIN_SOFT_STOP);
}

/**
  * @brief  Issues cSPIN Hard Stop command.
  * @param  None
  * @retval None
  */
void cSPIN_Hard_Stop(cSPIN_Rsrc_T* pRsrc)
{
	/* Send HardStop operation code to cSPIN */
	cSPIN_Write_Byte(pRsrc, cSPIN_HARD_STOP);
}

/**
  * @brief  Issues cSPIN Soft HiZ command.
  * @param  None
  * @retval None
  */
void cSPIN_Soft_HiZ(cSPIN_Rsrc_T* pRsrc)
{
	/* Send SoftHiZ operation code to cSPIN */
	cSPIN_Write_Byte(pRsrc, cSPIN_SOFT_HIZ);
}

/**
  * @brief  Issues cSPIN Hard HiZ command.
  * @param  None
  * @retval None
  */
void cSPIN_Hard_HiZ(cSPIN_Rsrc_T* pRsrc)
{
	/* Send HardHiZ operation code to cSPIN */
	cSPIN_Write_Byte(pRsrc, cSPIN_HARD_HIZ);
}

/**
  * @brief  Issues cSPIN Get Status command.
  * @param  None
  * @retval Status Register content
  */
uint16_t cSPIN_Get_Status(cSPIN_Rsrc_T* pRsrc)
{
	uint16_t temp = 0;
	uint16_t rx = 0;

	/* Send GetStatus operation code to cSPIN */
	cSPIN_Write_Byte(pRsrc, cSPIN_GET_STATUS);
	/* Send zero byte / receive MSByte from cSPIN */
	temp = cSPIN_Write_Byte(pRsrc, (uint8_t)(0x00));
	temp = temp << 8;
	rx |= temp;
	/* Send zero byte / receive LSByte from cSPIN */
	temp = cSPIN_Write_Byte(pRsrc, (uint8_t)(0x00));
	rx |= temp;
	return rx;
}

/**
  * @brief  Checks if the cSPIN is Busy by hardware - active Busy signal.
  * @param  None
  * @retval one if chip is busy, otherwise zero
  */
uint8_t cSPIN_Busy_HW(cSPIN_Rsrc_T* pRsrc)
{
	if(HAL_GPIO_ReadPin(pRsrc->nBusy_sync.GPIOx, pRsrc->nBusy_sync.GPIO_Pin) == GPIO_PIN_RESET) return 0x01;
	else return 0x00;
}

/**
  * @brief  Checks if the cSPIN is Busy by SPI - Busy flag bit in Status Register.
  * @param  None
  * @retval one if chip is busy, otherwise zero
  */
uint8_t cSPIN_Busy_SW(cSPIN_Rsrc_T* pRsrc)
{
	if(!(cSPIN_Get_Status(pRsrc) & cSPIN_STATUS_BUSY)) return 0x01;
	else return 0x00;
}

/**
  * @brief  Checks cSPIN Flag signal.
  * @param  None
  * @retval one if Flag signal is active, otherwise zero
  */
uint8_t cSPIN_Flag(cSPIN_Rsrc_T* pRsrc)
{
	if(HAL_GPIO_ReadPin(pRsrc->nFlag.GPIOx, pRsrc->nFlag.GPIO_Pin) == GPIO_PIN_RESET)	return 0x01;
	else return 0x00;
}


/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Inserts a delay time.
  * @param  nCount specifies the delay time length.
  * @retval None
  */
void cSPIN_Delay(__IO uint32_t nCount)
{
  for(; nCount!= 0;nCount--);
}

/**
  * @brief  Transmits/Receives one byte to/from cSPIN over SPI.
  * @param  byte Transmited byte
  * @retval Received byte
  */
uint8_t cSPIN_Write_Byte(cSPIN_Rsrc_T* pRsrc, uint8_t byte)
{
	uint8_t rtn;
	/* nSS signal activation - low */
	HAL_GPIO_WritePin(pRsrc->nSel.GPIOx, pRsrc->nSel.GPIO_Pin,GPIO_PIN_RESET);
	/* SPI byte send */
	while (HAL_SPI_GetState(pRsrc->SPI_HANDLE) != HAL_SPI_STATE_READY){ }
	HAL_SPI_TransmitReceive(pRsrc->SPI_HANDLE, &byte, &rtn, 1, HAL_MAX_DELAY);		
	/* nSS signal deactivation - high */
	HAL_GPIO_WritePin(pRsrc->nSel.GPIOx, pRsrc->nSel.GPIO_Pin,GPIO_PIN_SET);
	return rtn;
}

/**
  * @brief  Resets CSPIN and puts it into standby mode
  * @param  None
  * @retval None
  */
void cSPIN_Reset_And_Standby(cSPIN_Rsrc_T* pRsrc)
{
	HAL_GPIO_WritePin(pRsrc->nStby_nReset.GPIOx, pRsrc->nStby_nReset.GPIO_Pin,GPIO_PIN_RESET);
  cSPIN_Delay(10000);
  HAL_GPIO_WritePin(pRsrc->nStby_nReset.GPIOx, pRsrc->nStby_nReset.GPIO_Pin,GPIO_PIN_SET);
}

/**
  * @brief  Fills-in cSPIN configuration structure with default values.
  * @param  cSPIN_RegsStruct structure address (pointer to struct)
  * @retval None
  */
void cSPIN_Regs_Struct_Reset(cSPIN_Rsrc_T* pRsrc)
{
	pRsrc->reg.ABS_POS		= 0;
	pRsrc->reg.EL_POS 		= 0;
	pRsrc->reg.MARK 			= 0;
	pRsrc->reg.ACC 				= 0x08A;
	pRsrc->reg.DEC 				= 0x08A;
	pRsrc->reg.MAX_SPEED 	= 0x041;
	pRsrc->reg.MIN_SPEED 	= 0;
	pRsrc->reg.FS_SPD 		= 0x027;
#if defined(L6480)
	pRsrc->reg.KVAL_HOLD 	= 0x29;
	pRsrc->reg.KVAL_RUN 	= 0x29;
	pRsrc->reg.KVAL_ACC 	= 0x29;
	pRsrc->reg.KVAL_DEC 	= 0x29;
	pRsrc->reg.INT_SPD 		= 0x0408;
	pRsrc->reg.ST_SLP 		= 0x19;
	pRsrc->reg.FN_SLP_ACC = 0x29;
	pRsrc->reg.FN_SLP_DEC = 0x29;
	pRsrc->reg.K_THERM 		= 0;
	pRsrc->reg.STALL_TH 	= 0x10;
#endif /* defined(L6480) */
#if defined(L6482)
	pRsrc->reg.TVAL_HOLD 	= 0x29;
	pRsrc->reg.TVAL_RUN 	= 0x29;
	pRsrc->reg.TVAL_ACC 	= 0x29;
	pRsrc->reg.TVAL_DEC 	= 0x29;
	pRsrc->reg.T_FAST 		= 0x19;
	pRsrc->reg.TON_MIN 		= 0x29;
	pRsrc->reg.TOFF_MIN 	= 0x29;
#endif /* defined(L6482) */
	pRsrc->reg.OCD_TH 		= 0x8;
	pRsrc->reg.STEP_MODE 	= 0x7;
	pRsrc->reg.ALARM_EN 	= 0xFF;
	pRsrc->reg.GATECFG1 	= 0;
	pRsrc->reg.GATECFG2 	= 0;
	pRsrc->reg.CONFIG 		= 0x2C88;
}
 
/**
  * @brief  Configures cSPIN internal registers with values in the config structure.
  * @param  cSPIN_RegsStruct Configuration structure address (pointer to configuration structure)
  * @retval None
  */
void cSPIN_Registers_Set(cSPIN_Rsrc_T* pRsrc)
{
	cSPIN_Set_Param(pRsrc, cSPIN_ABS_POS, pRsrc->reg.ABS_POS);
	cSPIN_Set_Param(pRsrc, cSPIN_EL_POS, pRsrc->reg.EL_POS);
	cSPIN_Set_Param(pRsrc, cSPIN_MARK, pRsrc->reg.MARK);
	cSPIN_Set_Param(pRsrc, cSPIN_ACC, pRsrc->reg.ACC);
	cSPIN_Set_Param(pRsrc, cSPIN_DEC, pRsrc->reg.DEC);
	cSPIN_Set_Param(pRsrc, cSPIN_MAX_SPEED, pRsrc->reg.MAX_SPEED);
	cSPIN_Set_Param(pRsrc, cSPIN_MIN_SPEED, pRsrc->reg.MIN_SPEED);
	cSPIN_Set_Param(pRsrc, cSPIN_FS_SPD, pRsrc->reg.FS_SPD);
#if defined(L6480)
	cSPIN_Set_Param(pRsrc, cSPIN_KVAL_HOLD, pRsrc->reg.KVAL_HOLD);
	cSPIN_Set_Param(pRsrc, cSPIN_KVAL_RUN, pRsrc->reg.KVAL_RUN);
	cSPIN_Set_Param(pRsrc, cSPIN_KVAL_ACC, pRsrc->reg.KVAL_ACC);
	cSPIN_Set_Param(pRsrc, cSPIN_KVAL_DEC, pRsrc->reg.KVAL_DEC);
	cSPIN_Set_Param(pRsrc, cSPIN_INT_SPD, pRsrc->reg.INT_SPD);
	cSPIN_Set_Param(pRsrc, cSPIN_ST_SLP, pRsrc->reg.ST_SLP);
	cSPIN_Set_Param(pRsrc, cSPIN_FN_SLP_ACC, pRsrc->reg.FN_SLP_ACC);
	cSPIN_Set_Param(pRsrc, cSPIN_FN_SLP_DEC, pRsrc->reg.FN_SLP_DEC);
	cSPIN_Set_Param(pRsrc, cSPIN_K_THERM, pRsrc->reg.K_THERM);
	cSPIN_Set_Param(pRsrc, cSPIN_STALL_TH, pRsrc->reg.STALL_TH);
#endif /* defined(L6480) */
#if defined(L6482)
	cSPIN_Set_Param(pRsrc, cSPIN_TVAL_HOLD, pRsrc->reg.TVAL_HOLD);
	cSPIN_Set_Param(pRsrc, cSPIN_TVAL_RUN, pRsrc->reg.TVAL_RUN);
	cSPIN_Set_Param(pRsrc, cSPIN_TVAL_ACC, pRsrc->reg.TVAL_ACC);
	cSPIN_Set_Param(pRsrc, cSPIN_TVAL_DEC, pRsrc->reg.TVAL_DEC);
	cSPIN_Set_Param(pRsrc, cSPIN_T_FAST, pRsrc->reg.T_FAST);
	cSPIN_Set_Param(cSPIN_TON_MIN, pRsrc->reg.TON_MIN);
	cSPIN_Set_Param(pRsrc, cpRsrc, SPIN_TOFF_MIN, pRsrc->reg.TOFF_MIN);
#endif /* defined(L6482) */
	cSPIN_Set_Param(pRsrc, cSPIN_OCD_TH, pRsrc->reg.OCD_TH);
	cSPIN_Set_Param(pRsrc, cSPIN_STEP_MODE, pRsrc->reg.STEP_MODE);
	cSPIN_Set_Param(pRsrc, cSPIN_ALARM_EN, pRsrc->reg.ALARM_EN);
	cSPIN_Set_Param(pRsrc, cSPIN_GATECFG1, pRsrc->reg.GATECFG1);
	cSPIN_Set_Param(pRsrc, cSPIN_GATECFG2, pRsrc->reg.GATECFG2);
	cSPIN_Set_Param(pRsrc, cSPIN_CONFIG, pRsrc->reg.CONFIG);
}


/** @} */  
/******************* (C) COPYRIGHT 2013 STMicroelectronics *****END OF FILE****/
