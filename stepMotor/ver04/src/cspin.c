/**
  ******************************************************************************
  * @file    cspin.c 
  * @author  IPC Rennes
  * @version V2.1
  * @date    October 15, 2013
  * @brief   cSPIN (motor and L6482) product related routines
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
void cSPIN_Delay(__IO u32 nCount);
void cSPIN_Reset_And_Standby(cSPIN_Rsrc_T* pRsrc);
void cSPIN_Regs_Struct_Reset(cSPIN_Rsrc_T* pRsrc);
void cSPIN_Registers_SetAll(cSPIN_Rsrc_T* pRsrc);
void cSPIN_Registers_GetAll(cSPIN_Rsrc_T* pRsrc);
void cSPIN_Registers_GetFew(cSPIN_Rsrc_T* pRsrc);
void cSPIN_Reset_Device(cSPIN_Rsrc_T* pRsrc);
u8 cSPIN_Write_Byte(cSPIN_Rsrc_T* pRsrc, u8 byte);

/* commands(19) --------------------------------------------------------------*/
void cSPIN_Set_Param			(cSPIN_Rsrc_T* pRsrc, cSPIN_Registers_TypeDef param, uint32_t value);
uint32_t cSPIN_Get_Param	(cSPIN_Rsrc_T* pRsrc, cSPIN_Registers_TypeDef param);

bool isCmdErr						(cSPIN_Rsrc_T* pRsrc);
bool cSPIN_SetAbsPos		(cSPIN_Rsrc_T* pRsrc, u32 value);
bool cSPIN_GetAbsPos		(cSPIN_Rsrc_T* pRsrc, u32 *value);
bool cSPIN_SetAbsPos		(cSPIN_Rsrc_T* pRsrc, u32 param);
bool cSPIN_GetAbsPos		(cSPIN_Rsrc_T* pRsrc, u32 *param);
bool cSPIN_SetElPos			(cSPIN_Rsrc_T* pRsrc, u16 param);
bool cSPIN_GetElPos			(cSPIN_Rsrc_T* pRsrc, u16 *param);
bool cSPIN_SetMarkPos		(cSPIN_Rsrc_T* pRsrc, u32 param);
bool cSPIN_GetMarkPos		(cSPIN_Rsrc_T* pRsrc, u32 *param);
bool cSPIN_GetSpeed			(cSPIN_Rsrc_T* pRsrc, u32 *param);
bool cSPIN_SetAcc				(cSPIN_Rsrc_T* pRsrc, u16 param);
bool cSPIN_GetAcc				(cSPIN_Rsrc_T* pRsrc, u16 *param);
bool cSPIN_SetDec				(cSPIN_Rsrc_T* pRsrc, u16 param);
bool cSPIN_GetDec				(cSPIN_Rsrc_T* pRsrc, u16 *param);
bool cSPIN_SetMaxSpd		(cSPIN_Rsrc_T* pRsrc, u16 param);
bool cSPIN_GetMaxSpd		(cSPIN_Rsrc_T* pRsrc, u16 *param);
bool cSPIN_SetMinSpd		(cSPIN_Rsrc_T* pRsrc, u16 param);
bool cSPIN_GetMinSpd		(cSPIN_Rsrc_T* pRsrc, u16 *param);
bool cSPIN_SetLowSpdOpt	(cSPIN_Rsrc_T* pRsrc, bool param);
bool cSPIN_GetLowSpdOpt	(cSPIN_Rsrc_T* pRsrc, bool *param);
bool cSPIN_SetFSSpd			(cSPIN_Rsrc_T* pRsrc, u16 param);
bool cSPIN_GetFSSpd			(cSPIN_Rsrc_T* pRsrc, u16 *param);
bool cSPIN_SetBoostMode	(cSPIN_Rsrc_T* pRsrc, bool param);
bool cSPIN_GetBoostMode	(cSPIN_Rsrc_T* pRsrc, bool *param);
bool cSPIN_SetKValHold	(cSPIN_Rsrc_T* pRsrc, u8 param);
bool cSPIN_GetKValHold	(cSPIN_Rsrc_T* pRsrc, u8 *param);
bool cSPIN_SetKValRun		(cSPIN_Rsrc_T* pRsrc, u8 param);
bool cSPIN_GetKValRun		(cSPIN_Rsrc_T* pRsrc, u8 *param);
bool cSPIN_SetKValAcc		(cSPIN_Rsrc_T* pRsrc, u8 param);
bool cSPIN_GetKValAcc		(cSPIN_Rsrc_T* pRsrc, u8 *param);
bool cSPIN_SetKValDec		(cSPIN_Rsrc_T* pRsrc, u8 param);
bool cSPIN_GetKValDec		(cSPIN_Rsrc_T* pRsrc, u8 *param);
bool cSPIN_SetIntSpd		(cSPIN_Rsrc_T* pRsrc, u16 param);
bool cSPIN_GetIntSpd		(cSPIN_Rsrc_T* pRsrc, u16 *param);
bool cSPIN_SetStartSlp	(cSPIN_Rsrc_T* pRsrc, u8 param);
bool cSPIN_GetStartSlp	(cSPIN_Rsrc_T* pRsrc, u8 *param);
bool cSPIN_SetFnSlpAcc	(cSPIN_Rsrc_T* pRsrc, u8 param);
bool cSPIN_GetFnSlpAcc	(cSPIN_Rsrc_T* pRsrc, u8 *param);
bool cSPIN_SetFnSlpDec	(cSPIN_Rsrc_T* pRsrc, u8 param);
bool cSPIN_GetFnSlpDec	(cSPIN_Rsrc_T* pRsrc, u8 *param);
bool cSPIN_SetKTherm		(cSPIN_Rsrc_T* pRsrc, u8 param);
bool cSPIN_GetKTherm		(cSPIN_Rsrc_T* pRsrc, u8 *param);
bool cSPIN_GetAdcOut		(cSPIN_Rsrc_T* pRsrc, u8 *param);
bool cSPIN_SetOCDTh			(cSPIN_Rsrc_T* pRsrc, u8 param);
bool cSPIN_GetOCDTh			(cSPIN_Rsrc_T* pRsrc, u8 *param);
bool cSPIN_SetStallTh		(cSPIN_Rsrc_T* pRsrc, u8 param);
bool cSPIN_GetStallTh		(cSPIN_Rsrc_T* pRsrc, u8 *param);
bool cSPIN_SetStepSel		(cSPIN_Rsrc_T* pRsrc, u8 param);
bool cSPIN_GetStepSel		(cSPIN_Rsrc_T* pRsrc, u8 *param);
bool cSPIN_SetSyncSel		(cSPIN_Rsrc_T* pRsrc, u8 param);
bool cSPIN_GetSyncSel		(cSPIN_Rsrc_T* pRsrc, u8 *param);
bool cSPIN_SetSyncEn		(cSPIN_Rsrc_T* pRsrc, u8 param);
bool cSPIN_GetSyncEn		(cSPIN_Rsrc_T* pRsrc, u8 *param);
bool cSPIN_SetGateTcc		(cSPIN_Rsrc_T* pRsrc, u8 param);
bool cSPIN_GetGateTcc		(cSPIN_Rsrc_T* pRsrc, u8 *param);
bool cSPIN_SetGateCurrent		(cSPIN_Rsrc_T* pRsrc, u8 param);
bool cSPIN_GetGateCurrent		(cSPIN_Rsrc_T* pRsrc, u8 *param);
bool cSPIN_SetGateTBoost		(cSPIN_Rsrc_T* pRsrc, u8 param);
bool cSPIN_GetGateTBoost		(cSPIN_Rsrc_T* pRsrc, u8 *param);
bool cSPIN_SetGateTDeadTime	(cSPIN_Rsrc_T* pRsrc, u8 param);
bool cSPIN_GetGateTDeadTime	(cSPIN_Rsrc_T* pRsrc, u8 *param);
bool cSPIN_SetGateTBlank		(cSPIN_Rsrc_T* pRsrc, u8 param);
bool cSPIN_GetGateTBlank		(cSPIN_Rsrc_T* pRsrc, u8 *param);
bool cSPIN_SetSwMode (cSPIN_Rsrc_T* pRsrc, bool param);
bool cSPIN_GetSwMode (cSPIN_Rsrc_T* pRsrc, bool *param);
bool cSPIN_SetVsCompEn (cSPIN_Rsrc_T* pRsrc, bool param);
bool cSPIN_GetVsCompEn (cSPIN_Rsrc_T* pRsrc, bool *param);
bool cSPIN_SetOCSDEn (cSPIN_Rsrc_T* pRsrc, bool param);
bool cSPIN_GetOCSDEn (cSPIN_Rsrc_T* pRsrc, bool *param);
bool cSPIN_SetUVLOVal (cSPIN_Rsrc_T* pRsrc, bool param);
bool cSPIN_GetUVLOVal (cSPIN_Rsrc_T* pRsrc, bool *param);
bool cSPIN_SetVccVal (cSPIN_Rsrc_T* pRsrc, bool param);
bool cSPIN_GetVccVal (cSPIN_Rsrc_T* pRsrc, bool *param);
bool cSPIN_SetPwmFrq (cSPIN_Rsrc_T* pRsrc, u8 param);
bool cSPIN_GetPwmFrq (cSPIN_Rsrc_T* pRsrc, u8 *param);
bool cSPIN_GetStatus (cSPIN_Rsrc_T* pRsrc, u16 *param);

