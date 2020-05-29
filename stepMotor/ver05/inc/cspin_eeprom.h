/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : cspin_eeprom.h
  * @brief          : 
  *                   
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CSPIN_EEPROM_H
#define __CSPIN_EEPROM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "cspin.h"

/* Private includes ----------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/


/* Exported functions prototypes ---------------------------------------------*/

/* Private defines -----------------------------------------------------------*/
#define EROM_ABS_POS	(0)
#define EROM_EL_POS	(0)
#define EROM_MARK	(0)
#define EROM_SPEED	(0)
#define EROM_ACC	(0)
#define EROM_DEC	(0)
#define EROM_MAX_SPEED	(0)
#define EROM_MIN_SPEED	(0)
#define EROM_FS_SPD	(0)
#define EROM_KVAL_HOLD	(0)
#define EROM_KVAL_RUN	(0)
#define EROM_KVAL_ACC	(0)
#define EROM_KVAL_DEC	(0)
#define EROM_INT_SPD	(0)
#define EROM_ST_SLP	(0)
#define EROM_FN_SLP_ACC	(0)
#define EROM_FN_SLP_DEC	(0)
#define EROM_K_THERM	(0)
#define EROM_ADC_OUT	(0)
#define EROM_OCD_TH	(0)
#define EROM_STALL_TH	(0)
#define EROM_STEP_MODE	(0)
#define EROM_ALARM_EN	(0)
#define EROM_CONFIG	(0)
#define EROM_STATUS	(0)

	
	//  u32 ABS_POS;
	//  u16 EL_POS;
	//  u32 MARK;
	//  u32 SPEED;
	//  u16 ACC;
	//  u16 DEC;
	//  u16 MAX_SPEED;
	//  u16 MIN_SPEED;
	//  u16 FS_SPD;
	//  u8  KVAL_HOLD;
	//  u8  KVAL_RUN;
	//  u8  KVAL_ACC;
	//  u8  KVAL_DEC;
	//  u16 INT_SPD;
	//  u8  ST_SLP;
	//  u8  FN_SLP_ACC;
	//  u8  FN_SLP_DEC;
	//  u8  K_THERM;
	//  u8  ADC_OUT;
	//  u8  OCD_TH;
	//  u8  STALL_TH;
	//  u8  STEP_MODE;
	//  u8  ALARM_EN;
	//  u16 CONFIG;
	//  u16 STATUS;


//	pDev->RegSetAbsPos = 	cSPIN_SetAbsPos;
//	pDev->RegGetAbsPos = 	cSPIN_GetAbsPos;
//	pDev->RegSetElPos = 	cSPIN_SetElPos;
//	pDev->RegGetElPos = 	cSPIN_GetElPos;
//	pDev->RegSetMarkPos = 	cSPIN_SetMarkPos;
//	pDev->RegGetMarkPos = 	cSPIN_GetMarkPos;
//	pDev->RegGetSpeed = 	cSPIN_GetSpeed;
//	pDev->RegSetAcc = 	cSPIN_SetAcc;
//	pDev->RegGetAcc = 	cSPIN_GetAcc;
//	pDev->RegSetDec = 	cSPIN_SetDec;
//	pDev->RegGetDec = 	cSPIN_GetDec;
//	pDev->RegSetMaxSpd = 	cSPIN_SetMaxSpd;
//	pDev->RegGetMaxSpd = 	cSPIN_GetMaxSpd;
//	pDev->RegSetMinSpd = 	cSPIN_SetMinSpd;
//	pDev->RegGetMinSpd = 	cSPIN_GetMinSpd;
//	
//	pDev->RegSetLowSpdOpt = 	cSPIN_SetLowSpdOpt;
//	pDev->RegGetLowSpdOpt = 	cSPIN_GetLowSpdOpt;
//	pDev->RegSetFSSpd = 	cSPIN_SetFSSpd;
//	pDev->RegGetFSSpd = 	cSPIN_GetFSSpd;
//	pDev->RegSetBoostMode = 	cSPIN_SetBoostMode;
//	pDev->RegGetBoostMode = 	cSPIN_GetBoostMode;
//	
//	pDev->RegSetKValHold = 	cSPIN_SetKValHold;
//	pDev->RegGetKValHold = 	cSPIN_GetKValHold;
//	pDev->RegSetKValRun = 	cSPIN_SetKValRun;
//	pDev->RegGetKValRun = 	cSPIN_GetKValRun;
//	pDev->RegSetKValAcc = 	cSPIN_SetKValAcc;
//	pDev->RegGetKValAcc = 	cSPIN_GetKValAcc;
//	pDev->RegSetKValDec = 	cSPIN_SetKValDec;
//	pDev->RegGetKValDec = 	cSPIN_GetKValDec;
//	
//	pDev->RegSetIntSpd = 	cSPIN_SetIntSpd;
//	pDev->RegGetIntSpd = 	cSPIN_GetIntSpd;
//	pDev->RegSetStartSlp = 	cSPIN_SetStartSlp;
//	pDev->RegGetStartSlp = 	cSPIN_GetStartSlp;
//	pDev->RegSetFnSlpAcc = 	cSPIN_SetFnSlpAcc;
//	pDev->RegGetFnSlpAcc = 	cSPIN_GetFnSlpAcc;
//	pDev->RegSetFnSlpDec = 	cSPIN_SetFnSlpDec;
//	pDev->RegGetFnSlpDec = 	cSPIN_GetFnSlpDec;
//	pDev->RegSetKTherm = 	cSPIN_SetKTherm;
//	pDev->RegGetKTherm = 	cSPIN_GetKTherm;
//	pDev->RegGetAdcOut = 	cSPIN_GetAdcOut;
//	pDev->RegSetOCDTh = 	cSPIN_SetOCDTh;
//	pDev->RegGetOCDTh = 	cSPIN_GetOCDTh;
//	pDev->RegSetStallTh = 	cSPIN_SetStallTh;
//	pDev->RegGetStallTh = 	cSPIN_GetStallTh;
//	
//	pDev->RegSetStepSel = 	cSPIN_SetStepSel;
//	pDev->RegGetStepSel = 	cSPIN_GetStepSel;
//	pDev->RegSetSyncSel = 	cSPIN_SetSyncSel;
//	pDev->RegGetSyncSel = 	cSPIN_GetSyncSel;
//	pDev->RegSetSyncEn = 	cSPIN_SetSyncEn;
//	pDev->RegGetSyncEn = 	cSPIN_GetSyncEn;