void cSPIN_Run						(cSPIN_Rsrc_T* pRsrc, cSPIN_Direction_T direction, uint32_t speed);
void cSPIN_Step_Clock			(cSPIN_Rsrc_T* pRsrc, cSPIN_Direction_T direction);
void cSPIN_Move						(cSPIN_Rsrc_T* pRsrc, cSPIN_Direction_T direction, uint32_t n_step);
void cSPIN_Go_To					(cSPIN_Rsrc_T* pRsrc, uint32_t abs_pos);
void cSPIN_Go_To_Dir			(cSPIN_Rsrc_T* pRsrc, cSPIN_Direction_T direction, uint32_t abs_pos);
void cSPIN_Go_Until				(cSPIN_Rsrc_T* pRsrc, cSPIN_Action_T action, cSPIN_Direction_T direction, uint32_t speed);
void cSPIN_Release_SW			(cSPIN_Rsrc_T* pRsrc, cSPIN_Action_T action, cSPIN_Direction_T direction);
void cSPIN_Go_Home				(cSPIN_Rsrc_T* pRsrc);
void cSPIN_Go_Mark				(cSPIN_Rsrc_T* pRsrc);
void cSPIN_Reset_Pos			(cSPIN_Rsrc_T* pRsrc);
void cSPIN_Soft_Stop			(cSPIN_Rsrc_T* pRsrc);
void cSPIN_Hard_Stop			(cSPIN_Rsrc_T* pRsrc);
void cSPIN_Soft_HiZ				(cSPIN_Rsrc_T* pRsrc);
void cSPIN_Hard_HiZ				(cSPIN_Rsrc_T* pRsrc);

void cSPIN_tickExe(cSPIN_Rsrc_T* pRsrc);

void cSPIN_EnRptStatus(cSPIN_Rsrc_T* pRsrc);
void cSPIN_DisRptStatus(cSPIN_Rsrc_T* pRsrc);
void cSPIN_EnRptAbsPos(cSPIN_Rsrc_T* pRsrc);
void cSPIN_DisRptAbsPos(cSPIN_Rsrc_T* pRsrc);
void cSPIN_EnRptAdcOut(cSPIN_Rsrc_T* pRsrc);
void cSPIN_DisRptAdcOut(cSPIN_Rsrc_T* pRsrc);

uint16_t cSPIN_Get_Status	(cSPIN_Rsrc_T* pRsrc);
uint8_t cSPIN_Busy_HW(cSPIN_Rsrc_T* pRsrc);
uint8_t cSPIN_Busy_SW(cSPIN_Rsrc_T* pRsrc);
uint8_t cSPIN_Flag(cSPIN_Rsrc_T* pRsrc);

/**
  * @brief  Setup a motor to be ready to run
  * @param  None
  * @retval None
  */
void cSPIN_Setup(
	cSPIN_DEV_T *pDev, 
	const u8* NAME, 
	SPI_HandleTypeDef* SPI_HANDLE,
	PIN_T CS, 	
	PIN_T SW_L,
	PIN_T SW_R,
	PIN_T BUSY,
	PIN_T SCL,
	PIN_T SDA,
	void (*delay)	(u16 ms)
){
	cSPIN_Rsrc_T* pRsrc = &pDev->rsrc;	
	
	if(DEVNAME.Add(pRsrc->name, DEV_NAME_LEN, NAME)<0)	return;
	
	pRsrc->SPI_HANDLE = SPI_HANDLE;
	pRsrc->CS = CS;
	pRsrc->SW_L = SW_L;
	pRsrc->SW_R = SW_R;
	pRsrc->BUSY = BUSY;
	pRsrc->SCL = SCL;
	pRsrc->SDA = SDA;
	
	pRsrc->refLSeries = 0xff;
	pRsrc->refRSeries = 0xff;

	/* register functions */
	pDev->RegSetAbsPos = 	cSPIN_SetAbsPos;
	pDev->RegGetAbsPos = 	cSPIN_GetAbsPos;
	pDev->RegSetElPos = 	cSPIN_SetElPos;
	pDev->RegGetElPos = 	cSPIN_GetElPos;
	pDev->RegSetMarkPos = 	cSPIN_SetMarkPos;
	pDev->RegGetMarkPos = 	cSPIN_GetMarkPos;
	pDev->RegGetSpeed = 	cSPIN_GetSpeed;
	pDev->RegSetAcc = 	cSPIN_SetAcc;
	pDev->RegGetAcc = 	cSPIN_GetAcc;
	pDev->RegSetDec = 	cSPIN_SetDec;
	pDev->RegGetDec = 	cSPIN_GetDec;
	pDev->RegSetMaxSpd = 	cSPIN_SetMaxSpd;
	pDev->RegGetMaxSpd = 	cSPIN_GetMaxSpd;
	pDev->RegSetMinSpd = 	cSPIN_SetMinSpd;
	pDev->RegGetMinSpd = 	cSPIN_GetMinSpd;
	pDev->RegSetLowSpdOpt = 	cSPIN_SetLowSpdOpt;
	pDev->RegGetLowSpdOpt = 	cSPIN_GetLowSpdOpt;
	pDev->RegSetFSSpd = 	cSPIN_SetFSSpd;
	pDev->RegGetFSSpd = 	cSPIN_GetFSSpd;
	pDev->RegSetBoostMode = 	cSPIN_SetBoostMode;
	pDev->RegGetBoostMode = 	cSPIN_GetBoostMode;
	pDev->RegSetKValHold = 	cSPIN_SetKValHold;
	pDev->RegGetKValHold = 	cSPIN_GetKValHold;
	pDev->RegSetKValRun = 	cSPIN_SetKValRun;
	pDev->RegGetKValRun = 	cSPIN_GetKValRun;
	pDev->RegSetKValAcc = 	cSPIN_SetKValAcc;
	pDev->RegGetKValAcc = 	cSPIN_GetKValAcc;
	pDev->RegSetKValDec = 	cSPIN_SetKValDec;
	pDev->RegGetKValDec = 	cSPIN_GetKValDec;
	pDev->RegSetIntSpd = 	cSPIN_SetIntSpd;
	pDev->RegGetIntSpd = 	cSPIN_GetIntSpd;
	pDev->RegSetStartSlp = 	cSPIN_SetStartSlp;
	pDev->RegGetStartSlp = 	cSPIN_GetStartSlp;
	pDev->RegSetFnSlpAcc = 	cSPIN_SetFnSlpAcc;
	pDev->RegGetFnSlpAcc = 	cSPIN_GetFnSlpAcc;
	pDev->RegSetFnSlpDec = 	cSPIN_SetFnSlpDec;
	pDev->RegGetFnSlpDec = 	cSPIN_GetFnSlpDec;
	pDev->RegSetKTherm = 	cSPIN_SetKTherm;
	pDev->RegGetKTherm = 	cSPIN_GetKTherm;
	pDev->RegGetAdcOut = 	cSPIN_GetAdcOut;
	pDev->RegSetOCDTh = 	cSPIN_SetOCDTh;
	pDev->RegGetOCDTh = 	cSPIN_GetOCDTh;
	pDev->RegSetStallTh = 	cSPIN_SetStallTh;
	pDev->RegGetStallTh = 	cSPIN_GetStallTh;
	pDev->RegSetStepSel = 	cSPIN_SetStepSel;
	pDev->RegGetStepSel = 	cSPIN_GetStepSel;
	pDev->RegSetSyncSel = 	cSPIN_SetSyncSel;
	pDev->RegGetSyncSel = 	cSPIN_GetSyncSel;
	pDev->RegSetSyncEn = 	cSPIN_SetSyncEn;
	pDev->RegGetSyncEn = 	cSPIN_GetSyncEn;
	pDev->RegSetGateTcc = 	cSPIN_SetGateTcc;
	pDev->RegGetGateTcc = 	cSPIN_GetGateTcc;
	pDev->RegSetGateCurrent = 	cSPIN_SetGateCurrent;
	pDev->RegGetGateCurrent = 	cSPIN_GetGateCurrent;
	pDev->RegSetGateTBoost = 	cSPIN_SetGateTBoost;
	pDev->RegGetGateTBoost = 	cSPIN_GetGateTBoost;
	pDev->RegSetGateTDeadTime = 	cSPIN_SetGateTDeadTime;
	pDev->RegGetGateTDeadTime = 	cSPIN_GetGateTDeadTime;
	pDev->RegSetGateTBlank = 	cSPIN_SetGateTBlank;
	pDev->RegGetGateTBlank = 	cSPIN_GetGateTBlank;
	pDev->RegSetSwMode = cSPIN_SetSwMode;
	pDev->RegGetSwMode = cSPIN_GetSwMode;
	pDev->RegSetVsCompEn = cSPIN_SetVsCompEn;
	pDev->RegGetVsCompEn = cSPIN_GetVsCompEn;
	pDev->RegSetOCSDEn = cSPIN_SetOCSDEn; 
	pDev->RegGetOCSDEn = cSPIN_GetOCSDEn;
	pDev->RegSetUVLOVal =cSPIN_SetUVLOVal; 
	pDev->RegGetUVLOVal =cSPIN_GetUVLOVal; 
	pDev->RegSetVccVal = cSPIN_SetVccVal;
	pDev->RegGetVccVal = cSPIN_GetVccVal; 
	pDev->RegSetPwmFrq = cSPIN_SetPwmFrq;
	pDev->RegGetPwmFrq = cSPIN_GetPwmFrq;
	//pDev->RegGetStatus = cSPIN_GetStatus;

	pDev->Set_All		=	cSPIN_Registers_SetAll;
	pDev->Get_All		= cSPIN_Registers_GetAll;
	pDev->Get_Few		= cSPIN_Registers_GetFew;
	pDev->Set_Param = cSPIN_Set_Param;
	pDev->Get_Param		=	cSPIN_Get_Param;
	
	pDev->Run					=	cSPIN_Run;
	pDev->Step_Clock	=	cSPIN_Step_Clock;
	pDev->Move				=	cSPIN_Move;
	pDev->Go_To				=	cSPIN_Go_To;
	pDev->Go_To_Dir		=	cSPIN_Go_To_Dir;
	pDev->Go_Until		=	cSPIN_Go_Until;
	pDev->Release_SW	=	cSPIN_Release_SW;
	pDev->Go_Home			=	cSPIN_Go_Home;
	pDev->Go_Mark			=	cSPIN_Go_Mark;
	pDev->Reset_Pos		=	cSPIN_Reset_Pos;
	pDev->Soft_Stop		=	cSPIN_Soft_Stop;
	pDev->Hard_Stop		=	cSPIN_Hard_Stop;
	pDev->Soft_HiZ		=	cSPIN_Soft_HiZ;
	pDev->Hard_HiZ		=	cSPIN_Hard_HiZ;
	pDev->Get_Status	=	cSPIN_Get_Status;
	pDev->ResetDev		= cSPIN_Reset_Device;
	
	pDev->tickExe = cSPIN_tickExe;
	
	pDev->disableRptAbsPos = cSPIN_DisRptAbsPos;
	pDev->enableRptAbsPos = cSPIN_EnRptAbsPos;
	pDev->disableRptStatus = cSPIN_DisRptStatus;
	pDev->enableRptStatus = cSPIN_EnRptStatus;
	pDev->disableRptAdcOut = cSPIN_DisRptAdcOut;
	pDev->enableRptAdcOut = cSPIN_EnRptAdcOut;
	
	pDev->Busy_HW			=	cSPIN_Busy_HW;
	pDev->Busy_SW			=	cSPIN_Busy_SW;
	pDev->Flag				=	cSPIN_Flag;
//	return;
	/* Resets and puts cSPIN into standby mode */
	cSPIN_Reset_Device(pRsrc);	//software reset	
//	cSPIN_Reset_And_Standby(pRsrc);		//hardware reset	

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
#if defined(motor)
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
	pRsrc->reg.CONFIG 	= (u16)cSPIN_CONF_PARAM_CLOCK_SETTING | \
                        (u16)cSPIN_CONF_PARAM_SW_MODE	  | \
                        (u16)cSPIN_CONF_PARAM_VS_COMP		| \
                        (u16)cSPIN_CONF_PARAM_OC_SD      | \
                        (u16)cSPIN_CONF_PARAM_UVLOVAL    | \
                        (u16)cSPIN_CONF_PARAM_VCCVAL	   	| \
                        (u16)cSPIN_CONF_PARAM_PWM_DIV    | \
                        (u16)cSPIN_CONF_PARAM_PWM_MUL;
#endif /* defined(motor) */
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
	pRsrc->reg.T_FAST 	= (u8)cSPIN_CONF_PARAM_TOFF_FAST | (u8)cSPIN_CONF_PARAM_FAST_STEP;
				/* Minimum ON time value used by the current control system, range 0.5us to 64us */
	pRsrc->reg.TON_MIN 	= Tmin_Time_to_Par(cSPIN_CONF_PARAM_TON_MIN);
				/* Minimum OFF time value used by the current control system, range 0.5us to 64us */
	pRsrc->reg.TOFF_MIN	= Tmin_Time_to_Par(cSPIN_CONF_PARAM_TOFF_MIN);
				/* Set Config register according to config parameters */
				/* clock setting, switch hard stop interrupt mode, */
				/*  supply voltage compensation, overcurrent shutdown */
        /* UVLO threshold, VCC reg output voltage , target switching period, predictive current control */
	pRsrc->reg.CONFIG 	= (u16)cSPIN_CONF_PARAM_CLOCK_SETTING | \
                                          (u16)cSPIN_CONF_PARAM_SW_MODE	   | \
                                          (u16)cSPIN_CONF_PARAM_TQ_REG     | \
                                          (u16)cSPIN_CONF_PARAM_OC_SD      | \
                                          (u16)cSPIN_CONF_PARAM_UVLOVAL    | \
                                          (u16)cSPIN_CONF_PARAM_VCCVAL	   | \
                                          (u16)cSPIN_CONF_PARAM_TSW        | \
                                          (u16)cSPIN_CONF_PARAM_PRED;        
#endif /* defined(L6482) */
	/* Overcurrent threshold settings to cSPIN_CONF_PARAM_OCD_TH, range 31.25 to 1000mV */
	pRsrc->reg.OCD_TH 	= cSPIN_CONF_PARAM_OCD_TH;        
  /* Alarm settings to cSPIN_CONF_PARAM_ALARM_EN */
	pRsrc->reg.ALARM_EN 	= cSPIN_CONF_PARAM_ALARM_EN;
  /* Step mode and sycn mode settings via cSPIN_CONF_PARAM_SYNC_MODE and cSPIN_CONF_PARAM_STEP_MODE */
	pRsrc->reg.STEP_MODE 	= (u8)cSPIN_CONF_PARAM_SYNC_MODE | \
                          (u8)cSPIN_CONF_PARAM_STEP_MODE;
  /* Sink/source current, duration of constant current phases, duration of overboost phase settings */
  pRsrc->reg.GATECFG1   = (u16)cSPIN_CONF_PARAM_IGATE | \
													(u16)cSPIN_CONF_PARAM_TCC   | \
													(u16)cSPIN_CONF_PARAM_TBOOST;
	/* Blank time, Dead time stiings */
	 pRsrc->reg.GATECFG2  = (u16)cSPIN_CONF_PARAM_TBLANK | \
													(u16)cSPIN_CONF_PARAM_TDT;
  /* Program all cSPIN registers */
	cSPIN_Registers_SetAll(pRsrc);
             
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
void cSPIN_Set_Param(cSPIN_Rsrc_T* pRsrc, cSPIN_Registers_TypeDef param, u32 value)
{
	/* Send SetParam operation code to cSPIN */
	cSPIN_Write_Byte(pRsrc, (u8)cSPIN_SET_PARAM | (u8)param);
	switch (param)
	{
		/* These following REG need 3 bytes */
		case cSPIN_ABS_POS: ;
		case cSPIN_MARK: ;
			/* Send parameter - byte 2 to cSPIN */
			cSPIN_Write_Byte(pRsrc, (u8)(value >> 16));	
		/* These following REG need 2 bytes */
		case cSPIN_EL_POS: ;
		case cSPIN_ACC: ;
		case cSPIN_DEC: ;
		case cSPIN_MAX_SPEED: ;
		case cSPIN_MIN_SPEED: ;
		case cSPIN_FS_SPD: ;
#if defined(motor)
		case cSPIN_INT_SPD: ;
#endif /* defined(motor) */
		case cSPIN_CONFIG: ;
		case cSPIN_STATUS:
			/* Send parameter - byte 1 to cSPIN */
		   	cSPIN_Write_Byte(pRsrc, (u8)(value >> 8));
		/* These following REG need 1 bytes */
		default:
			/* Send parameter - byte 0 to cSPIN */
		   	cSPIN_Write_Byte(pRsrc, (u8)(value));
	}
}

/**
  * @brief  Issues cSPIN Get Param command.
  * @param  param cSPIN register address
  * @retval Register value - 1 to 3 bytes (depends on register)
  */
u32 cSPIN_Get_Param(cSPIN_Rsrc_T* pRsrc, cSPIN_Registers_TypeDef param)
{
	u32 rx = 0;

	/* Send GetParam operation code to cSPIN */
	cSPIN_Write_Byte(pRsrc, (u8)cSPIN_GET_PARAM | (u8)param);
	/* read back data */
	switch (param)
	{
		case cSPIN_ABS_POS: ;
		case cSPIN_MARK: ;
		case cSPIN_SPEED:
			rx = cSPIN_Write_Byte(pRsrc, 0x00);		rx <<= 8;
		case cSPIN_EL_POS: ;
		case cSPIN_ACC: ;
		case cSPIN_DEC: ;
		case cSPIN_MAX_SPEED: ;
		case cSPIN_MIN_SPEED: ;
		case cSPIN_FS_SPD: ;
#if defined(motor)
		case cSPIN_INT_SPD: ;
#endif /* defined(motor) */
		case cSPIN_CONFIG: ;
		case cSPIN_STATUS:
			rx |= cSPIN_Write_Byte(pRsrc, 0x00);		rx <<= 8;
		default:
			rx |= cSPIN_Write_Byte(pRsrc, 0x00);		
	}
	return rx;
}

/*****************************************
  * @brief  Issues cSPIN registers ops
  * @param  value, 22bit
  * @param  
  * @retval None
*****************************************/
bool isCmdErr(cSPIN_Rsrc_T* pRsrc){
	cSPIN_Delay(48);	//about 1us @ 48MHz	
	if(cSPIN_Get_Status(pRsrc) & (1<<7) )
		return CMD_FAIL;
	cSPIN_Delay(48);
	if(cSPIN_Get_Param(pRsrc, cSPIN_ADC_OUT) == 0)	return CMD_FAIL;
	return CMD_PASS;		
}

/*****************************************
  * @brief  Issues cSPIN registers ops
  * @param  true value, 22bit
  * @param  
  * @retval None
*****************************************/
bool cSPIN_SetAbsPos			(cSPIN_Rsrc_T* pRsrc, u32 param){
	cSPIN_Set_Param(pRsrc, cSPIN_ABS_POS, param);
	return isCmdErr(pRsrc);
}
bool cSPIN_GetAbsPos			(cSPIN_Rsrc_T* pRsrc, u32 *param){
	*param = cSPIN_Get_Param(pRsrc, cSPIN_ABS_POS);
	return isCmdErr(pRsrc);
}

bool cSPIN_SetElPos			(cSPIN_Rsrc_T* pRsrc, u16 param){
	cSPIN_Set_Param(pRsrc, cSPIN_EL_POS, param);
	return isCmdErr(pRsrc);
}
bool cSPIN_GetElPos			(cSPIN_Rsrc_T* pRsrc, u16 *param){
	*param = (u16)cSPIN_Get_Param(pRsrc, cSPIN_EL_POS);
	return isCmdErr(pRsrc);
}


bool cSPIN_SetMarkPos		(cSPIN_Rsrc_T* pRsrc, u32 param){
	cSPIN_Set_Param(pRsrc, cSPIN_MARK, param);
	return isCmdErr(pRsrc);
}
bool cSPIN_GetMarkPos		(cSPIN_Rsrc_T* pRsrc, u32 *param){
	*param = cSPIN_Get_Param(pRsrc, cSPIN_MARK);
	return isCmdErr(pRsrc);
}

bool cSPIN_GetSpeed			(cSPIN_Rsrc_T* pRsrc, u32 *param){
	*param = cSPIN_Get_Param(pRsrc, cSPIN_SPEED);
	return isCmdErr(pRsrc);
}

bool cSPIN_SetAcc			(cSPIN_Rsrc_T* pRsrc, u16 param){
	cSPIN_Set_Param(pRsrc, cSPIN_ACC, param);
	return isCmdErr(pRsrc);
}
bool cSPIN_GetAcc			(cSPIN_Rsrc_T* pRsrc, u16 *param){
	*param = (u16)cSPIN_Get_Param(pRsrc, cSPIN_ACC);
	return isCmdErr(pRsrc);
}

bool cSPIN_SetDec			(cSPIN_Rsrc_T* pRsrc, u16 param){
	cSPIN_Set_Param(pRsrc, cSPIN_DEC, param);
	return isCmdErr(pRsrc);
}
bool cSPIN_GetDec			(cSPIN_Rsrc_T* pRsrc, u16 *param){
	*param = (u16)cSPIN_Get_Param(pRsrc, cSPIN_DEC);
	return isCmdErr(pRsrc);
}

bool cSPIN_SetMaxSpd			(cSPIN_Rsrc_T* pRsrc, u16 param){
	cSPIN_Set_Param(pRsrc, cSPIN_MAX_SPEED, param);
	return isCmdErr(pRsrc);
}
bool cSPIN_GetMaxSpd			(cSPIN_Rsrc_T* pRsrc, u16 *param){
	*param = (u16)cSPIN_Get_Param(pRsrc, cSPIN_MAX_SPEED);
	return isCmdErr(pRsrc);
}

bool cSPIN_SetMinSpd			(cSPIN_Rsrc_T* pRsrc, u16 param){
	u16 reg = 0;
	reg = (u16)cSPIN_Get_Param(pRsrc, cSPIN_MIN_SPEED);
	if(isCmdErr(pRsrc)==CMD_FAIL)	return CMD_FAIL;
	reg &= (1<<12);	reg |= (param&0x0fff);
	cSPIN_Set_Param(pRsrc, cSPIN_MIN_SPEED, reg);
	return isCmdErr(pRsrc);
}
bool cSPIN_GetMinSpd			(cSPIN_Rsrc_T* pRsrc, u16 *param){
	*param = (u16)cSPIN_Get_Param(pRsrc, cSPIN_MIN_SPEED)&0x0fff;
	return isCmdErr(pRsrc);
}

bool cSPIN_SetLowSpdOpt		(cSPIN_Rsrc_T* pRsrc, bool param){
	u16 reg = 0;
	reg = (u16)cSPIN_Get_Param(pRsrc, cSPIN_MIN_SPEED)&0x0fff;
	if(isCmdErr(pRsrc)==CMD_FAIL)	return CMD_FAIL;
	if(param == FALSE)
		cSPIN_Set_Param(pRsrc, cSPIN_MIN_SPEED, reg);
	else
		cSPIN_Set_Param(pRsrc, cSPIN_MIN_SPEED, reg|(1<<12));
	return isCmdErr(pRsrc);
}
bool cSPIN_GetLowSpdOpt			(cSPIN_Rsrc_T* pRsrc, bool *param){
	if(cSPIN_Get_Param(pRsrc, cSPIN_MIN_SPEED)&(1<<12))
		*param = TRUE;
	else
		*param = FALSE;
	return isCmdErr(pRsrc);
}

bool cSPIN_SetFSSpd			(cSPIN_Rsrc_T* pRsrc, u16 param){
	u16 reg = 0;
	reg = (u16)cSPIN_Get_Param(pRsrc, cSPIN_FS_SPD);
	if(isCmdErr(pRsrc)==CMD_FAIL)	return CMD_FAIL;
	reg &= (1<<10);	reg |= (param&(0xffff>>(16-10)));
	cSPIN_Set_Param(pRsrc, cSPIN_FS_SPD, reg);
	return isCmdErr(pRsrc);
}
bool cSPIN_GetFSSpd			(cSPIN_Rsrc_T* pRsrc, u16 *param){
	*param = (u16)cSPIN_Get_Param(pRsrc, cSPIN_FS_SPD)&(0xffff>>(16-10));
	return isCmdErr(pRsrc);
}

bool cSPIN_SetBoostMode		(cSPIN_Rsrc_T* pRsrc, bool param){
	u16 reg = 0;
	reg = (u16)cSPIN_Get_Param(pRsrc, cSPIN_FS_SPD)&(0xffff>>(16-10));
	if(isCmdErr(pRsrc)==CMD_FAIL)	return CMD_FAIL;
	if(param == FALSE)
		cSPIN_Set_Param(pRsrc, cSPIN_FS_SPD, reg);
	else
		cSPIN_Set_Param(pRsrc, cSPIN_FS_SPD, reg|(1<<10));
	return isCmdErr(pRsrc);
}
bool cSPIN_GetBoostMode			(cSPIN_Rsrc_T* pRsrc, bool *param){
	if(cSPIN_Get_Param(pRsrc, cSPIN_FS_SPD)&(1<<10))
		*param = TRUE;
	else
		*param = FALSE;
	return isCmdErr(pRsrc);
}

bool cSPIN_SetKValHold (cSPIN_Rsrc_T* pRsrc, u8 param){
	cSPIN_Set_Param(pRsrc, cSPIN_KVAL_HOLD, param);
	return isCmdErr(pRsrc);
}
bool cSPIN_GetKValHold (cSPIN_Rsrc_T* pRsrc, u8 *param){
	*param = (u8)cSPIN_Get_Param(pRsrc, cSPIN_KVAL_HOLD);
	return isCmdErr(pRsrc);
}

bool cSPIN_SetKValRun (cSPIN_Rsrc_T* pRsrc, u8 param){
	cSPIN_Set_Param(pRsrc, cSPIN_KVAL_RUN, param);
	return isCmdErr(pRsrc);
}
bool cSPIN_GetKValRun (cSPIN_Rsrc_T* pRsrc, u8 *param){
	*param = (u8)cSPIN_Get_Param(pRsrc, cSPIN_KVAL_RUN);
	return isCmdErr(pRsrc);
}

bool cSPIN_SetKValAcc (cSPIN_Rsrc_T* pRsrc, u8 param){
	cSPIN_Set_Param(pRsrc, cSPIN_KVAL_ACC, param);
	return isCmdErr(pRsrc);
}
bool cSPIN_GetKValAcc (cSPIN_Rsrc_T* pRsrc, u8 *param){
	*param = (u8)cSPIN_Get_Param(pRsrc, cSPIN_KVAL_ACC);
	return isCmdErr(pRsrc);
}

bool cSPIN_SetKValDec (cSPIN_Rsrc_T* pRsrc, u8 param){
	cSPIN_Set_Param(pRsrc, cSPIN_KVAL_DEC, param);
	return isCmdErr(pRsrc);
}
bool cSPIN_GetKValDec (cSPIN_Rsrc_T* pRsrc, u8 *param){
	*param = (u8)cSPIN_Get_Param(pRsrc, cSPIN_KVAL_DEC);
	return isCmdErr(pRsrc);
}

bool cSPIN_SetIntSpd (cSPIN_Rsrc_T* pRsrc, u16 param){
	cSPIN_Set_Param(pRsrc, cSPIN_INT_SPD, param);
	return isCmdErr(pRsrc);
}
bool cSPIN_GetIntSpd (cSPIN_Rsrc_T* pRsrc, u16 *param){
	*param = (u16)cSPIN_Get_Param(pRsrc, cSPIN_INT_SPD);
	return isCmdErr(pRsrc);
}

bool cSPIN_SetStartSlp (cSPIN_Rsrc_T* pRsrc, u8 param){
	cSPIN_Set_Param(pRsrc, cSPIN_ST_SLP, param);
	return isCmdErr(pRsrc);
}
bool cSPIN_GetStartSlp (cSPIN_Rsrc_T* pRsrc, u8 *param){
	*param = (u8)cSPIN_Get_Param(pRsrc, cSPIN_ST_SLP);
	return isCmdErr(pRsrc);
}

bool cSPIN_SetFnSlpAcc (cSPIN_Rsrc_T* pRsrc, u8 param){
	cSPIN_Set_Param(pRsrc, cSPIN_FN_SLP_ACC, param);
	return isCmdErr(pRsrc);
}
bool cSPIN_GetFnSlpAcc (cSPIN_Rsrc_T* pRsrc, u8 *param){
	*param = (u8)cSPIN_Get_Param(pRsrc, cSPIN_FN_SLP_ACC);
	return isCmdErr(pRsrc);
}

bool cSPIN_SetFnSlpDec (cSPIN_Rsrc_T* pRsrc, u8 param){
	cSPIN_Set_Param(pRsrc, cSPIN_FN_SLP_DEC, param);
	return isCmdErr(pRsrc);
}
bool cSPIN_GetFnSlpDec (cSPIN_Rsrc_T* pRsrc, u8 *param){
	*param = (u8)cSPIN_Get_Param(pRsrc, cSPIN_FN_SLP_DEC);
	return isCmdErr(pRsrc);
}

bool cSPIN_SetKTherm (cSPIN_Rsrc_T* pRsrc, u8 param){
	cSPIN_Set_Param(pRsrc, cSPIN_K_THERM, param);
	return isCmdErr(pRsrc);
}
bool cSPIN_GetKTherm (cSPIN_Rsrc_T* pRsrc, u8 *param){
	*param = (u8)cSPIN_Get_Param(pRsrc, cSPIN_K_THERM);
	return isCmdErr(pRsrc);
}

bool cSPIN_GetAdcOut (cSPIN_Rsrc_T* pRsrc, u8 *param){
	*param = (u8)cSPIN_Get_Param(pRsrc, cSPIN_ADC_OUT);
	return isCmdErr(pRsrc);
}

bool cSPIN_SetOCDTh (cSPIN_Rsrc_T* pRsrc, u8 param){
	cSPIN_Set_Param(pRsrc, cSPIN_OCD_TH, param);
	return isCmdErr(pRsrc);
}
bool cSPIN_GetOCDTh (cSPIN_Rsrc_T* pRsrc, u8 *param){
	*param = (u8)cSPIN_Get_Param(pRsrc, cSPIN_OCD_TH);
	return isCmdErr(pRsrc);
}

bool cSPIN_SetStallTh (cSPIN_Rsrc_T* pRsrc, u8 param){
	cSPIN_Set_Param(pRsrc, cSPIN_STALL_TH, param);
	return isCmdErr(pRsrc);
}
bool cSPIN_GetStallTh (cSPIN_Rsrc_T* pRsrc, u8 *param){
	*param = (u8)cSPIN_Get_Param(pRsrc, cSPIN_STALL_TH);
	return isCmdErr(pRsrc);
}

bool cSPIN_SetStepSel (cSPIN_Rsrc_T* pRsrc, u8 param){
	u8 reg = 0;
	reg = (u8)cSPIN_Get_Param(pRsrc, cSPIN_STEP_MODE);
	if(isCmdErr(pRsrc)==CMD_FAIL)	return CMD_FAIL;
	reg &= (0xff<<3);	reg |= param;
	cSPIN_Set_Param(pRsrc, cSPIN_STEP_MODE, reg);
	return isCmdErr(pRsrc);
}
bool cSPIN_GetStepSel (cSPIN_Rsrc_T* pRsrc, u8 *param){
	*param = (u8)cSPIN_Get_Param(pRsrc, cSPIN_STEP_MODE)&0x07;
	return isCmdErr(pRsrc);
}

bool cSPIN_SetSyncSel (cSPIN_Rsrc_T* pRsrc, u8 param){
	u8 reg = 0;
	reg = (u8)cSPIN_Get_Param(pRsrc, cSPIN_STEP_MODE);
	if(isCmdErr(pRsrc)==CMD_FAIL)	return CMD_FAIL;
	reg &= (0xff^(0x07<<4));	reg |= (param&0x07)<<4;
	cSPIN_Set_Param(pRsrc, cSPIN_STEP_MODE, reg);
	return isCmdErr(pRsrc);
}
bool cSPIN_GetSyncSel (cSPIN_Rsrc_T* pRsrc, u8 *param){
	*param = ((u8)cSPIN_Get_Param(pRsrc, cSPIN_STEP_MODE)>>4)&0x07;
	return isCmdErr(pRsrc);
}

bool cSPIN_SetSyncEn (cSPIN_Rsrc_T* pRsrc, u8 param){
	u8 reg = 0;
	reg = (u8)cSPIN_Get_Param(pRsrc, cSPIN_STEP_MODE);
	if(isCmdErr(pRsrc)==CMD_FAIL)	return CMD_FAIL;
	reg &= 0x7f;	reg |= (param&0x01)<<7;
	cSPIN_Set_Param(pRsrc, cSPIN_STEP_MODE, reg);
	return isCmdErr(pRsrc);
}
bool cSPIN_GetSyncEn (cSPIN_Rsrc_T* pRsrc, u8 *param){
	*param = ((u8)cSPIN_Get_Param(pRsrc, cSPIN_STEP_MODE)>>7)&0x01;
	return isCmdErr(pRsrc);
}

bool cSPIN_SetGateTcc (cSPIN_Rsrc_T* pRsrc, u8 param){
	u16 reg = 0;
	reg = (u16)cSPIN_Get_Param(pRsrc, cSPIN_GATECFG1);
	if(isCmdErr(pRsrc)==CMD_FAIL)	return CMD_FAIL;
	reg &= (0xffff<<5);	reg |= param;
	cSPIN_Set_Param(pRsrc, cSPIN_GATECFG1, reg);
	return isCmdErr(pRsrc);
}
bool cSPIN_GetGateTcc (cSPIN_Rsrc_T* pRsrc, u8 *param){
	*param = (u16)cSPIN_Get_Param(pRsrc, cSPIN_GATECFG1)&0x1f;
	return isCmdErr(pRsrc);
}

bool cSPIN_SetGateCurrent (cSPIN_Rsrc_T* pRsrc, u8 param){
	u16 reg = 0;
	reg = (u16)cSPIN_Get_Param(pRsrc, cSPIN_GATECFG1);
	if(isCmdErr(pRsrc)==CMD_FAIL)	return CMD_FAIL;
	reg &= 0xff1f;	reg |= (param&0x07)<<5;
	cSPIN_Set_Param(pRsrc, cSPIN_GATECFG1, reg);
	return isCmdErr(pRsrc);
}
bool cSPIN_GetGateCurrent (cSPIN_Rsrc_T* pRsrc, u8 *param){
	
	*param = (cSPIN_Get_Param(pRsrc, cSPIN_GATECFG1)>>5)&0x07;
	
	return isCmdErr(pRsrc);
}

bool cSPIN_SetGateTBoost (cSPIN_Rsrc_T* pRsrc, u8 param){
	u16 reg = 0;
	reg = (u16)cSPIN_Get_Param(pRsrc, cSPIN_GATECFG1);
	if(isCmdErr(pRsrc)==CMD_FAIL)	return CMD_FAIL;
	reg &= (0xffff^(0x07<<8));	reg |= (u16)(param&0x07)<<8;
	cSPIN_Set_Param(pRsrc, cSPIN_GATECFG1, reg);
	return isCmdErr(pRsrc);
}
bool cSPIN_GetGateTBoost (cSPIN_Rsrc_T* pRsrc, u8 *param){
	*param = (u8)(cSPIN_Get_Param(pRsrc, cSPIN_GATECFG1)>>8)&0x07;
	return isCmdErr(pRsrc);
}

bool cSPIN_SetGateTDeadTime (cSPIN_Rsrc_T* pRsrc, u8 param){
	u8 reg = 0;
	reg = (u8)cSPIN_Get_Param(pRsrc, cSPIN_GATECFG2);
	if(isCmdErr(pRsrc)==CMD_FAIL)	return CMD_FAIL;
	reg &= 0xff<<5;	reg |= (param&0x1f);
	cSPIN_Set_Param(pRsrc, cSPIN_GATECFG2, reg);
	return isCmdErr(pRsrc);
}
bool cSPIN_GetGateTDeadTime (cSPIN_Rsrc_T* pRsrc, u8 *param){
	*param = (u8)cSPIN_Get_Param(pRsrc, cSPIN_GATECFG2)&0x1f;
	return isCmdErr(pRsrc);
}

bool cSPIN_SetGateTBlank (cSPIN_Rsrc_T* pRsrc, u8 param){
	u8 reg = 0;
	reg = (u8)cSPIN_Get_Param(pRsrc, cSPIN_GATECFG2);
	if(isCmdErr(pRsrc)==CMD_FAIL)	return CMD_FAIL;
	reg &= 0xff>>3;	reg |= (param&0x07)<<5;
	cSPIN_Set_Param(pRsrc, cSPIN_GATECFG2, reg);
	return isCmdErr(pRsrc);
}
bool cSPIN_GetGateTBlank (cSPIN_Rsrc_T* pRsrc, u8 *param){
	*param = (u8)(cSPIN_Get_Param(pRsrc, cSPIN_GATECFG2)&0x07)>>5;
	return isCmdErr(pRsrc);
}

/************************
	*config
************************/
//osc is fixed @int16MHz,8MHz output
bool cSPIN_SetSwMode (cSPIN_Rsrc_T* pRsrc, bool param){
	u16 reg = 0;
	reg = (u16)cSPIN_Get_Param(pRsrc, cSPIN_CONFIG);
	if(isCmdErr(pRsrc)==CMD_FAIL)	return CMD_FAIL;
	reg &= (1<<4)^0xff;	reg |= (param&0x01)<<4;
	cSPIN_Set_Param(pRsrc, cSPIN_CONFIG, reg);
	return isCmdErr(pRsrc);
}
bool cSPIN_GetSwMode (cSPIN_Rsrc_T* pRsrc, bool *param){
	*param = (bool)((cSPIN_Get_Param(pRsrc, cSPIN_CONFIG)>>4)&0x01);
	return isCmdErr(pRsrc);
}

bool cSPIN_SetVsCompEn (cSPIN_Rsrc_T* pRsrc, bool param){
	u16 reg = 0;
	reg = (u16)cSPIN_Get_Param(pRsrc, cSPIN_CONFIG);
	if(isCmdErr(pRsrc)==CMD_FAIL)	return CMD_FAIL;
	reg &= (1<<5)^0xff;	reg |= (param&0x01)<<5;
	cSPIN_Set_Param(pRsrc, cSPIN_CONFIG, reg);
	return isCmdErr(pRsrc);
}
bool cSPIN_GetVsCompEn (cSPIN_Rsrc_T* pRsrc, bool *param){
	*param = (bool)((cSPIN_Get_Param(pRsrc, cSPIN_CONFIG)>>5)&0x01);
	return isCmdErr(pRsrc);
}

bool cSPIN_SetOCSDEn (cSPIN_Rsrc_T* pRsrc, bool param){
	u16 reg = 0;
	reg = (u16)cSPIN_Get_Param(pRsrc, cSPIN_CONFIG);
	if(isCmdErr(pRsrc)==CMD_FAIL)	return CMD_FAIL;
	reg &= (1<<7)^0xff;	reg |= (param&0x01)<<7;
	cSPIN_Set_Param(pRsrc, cSPIN_CONFIG, reg);
	return isCmdErr(pRsrc);
}
bool cSPIN_GetOCSDEn (cSPIN_Rsrc_T* pRsrc, bool *param){
	*param = (bool)((cSPIN_Get_Param(pRsrc, cSPIN_CONFIG)>>7)&0x01);
	return isCmdErr(pRsrc);
}

bool cSPIN_SetUVLOVal (cSPIN_Rsrc_T* pRsrc, bool param){
	u16 reg = 0;
	reg = (u16)cSPIN_Get_Param(pRsrc, cSPIN_CONFIG);
	if(isCmdErr(pRsrc)==CMD_FAIL)	return CMD_FAIL;
	reg &= (1<<8)^0xff;	reg |= (param&0x01)<<8;
	cSPIN_Set_Param(pRsrc, cSPIN_CONFIG, reg);
	return isCmdErr(pRsrc);
}
bool cSPIN_GetUVLOVal (cSPIN_Rsrc_T* pRsrc, bool *param){
	*param = (bool)((cSPIN_Get_Param(pRsrc, cSPIN_CONFIG)>>8)&0x01);
	return isCmdErr(pRsrc);
}

bool cSPIN_SetVccVal (cSPIN_Rsrc_T* pRsrc, bool param){
	u16 reg = 0;
	reg = (u16)cSPIN_Get_Param(pRsrc, cSPIN_CONFIG);
	if(isCmdErr(pRsrc)==CMD_FAIL)	return CMD_FAIL;
	reg &= (1<<9)^0xff;	reg |= (param&0x01)<<9;
	cSPIN_Set_Param(pRsrc, cSPIN_CONFIG, reg);
	return isCmdErr(pRsrc);
}
bool cSPIN_GetVccVal (cSPIN_Rsrc_T* pRsrc, bool *param){
	*param = (bool)((cSPIN_Get_Param(pRsrc, cSPIN_CONFIG)>>9)&0x01);
	return isCmdErr(pRsrc);
}

bool cSPIN_SetPwmFrq (cSPIN_Rsrc_T* pRsrc, u8 param){
	u16 reg = 0;
	reg = (u16)cSPIN_Get_Param(pRsrc, cSPIN_CONFIG);
	if(isCmdErr(pRsrc)==CMD_FAIL)	return CMD_FAIL;
	reg &= (0xffff>>6);	reg |= (u16)(param&0x3f)<<10;
	cSPIN_Set_Param(pRsrc, cSPIN_CONFIG, reg);
	return isCmdErr(pRsrc);
}

bool cSPIN_GetPwmFrq (cSPIN_Rsrc_T* pRsrc, u8 *param){
	*param = (u8)((cSPIN_Get_Param(pRsrc, cSPIN_CONFIG)>>10)&0x3f);
	return isCmdErr(pRsrc);
}

//bool cSPIN_SetPwmDec (cSPIN_Rsrc_T* pRsrc, u8 param){
//	u16 reg = 0;
//	reg = (u16)cSPIN_Get_Param(pRsrc, cSPIN_CONFIG);
//	if(isCmdErr(pRsrc)==CMD_FAIL)	return CMD_FAIL;
//	reg &= (0xffff^(0x07<<10));	reg |= (u16)(param&0x07)<<10;
//	cSPIN_Set_Param(pRsrc, cSPIN_CONFIG, reg);
//	return isCmdErr(pRsrc);
//}
//bool cSPIN_GetPwmDec (cSPIN_Rsrc_T* pRsrc, u8 *param){
//	*param = (u8)((cSPIN_Get_Param(pRsrc, cSPIN_CONFIG)>>10)&0x07);
//	return isCmdErr(pRsrc);
//}

//bool cSPIN_SetPwmInt (cSPIN_Rsrc_T* pRsrc, u8 param){
//	u16 reg = 0;
//	reg = (u16)cSPIN_Get_Param(pRsrc, cSPIN_CONFIG);
//	if(isCmdErr(pRsrc)==CMD_FAIL)	return CMD_FAIL;
//	reg &= (0xffff^(0x07<<13));	reg |= (u16)(param&0x07)<<13;
//	cSPIN_Set_Param(pRsrc, cSPIN_CONFIG, reg);
//	return isCmdErr(pRsrc);
//}
//bool cSPIN_GetPwmInt (cSPIN_Rsrc_T* pRsrc, u8 *param){
//	*param = (u8)((cSPIN_Get_Param(pRsrc, cSPIN_CONFIG)>>13)&0x07);
//	return isCmdErr(pRsrc);
//}

//bool cSPIN_GetStatus (cSPIN_Rsrc_T* pRsrc, u16 *param){
//	*param = (u16)(cSPIN_Get_Param(pRsrc, cSPIN_STATUS));
//	return isCmdErr(pRsrc);
//}


/**
  * @brief  Issues cSPIN Run command.
  * @param  direction Movement direction (FWD, REV)
  * @param  speed over 3 bytes
  * @retval None
  */
void cSPIN_Run(cSPIN_Rsrc_T* pRsrc, cSPIN_Direction_T direction, u32 speed)
{
	/* Send RUN operation code to cSPIN */
	cSPIN_Write_Byte(pRsrc, (u8)cSPIN_RUN | (u8)direction);
	/* Send speed - byte 2 data cSPIN */
	cSPIN_Write_Byte(pRsrc, (u8)(speed >> 16));
	/* Send speed - byte 1 data cSPIN */
	cSPIN_Write_Byte(pRsrc, (u8)(speed >> 8));
	/* Send speed - byte 0 data cSPIN */
	cSPIN_Write_Byte(pRsrc, (u8)(speed));
}

/**
  * @brief  Issues cSPIN Step Clock command.
  * @param  direction Movement direction (FWD, REV)
  * @retval None
  */
void cSPIN_Step_Clock(cSPIN_Rsrc_T* pRsrc, cSPIN_Direction_T direction)
{
	/* Send StepClock operation code to cSPIN */
	cSPIN_Write_Byte(pRsrc, (u8)cSPIN_STEP_CLOCK | (u8)direction);
}

/**
  * @brief  Issues cSPIN Move command.
  * @param  direction Movement direction
  * @param  n_step number of steps
  * @retval None
  */
void cSPIN_Move(cSPIN_Rsrc_T* pRsrc, cSPIN_Direction_T direction, u32 n_step)
{
	/* Send Move operation code to cSPIN */
	cSPIN_Write_Byte(pRsrc, (u8)cSPIN_MOVE | (u8)direction);
	/* Send n_step - byte 2 data cSPIN */
	cSPIN_Write_Byte(pRsrc, (u8)(n_step >> 16));
	/* Send n_step - byte 1 data cSPIN */
	cSPIN_Write_Byte(pRsrc, (u8)(n_step >> 8));
	/* Send n_step - byte 0 data cSPIN */
	cSPIN_Write_Byte(pRsrc, (u8)(n_step));
}

/**
  * @brief  Issues cSPIN Go To command.
  * @param  abs_pos absolute position where requested to move
  * @retval None
  */
void cSPIN_Go_To(cSPIN_Rsrc_T* pRsrc, u32 abs_pos)
{
	/* Send GoTo operation code to cSPIN */
	cSPIN_Write_Byte(pRsrc, cSPIN_GO_TO);
	/* Send absolute position parameter - byte 2 data to cSPIN */
	cSPIN_Write_Byte(pRsrc, (u8)(abs_pos >> 16));
	/* Send absolute position parameter - byte 1 data to cSPIN */
	cSPIN_Write_Byte(pRsrc, (u8)(abs_pos >> 8));
	/* Send absolute position parameter - byte 0 data to cSPIN */
	cSPIN_Write_Byte(pRsrc, (u8)(abs_pos));
}

/**
  * @brief  Issues cSPIN Go To Dir command.
  * @param  direction movement direction
  * @param  abs_pos absolute position where requested to move
  * @retval None
  */
void cSPIN_Go_To_Dir(cSPIN_Rsrc_T* pRsrc, cSPIN_Direction_T direction, u32 abs_pos)
{
	/* Send GoTo_DIR operation code to cSPIN */
	cSPIN_Write_Byte(pRsrc, (u8)cSPIN_GO_TO_DIR | (u8)direction);
	/* Send absolute position parameter - byte 2 data to cSPIN */
	cSPIN_Write_Byte(pRsrc, (u8)(abs_pos >> 16));
	/* Send absolute position parameter - byte 1 data to cSPIN */
	cSPIN_Write_Byte(pRsrc, (u8)(abs_pos >> 8));
	/* Send absolute position parameter - byte 0 data to cSPIN */
	cSPIN_Write_Byte(pRsrc, (u8)(abs_pos));
}

/**
  * @brief  Issues cSPIN Go Until command.
  * @param  action
  * @param  direction movement direction
  * @param  speed
  * @retval None
  */
void cSPIN_Go_Until(cSPIN_Rsrc_T* pRsrc, cSPIN_Action_T action, cSPIN_Direction_T direction, u32 speed)
{
	/* Send GoUntil operation code to cSPIN */
	cSPIN_Write_Byte(pRsrc, (u8)cSPIN_GO_UNTIL | (u8)action | (u8)direction);
	/* Send speed parameter - byte 2 data to cSPIN */
	cSPIN_Write_Byte(pRsrc, (u8)(speed >> 16));
	/* Send speed parameter - byte 1 data to cSPIN */
	cSPIN_Write_Byte(pRsrc, (u8)(speed >> 8));
	/* Send speed parameter - byte 0 data to cSPIN */
	cSPIN_Write_Byte(pRsrc, (u8)(speed));
}

/**
  * @brief  Issues cSPIN Release SW command.
  * @param  action
  * @param  direction movement direction
  * @retval None
  */
void cSPIN_Release_SW(cSPIN_Rsrc_T* pRsrc, cSPIN_Action_T action, cSPIN_Direction_T direction)
{
	/* Send ReleaseSW operation code to cSPIN */
	cSPIN_Write_Byte(pRsrc, (u8)cSPIN_RELEASE_SW | (u8)action | (u8)direction);
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
u16 cSPIN_Get_Status(cSPIN_Rsrc_T* pRsrc)
{
	u16 temp = 0;
	u16 rx = 0;

	/* Send GetStatus operation code to cSPIN */
	cSPIN_Write_Byte(pRsrc, cSPIN_GET_STATUS);
	/* Send zero byte / receive MSByte from cSPIN */
	temp = cSPIN_Write_Byte(pRsrc, (u8)(0x00));
	temp = temp << 8;
	rx |= temp;
	/* Send zero byte / receive LSByte from cSPIN */
	temp = cSPIN_Write_Byte(pRsrc, (u8)(0x00));
	rx |= temp;
	return rx;
}

/**
  * @brief  Checks if the cSPIN is Busy by hardware - active Busy signal.
  * @param  None
  * @retval one if chip is busy, otherwise zero
  */
u8 cSPIN_Busy_HW(cSPIN_Rsrc_T* pRsrc)
{
	if(HAL_GPIO_ReadPin(pRsrc->BUSY.GPIOx, pRsrc->BUSY.GPIO_Pin) == GPIO_PIN_RESET) return 0x01;
	else return 0x00;
}

/**
  * @brief  Checks if the cSPIN is Busy by SPI - Busy flag bit in Status Register.
  * @param  None
  * @retval one if chip is busy, otherwise zero
  */
u8 cSPIN_Busy_SW(cSPIN_Rsrc_T* pRsrc)
{
	if(!(cSPIN_Get_Status(pRsrc) & cSPIN_STATUS_BUSY)) return 0x01;
	else return 0x00;
}

/**
  * @brief  Checks cSPIN Flag signal.
  * @param  None
  * @retval one if Flag signal is active, otherwise zero
  */
u8 cSPIN_Flag(cSPIN_Rsrc_T* pRsrc)
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
void cSPIN_Delay(__IO u32 nCount)
{
  for(; nCount!= 0;nCount--);
}

/**
  * @brief  Transmits/Receives one byte to/from cSPIN over SPI.
  * @param  byte Transmited byte
  * @retval Received byte
  */
u8 cSPIN_Write_Byte(cSPIN_Rsrc_T* pRsrc, u8 byte)
{
	u8 rtn=0;
	while (HAL_SPI_GetState(pRsrc->SPI_HANDLE) != HAL_SPI_STATE_READY);
	/* nSS signal activation - low */
	HAL_GPIO_WritePin(pRsrc->CS.GPIOx, pRsrc->CS.GPIO_Pin, GPIO_PIN_RESET);
	/* SPI byte send */
	HAL_SPI_TransmitReceive(pRsrc->SPI_HANDLE, &byte, &rtn, 1, HAL_MAX_DELAY);
	/* nSS signal deactivation - high */
	HAL_GPIO_WritePin(pRsrc->CS.GPIOx, pRsrc->CS.GPIO_Pin,GPIO_PIN_SET);
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
#if defined(motor)
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
#endif /* defined(motor) */
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
void cSPIN_Registers_SetAll(cSPIN_Rsrc_T* pRsrc)
{
	cSPIN_Set_Param(pRsrc, cSPIN_ABS_POS, pRsrc->reg.ABS_POS);
	cSPIN_Set_Param(pRsrc, cSPIN_EL_POS, pRsrc->reg.EL_POS);
	cSPIN_Set_Param(pRsrc, cSPIN_MARK, pRsrc->reg.MARK);
	cSPIN_Set_Param(pRsrc, cSPIN_ACC, pRsrc->reg.ACC);
	cSPIN_Set_Param(pRsrc, cSPIN_DEC, pRsrc->reg.DEC);
	cSPIN_Set_Param(pRsrc, cSPIN_MAX_SPEED, pRsrc->reg.MAX_SPEED);
	cSPIN_Set_Param(pRsrc, cSPIN_MIN_SPEED, pRsrc->reg.MIN_SPEED);
	cSPIN_Set_Param(pRsrc, cSPIN_FS_SPD, pRsrc->reg.FS_SPD);
#if defined(motor)
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
#endif /* defined(motor) */
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

/**
  * @brief  Configures cSPIN internal registers with values in the config structure.
  * @param  cSPIN_RegsStruct Configuration structure address (pointer to configuration structure)
  * @retval None
  */
void cSPIN_Registers_GetFew(cSPIN_Rsrc_T* pRsrc){
	//¶ÁÈ¡
//	pRsrc->reg.ABS_POS = cSPIN_Get_Param(pRsrc, cSPIN_ABS_POS);
//	pRsrc->reg.SPEED = cSPIN_Get_Param(pRsrc, cSPIN_SPEED);
//	pRsrc->reg.ADC_OUT = cSPIN_Get_Param(pRsrc, cSPIN_ADC_OUT);
//	pRsrc->reg.STATUS = cSPIN_Get_Param(pRsrc, cSPIN_STATUS);	
}

void cSPIN_Registers_GetAll(cSPIN_Rsrc_T* pRsrc)
{	//
	cSPIN_Registers_GetFew(pRsrc);
	pRsrc->reg.EL_POS = cSPIN_Get_Param(pRsrc, cSPIN_EL_POS);
	pRsrc->reg.MARK = cSPIN_Get_Param(pRsrc, cSPIN_MARK);
	pRsrc->reg.ACC = cSPIN_Get_Param(pRsrc, cSPIN_ACC);
	pRsrc->reg.DEC = cSPIN_Get_Param(pRsrc, cSPIN_DEC);
	pRsrc->reg.MAX_SPEED = cSPIN_Get_Param(pRsrc, cSPIN_MAX_SPEED);
	pRsrc->reg.MIN_SPEED = cSPIN_Get_Param(pRsrc, cSPIN_MIN_SPEED);
	pRsrc->reg.FS_SPD = cSPIN_Get_Param(pRsrc, cSPIN_FS_SPD);
#if defined(motor)
	pRsrc->reg.KVAL_HOLD = cSPIN_Get_Param(pRsrc, cSPIN_KVAL_HOLD);
	pRsrc->reg.KVAL_RUN = cSPIN_Get_Param(pRsrc, cSPIN_KVAL_RUN);
	pRsrc->reg.KVAL_ACC = cSPIN_Get_Param(pRsrc, cSPIN_KVAL_ACC);
	pRsrc->reg.KVAL_DEC = cSPIN_Get_Param(pRsrc, cSPIN_KVAL_DEC);
	pRsrc->reg.INT_SPD = cSPIN_Get_Param(pRsrc, cSPIN_INT_SPD);
	pRsrc->reg.ST_SLP = cSPIN_Get_Param(pRsrc, cSPIN_ST_SLP);
	pRsrc->reg.FN_SLP_ACC = cSPIN_Get_Param(pRsrc, cSPIN_FN_SLP_ACC);
	pRsrc->reg.FN_SLP_DEC = cSPIN_Get_Param(pRsrc, cSPIN_FN_SLP_DEC);
	pRsrc->reg.K_THERM = cSPIN_Get_Param(pRsrc, cSPIN_K_THERM);
	pRsrc->reg.STALL_TH = cSPIN_Get_Param(pRsrc, cSPIN_STALL_TH);
#endif /* defined(motor) */
#if defined(L6482)
	pRsrc->reg.TVAL_HOLD = cSPIN_Get_Param(pRsrc, cSPIN_TVAL_HOLD);
	pRsrc->reg.TVAL_RUN = cSPIN_Get_Param(pRsrc, cSPIN_TVAL_RUN);
	pRsrc->reg.TVAL_ACC = cSPIN_Get_Param(pRsrc, cSPIN_TVAL_ACC);
	pRsrc->reg.TVAL_DEC = cSPIN_Get_Param(pRsrc, cSPIN_TVAL_DEC);
	pRsrc->reg.T_FAST = cSPIN_Get_Param(pRsrc, cSPIN_T_FAST);
	pRsrc->reg.TON_MIN = cSPIN_Get_Param(cSPIN_TON_MIN, pRsrc->reg.TON_MIN);
	pRsrc->reg.TOFF_MIN = cSPIN_Get_Param(pRsrc, cpRsrc, SPIN_TOFF_MIN);
#endif /* defined(L6482) */
	pRsrc->reg.OCD_TH = cSPIN_Get_Param(pRsrc, cSPIN_OCD_TH);
	pRsrc->reg.STEP_MODE = cSPIN_Get_Param(pRsrc, cSPIN_STEP_MODE);
	pRsrc->reg.ALARM_EN = cSPIN_Get_Param(pRsrc, cSPIN_ALARM_EN);
	pRsrc->reg.GATECFG1 = cSPIN_Get_Param(pRsrc, cSPIN_GATECFG1);
	pRsrc->reg.GATECFG2 = cSPIN_Get_Param(pRsrc, cSPIN_GATECFG2);
	pRsrc->reg.CONFIG = cSPIN_Get_Param(pRsrc, cSPIN_CONFIG);
}

/*******************************************************************************
* Function Name  : cSPIN_tickExe
* Description    : per
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void cSPIN_tickExe(cSPIN_Rsrc_T* pRsrc){
	pRsrc->tick++;
	if(pRsrc->tick > MOTOR_APP_TICK_IN_4MS){
		pRsrc->tick = 0;
		//push reference sw
		pRsrc->refLSeries <<= 1;
		pRsrc->refRSeries <<= 1;
		if(HAL_GPIO_ReadPin(pRsrc->SW_L.GPIOx, pRsrc->SW_L.GPIO_Pin) == GPIO_PIN_SET)
			pRsrc->refLSeries |= 1;
		if(HAL_GPIO_ReadPin(pRsrc->SW_R.GPIOx, pRsrc->SW_R.GPIO_Pin) == GPIO_PIN_SET)
			pRsrc->refRSeries |= 1;
		//see if there is a rising/falling edge
//		if((pRsrc->refLSeries&0x03) == 0x02)	pRsrc->callBackRefLTurnOn();
//		if((pRsrc->refLSeries&0x03) == 0x01)	pRsrc->callBackRefLRelease();
//		if((pRsrc->refRSeries&0x03) == 0x02)	pRsrc->callBackRefRTurnOn();
//		if((pRsrc->refRSeries&0x03) == 0x01)	pRsrc->callBackRefRRelease();
		//see if absPos changed
		if(pRsrc->flags & 1<<MOTOR_RPT_ABSPOS_BIT){
			pRsrc->regPrv.ABS_POS = pRsrc->reg.ABS_POS;
			pRsrc->reg.ABS_POS = cSPIN_Get_Param(pRsrc, cSPIN_ABS_POS);
//			if(pRsrc->regPrv.ABS_POS ^  pRsrc->reg.ABS_POS)	pRsrc->callBackAbsPosChanged(pRsrc->reg.ABS_POS);		
		}
		//see if ADC_OUT changed
		if(pRsrc->flags & 1<<MOTOR_RPT_ADCOUT_BIT){
			pRsrc->regPrv.ADC_OUT = pRsrc->reg.ADC_OUT;
			pRsrc->reg.ADC_OUT = cSPIN_Get_Param(pRsrc, cSPIN_ADC_OUT);
//			if(pRsrc->regPrv.ADC_OUT ^  pRsrc->reg.ADC_OUT)	pRsrc->callBackAdcOutChanged(pRsrc->reg.ADC_OUT);
		}
		//see if STATUS changed
		if(pRsrc->flags & 1<<MOTOR_RPT_STATUS_BIT){
			pRsrc->regPrv.STATUS = pRsrc->reg.STATUS;
			pRsrc->reg.STATUS = cSPIN_Get_Status(pRsrc);
//			if(pRsrc->regPrv.STATUS ^  pRsrc->reg.STATUS)	pRsrc->callBackStatusChanged(pRsrc->reg.STATUS);
		}
	}
}

/*******************************************************************************
* Function Name  : cmd
* Description    : command
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void cSPIN_EnRptStatus(cSPIN_Rsrc_T* pRsrc){
	pRsrc->flags |= 1<<MOTOR_RPT_STATUS_BIT;
}
void cSPIN_DisRptStatus(cSPIN_Rsrc_T* pRsrc){
	pRsrc->flags &= (1<<MOTOR_RPT_STATUS_BIT)^0xffff;
}
void cSPIN_EnRptAbsPos(cSPIN_Rsrc_T* pRsrc){
	pRsrc->flags |= 1<<MOTOR_RPT_ABSPOS_BIT;
}
void cSPIN_DisRptAbsPos(cSPIN_Rsrc_T* pRsrc){
	pRsrc->flags &= (1<<MOTOR_RPT_ABSPOS_BIT)^0xffff;
}
void cSPIN_EnRptAdcOut(cSPIN_Rsrc_T* pRsrc){
	pRsrc->flags |= 1<<MOTOR_RPT_ADCOUT_BIT;
}
void cSPIN_DisRptAdcOut(cSPIN_Rsrc_T* pRsrc){
	pRsrc->flags &= (1<<MOTOR_RPT_ADCOUT_BIT)^0xffff;
}


/** @} */  
/******************* (C) COPYRIGHT 2013 STMicroelectronics *****END OF FILE****/