//#if(USING_L6480)
//	pDev->RegSetGateTcc = 	cSPIN_SetGateTcc;
//	pDev->RegGetGateTcc = 	cSPIN_GetGateTcc;
//	pDev->RegSetGateCurrent = 	cSPIN_SetGateCurrent;
//	pDev->RegGetGateCurrent = 	cSPIN_GetGateCurrent;
//	pDev->RegSetGateTBoost = 	cSPIN_SetGateTBoost;
//	pDev->RegGetGateTBoost = 	cSPIN_GetGateTBoost;
//	pDev->RegSetGateTDeadTime = 	cSPIN_SetGateTDeadTime;
//	pDev->RegGetGateTDeadTime = 	cSPIN_GetGateTDeadTime;
//	pDev->RegSetGateTBlank = 	cSPIN_SetGateTBlank;
//	pDev->RegGetGateTBlank = 	cSPIN_GetGateTBlank;
//#endif	
//	pDev->RegSetSwMode = cSPIN_SetSwMode;
//	pDev->RegGetSwMode = cSPIN_GetSwMode;
//	pDev->RegSetVsCompEn = cSPIN_SetVsCompEn;
//	pDev->RegGetVsCompEn = cSPIN_GetVsCompEn;
//	pDev->RegSetOCSDEn = cSPIN_SetOCSDEn; 
//	pDev->RegGetOCSDEn = cSPIN_GetOCSDEn;
//	pDev->RegSetUVLOVal =cSPIN_SetUVLOVal; 
//	pDev->RegGetUVLOVal =cSPIN_GetUVLOVal; 
//	pDev->RegSetVccVal = cSPIN_SetVccVal;
//	pDev->RegGetVccVal = cSPIN_GetVccVal; 
//	pDev->RegSetPwmFrq = cSPIN_SetPwmFrq;
//	pDev->RegGetPwmFrq = cSPIN_GetPwmFrq;

//	pDev->Set_Param = cSPIN_Set_Param;
//	pDev->Get_Param		=	cSPIN_Get_Param;
//	
//	pDev->Run					=	cSPIN_Run;
//	pDev->Step_Clock	=	cSPIN_Step_Clock;
//	pDev->Move				=	cSPIN_Move;
//	pDev->Go_To				=	cSPIN_Go_To;
//	pDev->Go_To_Dir		=	cSPIN_Go_To_Dir;
//	pDev->Go_Until		=	cSPIN_Go_Until;
//	pDev->Release_SW	=	cSPIN_Release_SW;
//	pDev->Go_Home			=	cSPIN_Go_Home;
//	pDev->Go_Mark			=	cSPIN_Go_Mark;
//	pDev->Reset_Pos		=	cSPIN_Reset_Pos;
//	pDev->Soft_Stop		=	cSPIN_Soft_Stop;
//	pDev->Hard_Stop		=	cSPIN_Hard_Stop;
//	pDev->Soft_HiZ		=	cSPIN_Soft_HiZ;
//	pDev->Hard_HiZ		=	cSPIN_Hard_HiZ;
//	
//	pDev->Get_Status	=	cSPIN_Get_Status;
//	pDev->ResetDev		= cSPIN_Reset_Device;
//	
//	pDev->tickExe = cSPIN_tickExe;
//	
//	pDev->disableRptAbsPos = cSPIN_DisRptAbsPos;
//	pDev->enableRptAbsPos = cSPIN_EnRptAbsPos;
//	pDev->disableRptStatus = cSPIN_DisRptStatus;
//	pDev->enableRptStatus = cSPIN_EnRptStatus;
//	pDev->disableRptAdcOut = cSPIN_DisRptAdcOut;
//	pDev->enableRptAdcOut = cSPIN_EnRptAdcOut;
//	
//	pDev->Busy_SW			=	cSPIN_Busy_SW;
//	pDev->Flag				=	cSPIN_Flag;

#ifdef __cplusplus
}
#endif

#endif /* cspin_eeprom */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
