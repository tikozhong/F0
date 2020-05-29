/**
  ******************************************************************************
  * @file    cspin.c 
	* @path		D:\Tiko Workplace\project\MBL6470\fw\MBL6470\usr\stepMotor\ver04\src\cspin.c
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
#include "misc.h"
#include "gpioDecal.h"
#include "cspin.h"
#include "cspin_config.h"
#include "cspin_eeprom.h"
#include "board.h"
#include "string.h"

/** @addtogroup cSPIN FW library interface
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define EEPROM_OFFSET_CHK		
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
//static void cSPIN_Delay(__IO u32 nCount);

//static void cSPIN_Regs_Struct_Reset(cSPIN_Rsrc_T* pRsrc);
//static void cSPIN_Registers_SetAll(cSPIN_Rsrc_T* pRsrc);
static void cSPIN_Default_Device(cSPIN_Rsrc_T* pRsrc);
static void cSPIN_Reset_Device(cSPIN_Rsrc_T* pRsrc);
//static u8 cSPIN_Write_Byte(cSPIN_Rsrc_T* pRsrc, u8 byte);
static bool cSPIN_TransmitReceive(cSPIN_Rsrc_T* pRsrc, u8* txbuf, u8* rxbuf, u8 len);
static bool cSPIN_TransmitReceiveX4(cSPIN_Rsrc_T* pRsrc, u8* txbuf, u8* rxbuf);
/* commands(19) --------------------------------------------------------------*/
static void cSPIN_Set_Param			(cSPIN_Rsrc_T* pRsrc, cSPIN_Registers_TypeDef param, uint32_t value);
static uint32_t cSPIN_Get_Param	(cSPIN_Rsrc_T* pRsrc, cSPIN_Registers_TypeDef param);

static bool isCmdErr						(cSPIN_Rsrc_T* pRsrc);
static bool cSPIN_SetAbsPos		(cSPIN_Rsrc_T* pRsrc, u32 value);
static bool cSPIN_GetAbsPos		(cSPIN_Rsrc_T* pRsrc, u32 *value);
static bool cSPIN_SetAbsPos		(cSPIN_Rsrc_T* pRsrc, u32 param);
static bool cSPIN_GetAbsPos		(cSPIN_Rsrc_T* pRsrc, u32 *param);
static bool cSPIN_SetElPos			(cSPIN_Rsrc_T* pRsrc, u16 param);
static bool cSPIN_GetElPos			(cSPIN_Rsrc_T* pRsrc, u16 *param);
static bool cSPIN_SetMarkPos		(cSPIN_Rsrc_T* pRsrc, u32 param);
static bool cSPIN_GetMarkPos		(cSPIN_Rsrc_T* pRsrc, u32 *param);
static bool cSPIN_GetSpeed			(cSPIN_Rsrc_T* pRsrc, u32 *param);
static bool cSPIN_SetAcc				(cSPIN_Rsrc_T* pRsrc, u16 param);
static bool cSPIN_GetAcc				(cSPIN_Rsrc_T* pRsrc, u16 *param);
static bool cSPIN_SetDec				(cSPIN_Rsrc_T* pRsrc, u16 param);
static bool cSPIN_GetDec				(cSPIN_Rsrc_T* pRsrc, u16 *param);
static bool cSPIN_SetMaxSpd		(cSPIN_Rsrc_T* pRsrc, u16 param);
static bool cSPIN_GetMaxSpd		(cSPIN_Rsrc_T* pRsrc, u16 *param);
static bool cSPIN_SetMinSpd		(cSPIN_Rsrc_T* pRsrc, u16 param);
static bool cSPIN_GetMinSpd		(cSPIN_Rsrc_T* pRsrc, u16 *param);
static bool cSPIN_SetLowSpdOpt	(cSPIN_Rsrc_T* pRsrc, bool param);
static bool cSPIN_GetLowSpdOpt	(cSPIN_Rsrc_T* pRsrc, bool *param);
static bool cSPIN_SetFSSpd			(cSPIN_Rsrc_T* pRsrc, u16 param);
static bool cSPIN_GetFSSpd			(cSPIN_Rsrc_T* pRsrc, u16 *param);
//static bool cSPIN_SetBoostMode	(cSPIN_Rsrc_T* pRsrc, bool param);
//static bool cSPIN_GetBoostMode	(cSPIN_Rsrc_T* pRsrc, bool *param);
static bool cSPIN_SetKValHold	(cSPIN_Rsrc_T* pRsrc, u8 param);
static bool cSPIN_GetKValHold	(cSPIN_Rsrc_T* pRsrc, u8 *param);
static bool cSPIN_SetKValRun		(cSPIN_Rsrc_T* pRsrc, u8 param);
static bool cSPIN_GetKValRun		(cSPIN_Rsrc_T* pRsrc, u8 *param);
static bool cSPIN_SetKValAcc		(cSPIN_Rsrc_T* pRsrc, u8 param);
static bool cSPIN_GetKValAcc		(cSPIN_Rsrc_T* pRsrc, u8 *param);
static bool cSPIN_SetKValDec		(cSPIN_Rsrc_T* pRsrc, u8 param);
static bool cSPIN_GetKValDec		(cSPIN_Rsrc_T* pRsrc, u8 *param);
static bool cSPIN_SetIntSpd		(cSPIN_Rsrc_T* pRsrc, u16 param);
static bool cSPIN_GetIntSpd		(cSPIN_Rsrc_T* pRsrc, u16 *param);
static bool cSPIN_SetStartSlp	(cSPIN_Rsrc_T* pRsrc, u8 param);
static bool cSPIN_GetStartSlp	(cSPIN_Rsrc_T* pRsrc, u8 *param);
static bool cSPIN_SetFnSlpAcc	(cSPIN_Rsrc_T* pRsrc, u8 param);
static bool cSPIN_GetFnSlpAcc	(cSPIN_Rsrc_T* pRsrc, u8 *param);
static bool cSPIN_SetFnSlpDec	(cSPIN_Rsrc_T* pRsrc, u8 param);
static bool cSPIN_GetFnSlpDec	(cSPIN_Rsrc_T* pRsrc, u8 *param);
static bool cSPIN_SetKTherm		(cSPIN_Rsrc_T* pRsrc, u8 param);
static bool cSPIN_GetKTherm		(cSPIN_Rsrc_T* pRsrc, u8 *param);
static bool cSPIN_GetAdcOut		(cSPIN_Rsrc_T* pRsrc, u8 *param);
static bool cSPIN_SetOCDTh			(cSPIN_Rsrc_T* pRsrc, u8 param);
static bool cSPIN_GetOCDTh			(cSPIN_Rsrc_T* pRsrc, u8 *param);
static bool cSPIN_SetStallTh		(cSPIN_Rsrc_T* pRsrc, u8 param);
static bool cSPIN_GetStallTh		(cSPIN_Rsrc_T* pRsrc, u8 *param);
static bool cSPIN_SetStepSel		(cSPIN_Rsrc_T* pRsrc, u8 param);
static bool cSPIN_GetStepSel		(cSPIN_Rsrc_T* pRsrc, u8 *param);
static bool cSPIN_SetSyncSel		(cSPIN_Rsrc_T* pRsrc, u8 param);
static bool cSPIN_GetSyncSel		(cSPIN_Rsrc_T* pRsrc, u8 *param);
static bool cSPIN_SetSyncEn		(cSPIN_Rsrc_T* pRsrc, u8 param);
static bool cSPIN_GetSyncEn		(cSPIN_Rsrc_T* pRsrc, u8 *param);
#if(USING_L6480)
static bool cSPIN_SetGateTcc		(cSPIN_Rsrc_T* pRsrc, u8 param);
static bool cSPIN_GetGateTcc		(cSPIN_Rsrc_T* pRsrc, u8 *param);
static bool cSPIN_SetGateCurrent		(cSPIN_Rsrc_T* pRsrc, u8 param);
static bool cSPIN_GetGateCurrent		(cSPIN_Rsrc_T* pRsrc, u8 *param);
static bool cSPIN_SetGateTBoost		(cSPIN_Rsrc_T* pRsrc, u8 param);
static bool cSPIN_GetGateTBoost		(cSPIN_Rsrc_T* pRsrc, u8 *param);
static bool cSPIN_SetGateTDeadTime	(cSPIN_Rsrc_T* pRsrc, u8 param);
static bool cSPIN_GetGateTDeadTime	(cSPIN_Rsrc_T* pRsrc, u8 *param);
static bool cSPIN_SetGateTBlank		(cSPIN_Rsrc_T* pRsrc, u8 param);
static bool cSPIN_GetGateTBlank		(cSPIN_Rsrc_T* pRsrc, u8 *param);
#endif
static bool cSPIN_SetConfig (cSPIN_Rsrc_T* pRsrc, u16 param);
static bool cSPIN_GetConfig (cSPIN_Rsrc_T* pRsrc, u16 *param);


static void cSPIN_Run						(cSPIN_Rsrc_T* pRsrc, cSPIN_Direction_T direction, uint32_t speed);
static void cSPIN_RunTimeout		(cSPIN_Rsrc_T* pRsrc, cSPIN_Direction_T direction, uint32_t speed, u32 timeout);
static void cSPIN_Step_Clock			(cSPIN_Rsrc_T* pRsrc, cSPIN_Direction_T direction);
static void cSPIN_Move						(cSPIN_Rsrc_T* pRsrc, cSPIN_Direction_T direction, uint32_t n_step);
static void cSPIN_Go_To					(cSPIN_Rsrc_T* pRsrc, uint32_t abs_pos);
static void cSPIN_Go_To_Dir			(cSPIN_Rsrc_T* pRsrc, cSPIN_Direction_T direction, uint32_t abs_pos);
static void cSPIN_Go_Until				(cSPIN_Rsrc_T* pRsrc, cSPIN_Action_T action, cSPIN_Direction_T direction, uint32_t speed);
static void cSPIN_Release_SW			(cSPIN_Rsrc_T* pRsrc, cSPIN_Action_T action, cSPIN_Direction_T direction);
static void cSPIN_Go_Home				(cSPIN_Rsrc_T* pRsrc);
static void cSPIN_Go_Mark				(cSPIN_Rsrc_T* pRsrc);
static void cSPIN_Reset_Pos			(cSPIN_Rsrc_T* pRsrc);
static void cSPIN_Soft_Stop			(cSPIN_Rsrc_T* pRsrc);
static void cSPIN_Hard_Stop			(cSPIN_Rsrc_T* pRsrc);
static void cSPIN_Soft_HiZ				(cSPIN_Rsrc_T* pRsrc);
static void cSPIN_Hard_HiZ				(cSPIN_Rsrc_T* pRsrc);

static void cSPIN_tickExe(cSPIN_Rsrc_T* pRsrc, u8 pollingTm);

static void cSPIN_EnRptStatus(cSPIN_Rsrc_T* pRsrc);
static void cSPIN_DisRptStatus(cSPIN_Rsrc_T* pRsrc);
static void cSPIN_EnRptAbsPos(cSPIN_Rsrc_T* pRsrc);
static void cSPIN_DisRptAbsPos(cSPIN_Rsrc_T* pRsrc);
static void cSPIN_EnRptAdcOut(cSPIN_Rsrc_T* pRsrc);
static void cSPIN_DisRptAdcOut(cSPIN_Rsrc_T* pRsrc);

static uint16_t cSPIN_Get_Status	(cSPIN_Rsrc_T* pRsrc);
static uint8_t cSPIN_Busy_SW(cSPIN_Rsrc_T* pRsrc);
static uint8_t cSPIN_Flag(cSPIN_Rsrc_T* pRsrc);

static void cSPIN_EromWrite(cSPIN_Rsrc_T* pRsrc, u8 addr, u32 val);
static bool cSPIN_EromRead(cSPIN_Rsrc_T* pRsrc, u8 addr, u32* val);

/**
  * @brief  Setup a motor to be ready to run
  * @param  None
  * @retval None
  */
void cSPIN_Setup1(
	cSPIN_DEV_T *pDev, 
	const char* NAME, 
	SPI_HandleTypeDef* SPI_HANDLE,
	PIN_T CS, 	
	
	PIN_T SW_L,
	PIN_T SW_R,
	
	PIN_T FLAG,
	PIN_T SYNC,
	PIN_T STCK,
	
  s8 (*EepromRead)   (u16 addr, u8 *pDat, u16 nBytes),
  s8 (*EepromWrite)  (u16 addr, const u8 *pDat, u16 nBytes)
){
	cSPIN_Setup(pDev,NAME,SPI_HANDLE,CS,SW_L,SW_R,FLAG,SYNC,STCK);
	pDev->rsrc.EepromRead = EepromRead;
  pDev->rsrc.EepromWrite = EepromWrite;
}
	
void cSPIN_Setup(
	cSPIN_DEV_T *pDev, 
	const char* NAME, 
	SPI_HandleTypeDef* SPI_HANDLE,
	PIN_T CS, 	
	
	PIN_T SW_L,
	PIN_T SW_R,
	
	PIN_T FLAG,
	PIN_T SYNC,
	PIN_T STCK
){
	cSPIN_Rsrc_T* pRsrc = &pDev->rsrc;		
	pRsrc->SPI_HANDLE = SPI_HANDLE;
	pRsrc->CS = CS;		
	pRsrc->SW_L = SW_L;
	pRsrc->SW_R = SW_R;
	pRsrc->FLG = FLAG;
	pRsrc->SYNC = SYNC;
	pRsrc->STCK = STCK;
	memset(pRsrc->name,0, DEV_NAME_LEN);
	strcpy(pRsrc->name, NAME);

	pRsrc->refLSeries = 0xff;
	pRsrc->refRSeries = 0xff;
	pRsrc->runTmr = 0;
	
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
//	pDev->RegSetBoostMode = 	cSPIN_SetBoostMode;
//	pDev->RegGetBoostMode = 	cSPIN_GetBoostMode;
	
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

#if(USING_L6480)
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
#endif	
	pDev->RegSetCofig = cSPIN_SetConfig;
	pDev->RegGetCofig = cSPIN_GetConfig;

	pDev->Set_Param = cSPIN_Set_Param;
	pDev->Get_Param		=	cSPIN_Get_Param;
	
	pDev->Default = cSPIN_Default_Device;
	pDev->ResetDev		= cSPIN_Reset_Device;	
	
	pDev->Run					=	cSPIN_Run;
	pDev->RunTimeout	= cSPIN_RunTimeout;
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
	
	pDev->Polling = cSPIN_tickExe;
	
	pDev->disableRptAbsPos = cSPIN_DisRptAbsPos;
	pDev->enableRptAbsPos = cSPIN_EnRptAbsPos;
	pDev->disableRptStatus = cSPIN_DisRptStatus;
	pDev->enableRptStatus = cSPIN_EnRptStatus;
	pDev->disableRptAdcOut = cSPIN_DisRptAdcOut;
	pDev->enableRptAdcOut = cSPIN_EnRptAdcOut;
	
	pDev->Busy_SW			=	cSPIN_Busy_SW;
	pDev->Flag				=	cSPIN_Flag;

	cSPIN_Reset_Device(pRsrc);


//	/* Structure initialization by default values, in order to avoid blank records */
//	/***********************************************
//	@ Load register val from cspin_config.h
//	***********************************************/
//	/* Acceleration rate settings to cSPIN_CONF_PARAM_ACC in steps/s2, range 14.55 to 59590 steps/s2 */
//	pRsrc->reg.ACC 		= AccDec_Steps_to_Par(cSPIN_CONF_PARAM_ACC);
//	/* Deceleration rate settings to cSPIN_CONF_PARAM_DEC in steps/s2, range 14.55 to 59590 steps/s2 */
//	pRsrc->reg.DEC 		= AccDec_Steps_to_Par(cSPIN_CONF_PARAM_DEC); 
//	/* Maximum speed settings to cSPIN_CONF_PARAM_MAX_SPEED in steps/s, range 15.25 to 15610 steps/s */
//	pRsrc->reg.MAX_SPEED 	= MaxSpd_Steps_to_Par(cSPIN_CONF_PARAM_MAX_SPEED);
//	/* Full step speed settings cSPIN_CONF_PARAM_FS_SPD in steps/s, range 7.63 to 15625 steps/s */
//	pRsrc->reg.FS_SPD 	= FSSpd_Steps_to_Par(cSPIN_CONF_PARAM_FS_SPD);
//#if defined(motor)
//	/* Minimum speed settings to cSPIN_CONF_PARAM_MIN_SPEED in steps/s, range 0 to 976.3 steps/s */
//	pRsrc->reg.MIN_SPEED	= cSPIN_CONF_PARAM_LSPD_BIT|MinSpd_Steps_to_Par(cSPIN_CONF_PARAM_MIN_SPEED);
//  /* Acceleration duty cycle (torque) settings to cSPIN_CONF_PARAM_KVAL_ACC in %, range 0 to 99.6% */
//	pRsrc->reg.KVAL_ACC 	= Kval_Perc_to_Par(cSPIN_CONF_PARAM_KVAL_ACC);
//  /* Deceleration duty cycle (torque) settings to cSPIN_CONF_PARAM_KVAL_DEC in %, range 0 to 99.6% */
//	pRsrc->reg.KVAL_DEC 	= Kval_Perc_to_Par(cSPIN_CONF_PARAM_KVAL_DEC);		
//  /* Run duty cycle (torque) settings to cSPIN_CONF_PARAM_KVAL_RUN in %, range 0 to 99.6% */
//	pRsrc->reg.KVAL_RUN 	= Kval_Perc_to_Par(cSPIN_CONF_PARAM_KVAL_RUN);
//  /* Hold duty cycle (torque) settings to cSPIN_CONF_PARAM_KVAL_HOLD in %, range 0 to 99.6% */
//	pRsrc->reg.KVAL_HOLD 	= Kval_Perc_to_Par(cSPIN_CONF_PARAM_KVAL_HOLD);
//	/* Thermal compensation param settings to cSPIN_CONF_PARAM_K_THERM, range 1 to 1.46875 */
//	pRsrc->reg.K_THERM 	= KTherm_to_Par(cSPIN_CONF_PARAM_K_THERM);
//	/* Intersect speed settings for BEMF compensation to cSPIN_CONF_PARAM_INT_SPD in steps/s, range 0 to 3906 steps/s */
//	pRsrc->reg.INT_SPD 	= IntSpd_Steps_to_Par(cSPIN_CONF_PARAM_INT_SPD);
//	/* BEMF start slope settings for BEMF compensation to cSPIN_CONF_PARAM_ST_SLP in % step/s, range 0 to 0.4% s/step */
//	pRsrc->reg.ST_SLP 	= BEMF_Slope_Perc_to_Par(cSPIN_CONF_PARAM_ST_SLP);
//	/* BEMF final acc slope settings for BEMF compensation to cSPIN_CONF_PARAM_FN_SLP_ACC in% step/s, range 0 to 0.4% s/step */
//	pRsrc->reg.FN_SLP_ACC = BEMF_Slope_Perc_to_Par(cSPIN_CONF_PARAM_FN_SLP_ACC);
//	/* BEMF final dec slope settings for BEMF compensation to cSPIN_CONF_PARAM_FN_SLP_DEC in% step/s, range 0 to 0.4% s/step */
//	pRsrc->reg.FN_SLP_DEC = BEMF_Slope_Perc_to_Par(cSPIN_CONF_PARAM_FN_SLP_DEC);
//	/* Stall threshold settings to cSPIN_CONF_PARAM_STALL_TH in mV, range 31.25 to 1000mV */
//	pRsrc->reg.STALL_TH 	= StallTh_to_Par(cSPIN_CONF_PARAM_STALL_TH);
//  /* Set Config register according to config parameters */
//  /* clock setting, switch hard stop interrupt mode, */
//  /*  supply voltage compensation, overcurrent shutdown */
//  /* UVLO threshold, VCC reg output voltage , PWM frequency */
//	pRsrc->reg.CONFIG 	= (u16)cSPIN_CONF_PARAM_CLOCK_SETTING | \
//                        (u16)cSPIN_CONF_PARAM_SW_MODE	  | \
//                        (u16)cSPIN_CONF_PARAM_VS_COMP		| \
//                        (u16)cSPIN_CONF_PARAM_OC_SD      | \
//                        (u16)cSPIN_CONF_PARAM_UVLOVAL    | \
//                        (u16)cSPIN_CONF_PARAM_VCCVAL	   	| \
//                        (u16)cSPIN_CONF_PARAM_PWM_DIV    | \
//                        (u16)cSPIN_CONF_PARAM_PWM_MUL;
//#endif /* defined(motor) */
//#if defined(L6482)
//        /* Minimum speed settings to cSPIN_CONF_PARAM_MIN_SPEED in steps/s, range 0 to 976.3 steps/s */
//	pRsrc->reg.MIN_SPEED	= MinSpd_Steps_to_Par(cSPIN_CONF_PARAM_MIN_SPEED);
//        /* Reference voltage assigned to the torque regulation DAC during motor acceleration, range 7.8mV to 1000mV */
//	pRsrc->reg.TVAL_ACC 	= Tval_Current_to_Par(cSPIN_CONF_PARAM_TVAL_ACC);
//        /* Reference voltage assigned to the torque regulation DAC during motor deceleration, range 7.8mV to 1000mV */
//	pRsrc->reg.TVAL_DEC 	= Tval_Current_to_Par(cSPIN_CONF_PARAM_TVAL_DEC);		
//        /* Reference voltage assigned to the torque regulation DAC when motor is running, range 7.8mV to 1000mV */
//	pRsrc->reg.TVAL_RUN 	= Tval_Current_to_Par(cSPIN_CONF_PARAM_TVAL_RUN);
//        /* Reference voltage assigned to the torque regulation DAC when motor is stopped, range 7.8mV to 1000mV */
//	pRsrc->reg.TVAL_HOLD 	= Tval_Current_to_Par(cSPIN_CONF_PARAM_TVAL_HOLD);
//				/* Maximum fast decay and fall step times used by the current control system, range 2us to 32us */
//	pRsrc->reg.T_FAST 	= (u8)cSPIN_CONF_PARAM_TOFF_FAST | (u8)cSPIN_CONF_PARAM_FAST_STEP;
//				/* Minimum ON time value used by the current control system, range 0.5us to 64us */
//	pRsrc->reg.TON_MIN 	= Tmin_Time_to_Par(cSPIN_CONF_PARAM_TON_MIN);
//				/* Minimum OFF time value used by the current control system, range 0.5us to 64us */
//	pRsrc->reg.TOFF_MIN	= Tmin_Time_to_Par(cSPIN_CONF_PARAM_TOFF_MIN);
//				/* Set Config register according to config parameters */
//				/* clock setting, switch hard stop interrupt mode, */
//				/*  supply voltage compensation, overcurrent shutdown */
//        /* UVLO threshold, VCC reg output voltage , target switching period, predictive current control */
//	pRsrc->reg.CONFIG 	= (u16)cSPIN_CONF_PARAM_CLOCK_SETTING | \
//                                          (u16)cSPIN_CONF_PARAM_SW_MODE	   | \
//                                          (u16)cSPIN_CONF_PARAM_TQ_REG     | \
//                                          (u16)cSPIN_CONF_PARAM_OC_SD      | \
//                                          (u16)cSPIN_CONF_PARAM_UVLOVAL    | \
//                                          (u16)cSPIN_CONF_PARAM_VCCVAL	   | \
//                                          (u16)cSPIN_CONF_PARAM_TSW        | \
//                                          (u16)cSPIN_CONF_PARAM_PRED;        
//#endif /* defined(L6482) */
//	/* Overcurrent threshold settings to cSPIN_CONF_PARAM_OCD_TH, range 31.25 to 1000mV */
//	pRsrc->reg.OCD_TH 	= cSPIN_CONF_PARAM_OCD_TH;        
//  /* Alarm settings to cSPIN_CONF_PARAM_ALARM_EN */
//	pRsrc->reg.ALARM_EN 	= cSPIN_CONF_PARAM_ALARM_EN;
//  /* Step mode and sycn mode settings via cSPIN_CONF_PARAM_SYNC_MODE and cSPIN_CONF_PARAM_STEP_MODE */
//	pRsrc->reg.STEP_MODE 	= (u8)cSPIN_CONF_PARAM_SYNC_MODE | \
//                          (u8)cSPIN_CONF_PARAM_STEP_MODE;
//  /* Sink/source current, duration of constant current phases, duration of overboost phase settings */
//  pRsrc->reg.GATECFG1   = (u16)cSPIN_CONF_PARAM_IGATE | \
//													(u16)cSPIN_CONF_PARAM_TCC   | \
//													(u16)cSPIN_CONF_PARAM_TBOOST;
//	/* Blank time, Dead time stiings */
//	 pRsrc->reg.GATECFG2  = (u16)cSPIN_CONF_PARAM_TBLANK | \
//													(u16)cSPIN_CONF_PARAM_TDT;
//  /* Program all cSPIN registers */
//  
//	cSPIN_Regs_Struct_Reset(pRsrc);
//	cSPIN_Registers_SetAll(pRsrc);
             
	/* Clear Flag pin */
	pRsrc->reg.STATUS = cSPIN_Get_Status(pRsrc); 
}

/**
  * @brief  Issues cSPIN NOP command.
  * @param  None
  * @retval None
  */
//static void cSPIN_Nop(cSPIN_Rsrc_T* pRsrc)
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
static void cSPIN_Set_Param(cSPIN_Rsrc_T* pRsrc, cSPIN_Registers_TypeDef param, u32 value)
{
	u8 txbuf[4] = {0};

	txbuf[0] = (u8)cSPIN_SET_PARAM | (u8)param;
	switch (param)
	{
		/* These following REG need 3 bytes */
		case cSPIN_ABS_POS: ;
		case cSPIN_MARK: ;
			txbuf[1] = (u8)(value >> 16);	
			txbuf[2] = (u8)(value >> 8);
			txbuf[3] = (u8)(value);
			cSPIN_TransmitReceiveX4(pRsrc, txbuf, NULL);
			break;
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
			txbuf[1] = (u8)(value >> 8);	
			txbuf[2] = (u8)(value);
			cSPIN_TransmitReceiveX4(pRsrc, txbuf, NULL);
			break;
		default:
			txbuf[1] = (u8)(value);
			cSPIN_TransmitReceiveX4(pRsrc, txbuf, NULL);
	}
}

/**
  * @brief  Issues cSPIN Get Param command.
  * @param  param cSPIN register address
  * @retval Register value - 1 to 3 bytes (depends on register)
  */
static u32 cSPIN_Get_Param(cSPIN_Rsrc_T* pRsrc, cSPIN_Registers_TypeDef param)
{
	u8 txbuf[4]={0}, rxbuf[4]={0};
	u32 rx = 0;
	txbuf[0] = (u8)cSPIN_GET_PARAM | (u8)param;	
	switch (param)
	{
		case cSPIN_ABS_POS: ;
		case cSPIN_MARK: ;
		case cSPIN_SPEED:
			cSPIN_TransmitReceiveX4(pRsrc, txbuf, rxbuf);
			rx = rxbuf[1];	rx <<=8;
			rx |= rxbuf[2];	rx <<=8;
			rx |= rxbuf[3];	
			break;
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
			cSPIN_TransmitReceiveX4(pRsrc, txbuf, rxbuf);
			rx = rxbuf[1];	rx <<=8;
			rx |= rxbuf[2];	
			break;
		default:
			cSPIN_TransmitReceiveX4(pRsrc, txbuf, rxbuf);	
			rx = rxbuf[1];		
	}
	return rx;
	
	
//	u32 rx = 0;
//	/* Send GetParam operation code to cSPIN */
//	cSPIN_Write_Byte(pRsrc, (u8)cSPIN_GET_PARAM | (u8)param);
//	/* read back data */
//	switch (param)
//	{
//		case cSPIN_ABS_POS: ;
//		case cSPIN_MARK: ;
//		case cSPIN_SPEED:
//			rx = cSPIN_Write_Byte(pRsrc, 0x00);		rx <<= 8;
//		case cSPIN_EL_POS: ;
//		case cSPIN_ACC: ;
//		case cSPIN_DEC: ;
//		case cSPIN_MAX_SPEED: ;
//		case cSPIN_MIN_SPEED: ;
//		case cSPIN_FS_SPD: ;
//#if defined(motor)
//		case cSPIN_INT_SPD: ;
//#endif /* defined(motor) */
//		case cSPIN_CONFIG: ;
//		case cSPIN_STATUS:
//			rx |= cSPIN_Write_Byte(pRsrc, 0x00);		rx <<= 8;
//		default:
//			rx |= cSPIN_Write_Byte(pRsrc, 0x00);		
//	}
//	cSPIN_Delay(100);	//about 1us @ 48MHz	
//	return rx;
}

/*****************************************
  * @brief  Issues cSPIN registers ops
  * @param  value, 22bit
  * @param  
  * @retval None
*****************************************/
static bool isCmdErr(cSPIN_Rsrc_T* pRsrc){

	if((pRsrc->FLG.GPIOx == NULL) || HAL_GPIO_ReadPin(pRsrc->FLG.GPIOx, pRsrc->FLG.GPIO_Pin) == GPIO_PIN_RESET){
		if(cSPIN_Get_Status(pRsrc) & (1<<7) )		return (TRUE);
		return (FALSE);
	}
	else{	return (FALSE);	}
//	cSPIN_Delay(48);	//about 1us @ 48MHz	
//	if(cSPIN_Get_Status(pRsrc) & (1<<7) )
//		return CMD_FAIL;
//	cSPIN_Delay(48);
//	if(cSPIN_Get_Param(pRsrc, cSPIN_ADC_OUT) == 0)	return CMD_FAIL;
//	return CMD_PASS;		
}

/*****************************************
  * @brief  Issues cSPIN registers ops
  * @param  true value, 22bit
  * @param  
  * @retval None
*****************************************/
static bool cSPIN_SetAbsPos(cSPIN_Rsrc_T* pRsrc, u32 param){
	u32 b32x0 = param & BIT_LEN(22);
	cSPIN_Set_Param(pRsrc, cSPIN_ABS_POS, b32x0);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	cSPIN_EromWrite(pRsrc, cSPIN_ABS_POS, b32x0);
	return TRUE;
}

static bool cSPIN_GetAbsPos(cSPIN_Rsrc_T* pRsrc, u32 *param){
	*param = cSPIN_Get_Param(pRsrc, cSPIN_ABS_POS);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	return TRUE;
}

static bool cSPIN_SetElPos(cSPIN_Rsrc_T* pRsrc, u16 param){
	u32 b32x0 = param & BIT_LEN(9);
	u32 b32x1 = 0;
	cSPIN_Set_Param(pRsrc, cSPIN_EL_POS, b32x0);
	cSPIN_EromWrite(pRsrc, cSPIN_EL_POS, b32x0);	
	if(cSPIN_GetElPos(pRsrc, (u16*)&b32x1) == FALSE)	return FALSE;
	return (b32x0 == b32x1);
}
static bool cSPIN_GetElPos(cSPIN_Rsrc_T* pRsrc, u16 *param){
	*param = (u16)cSPIN_Get_Param(pRsrc, cSPIN_EL_POS);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	return TRUE;
}

static bool cSPIN_SetMarkPos(cSPIN_Rsrc_T* pRsrc, u32 param){
	u32 b32x0 = param & BIT_LEN(22);
	u32 b32x1 = 0;
	cSPIN_Set_Param(pRsrc, cSPIN_MARK, b32x0);
	cSPIN_EromWrite(pRsrc, cSPIN_MARK, b32x0);	
	if(cSPIN_GetMarkPos(pRsrc, &b32x1) == FALSE)	return FALSE;
	return (b32x0 == b32x1);
}
static bool cSPIN_GetMarkPos(cSPIN_Rsrc_T* pRsrc, u32 *param){
	*param = cSPIN_Get_Param(pRsrc, cSPIN_MARK);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	return TRUE;
}

static bool cSPIN_GetSpeed(cSPIN_Rsrc_T* pRsrc, u32 *param){
	*param = cSPIN_Get_Param(pRsrc, cSPIN_SPEED);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	return TRUE;
}

static bool cSPIN_SetAcc(cSPIN_Rsrc_T* pRsrc, u16 param){
	u32 b32x0 = param & BIT_LEN(12);
	cSPIN_Set_Param(pRsrc, cSPIN_ACC, b32x0);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	cSPIN_EromWrite(pRsrc, cSPIN_ACC, b32x0);
	return TRUE;
}
static bool cSPIN_GetAcc(cSPIN_Rsrc_T* pRsrc, u16 *param){
	*param = (u16)cSPIN_Get_Param(pRsrc, cSPIN_ACC);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	return TRUE;
}

static bool cSPIN_SetDec(cSPIN_Rsrc_T* pRsrc, u16 param){
	u32 b32x0 = param & BIT_LEN(12);
	cSPIN_Set_Param(pRsrc, cSPIN_DEC, b32x0);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	cSPIN_EromWrite(pRsrc, cSPIN_DEC, b32x0);
	return TRUE;
}
static bool cSPIN_GetDec(cSPIN_Rsrc_T* pRsrc, u16 *param){
	*param = (u16)cSPIN_Get_Param(pRsrc, cSPIN_DEC);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	return TRUE;
}

static bool cSPIN_SetMaxSpd(cSPIN_Rsrc_T* pRsrc, u16 param){
	u32 b32x0 = param & BIT_LEN(10);
	cSPIN_Set_Param(pRsrc, cSPIN_MAX_SPEED, b32x0);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	cSPIN_EromWrite(pRsrc, cSPIN_MAX_SPEED, b32x0);
	return TRUE;
}
static bool cSPIN_GetMaxSpd(cSPIN_Rsrc_T* pRsrc, u16 *param){
	*param = (u16)cSPIN_Get_Param(pRsrc, cSPIN_MAX_SPEED);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	return TRUE;
}

static bool cSPIN_SetMinSpd(cSPIN_Rsrc_T* pRsrc, u16 param){
	u32 b32x0 = param & BIT_LEN(12);
	//read back 
	u32 b32x1 = cSPIN_Get_Param(pRsrc, cSPIN_MIN_SPEED);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	b32x0 |= (b32x1 & BIT(12));
	
	cSPIN_Set_Param(pRsrc, cSPIN_MIN_SPEED, b32x0);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	cSPIN_EromWrite(pRsrc, cSPIN_MIN_SPEED, b32x0);
	return TRUE;
}
static bool cSPIN_GetMinSpd(cSPIN_Rsrc_T* pRsrc, u16 *param){
	*param = cSPIN_Get_Param(pRsrc, cSPIN_MIN_SPEED)&BIT_LEN(12);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	return TRUE;
}

static bool cSPIN_SetLowSpdOpt(cSPIN_Rsrc_T* pRsrc, bool param){
	u32 b32x0;
	//read back 
	u32 b32x1 = cSPIN_Get_Param(pRsrc, cSPIN_MIN_SPEED);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	
	b32x0 = b32x1 & BIT_LEN(12);
	if(param)		b32x0 |= BIT(12);
	cSPIN_Set_Param(pRsrc, cSPIN_MIN_SPEED, b32x0);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	cSPIN_EromWrite(pRsrc, cSPIN_MIN_SPEED, b32x0);
	return TRUE;
}
static bool cSPIN_GetLowSpdOpt(cSPIN_Rsrc_T* pRsrc, bool *param){
	if(cSPIN_Get_Param(pRsrc, cSPIN_MIN_SPEED)& BIT(12))	*param = TRUE;
	else	*param = FALSE;
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	return TRUE;
}

static bool cSPIN_SetFSSpd(cSPIN_Rsrc_T* pRsrc, u16 param){
	u32 b32x0 = param & BIT_LEN(10);
	cSPIN_Set_Param(pRsrc, cSPIN_FS_SPD, b32x0);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	cSPIN_EromWrite(pRsrc, cSPIN_FS_SPD, b32x0);
	return TRUE;
}
static bool cSPIN_GetFSSpd(cSPIN_Rsrc_T* pRsrc, u16 *param){
	*param = (u16)cSPIN_Get_Param(pRsrc, cSPIN_FS_SPD) & BIT_LEN(10);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	return TRUE;
}

//static bool cSPIN_SetBoostMode(cSPIN_Rsrc_T* pRsrc, bool param){
//	u32 b32x0 = param & BIT_LEN(22);
//	cSPIN_Set_Param(pRsrc, cSPIN_ABS_POS, b32x0);
//	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
//	cSPIN_EromWrite(pRsrc, cSPIN_ABS_POS, b32x0);
//	return TRUE;

//	u16 reg = 0;
//	reg = (u16)cSPIN_Get_Param(pRsrc, cSPIN_FS_SPD)&(0xffff>>(16-10));
//	if(isCmdErr(pRsrc)==CMD_FAIL)	return CMD_FAIL;
//	if(param == FALSE)
//		cSPIN_Set_Param(pRsrc, cSPIN_FS_SPD, reg);
//	else
//		cSPIN_Set_Param(pRsrc, cSPIN_FS_SPD, reg|(1<<10));
//	return isCmdErr(pRsrc);
//}
//static bool cSPIN_GetBoostMode(cSPIN_Rsrc_T* pRsrc, bool *param){
//	if(cSPIN_Get_Param(pRsrc, cSPIN_FS_SPD)&(1<<10))
//		*param = TRUE;
//	else
//		*param = FALSE;
//	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
//	return TRUE;
//}

static bool cSPIN_SetKValHold (cSPIN_Rsrc_T* pRsrc, u8 param){
	u32 b32x0 = param & BIT_LEN(8);
	cSPIN_Set_Param(pRsrc, cSPIN_KVAL_HOLD, b32x0);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	cSPIN_EromWrite(pRsrc, cSPIN_KVAL_HOLD, b32x0);
	return TRUE;
}
static bool cSPIN_GetKValHold (cSPIN_Rsrc_T* pRsrc, u8 *param){
	*param = (u8)cSPIN_Get_Param(pRsrc, cSPIN_KVAL_HOLD);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	return TRUE;
}

static bool cSPIN_SetKValRun (cSPIN_Rsrc_T* pRsrc, u8 param){
	u32 b32x0 = param & BIT_LEN(8);
	cSPIN_Set_Param(pRsrc, cSPIN_KVAL_RUN, b32x0);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	cSPIN_EromWrite(pRsrc, cSPIN_KVAL_RUN, b32x0);
	return TRUE;
}
static bool cSPIN_GetKValRun (cSPIN_Rsrc_T* pRsrc, u8 *param){
	*param = (u8)cSPIN_Get_Param(pRsrc, cSPIN_KVAL_RUN);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	return TRUE;
}

static bool cSPIN_SetKValAcc (cSPIN_Rsrc_T* pRsrc, u8 param){
	u32 b32x0 = param & BIT_LEN(8);
	cSPIN_Set_Param(pRsrc, cSPIN_KVAL_ACC, b32x0);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	cSPIN_EromWrite(pRsrc, cSPIN_KVAL_ACC, b32x0);
	return TRUE;
}
static bool cSPIN_GetKValAcc (cSPIN_Rsrc_T* pRsrc, u8 *param){
	*param = (u8)cSPIN_Get_Param(pRsrc, cSPIN_KVAL_ACC);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	return TRUE;
}

static bool cSPIN_SetKValDec (cSPIN_Rsrc_T* pRsrc, u8 param){
	u32 b32x0 = param & BIT_LEN(8);
	cSPIN_Set_Param(pRsrc, cSPIN_KVAL_DEC, b32x0);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	cSPIN_EromWrite(pRsrc, cSPIN_KVAL_DEC, b32x0);
	return TRUE;
}
static bool cSPIN_GetKValDec (cSPIN_Rsrc_T* pRsrc, u8 *param){
	*param = (u8)cSPIN_Get_Param(pRsrc, cSPIN_KVAL_DEC);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	return TRUE;
}

static bool cSPIN_SetIntSpd (cSPIN_Rsrc_T* pRsrc, u16 param){
	u32 b32x0 = param & BIT_LEN(14);
	cSPIN_Set_Param(pRsrc, cSPIN_INT_SPD, b32x0);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	cSPIN_EromWrite(pRsrc, cSPIN_INT_SPD, b32x0);
	return TRUE;
}
static bool cSPIN_GetIntSpd (cSPIN_Rsrc_T* pRsrc, u16 *param){
	*param = (u16)cSPIN_Get_Param(pRsrc, cSPIN_INT_SPD) & BIT_LEN(14);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	return TRUE;
}

static bool cSPIN_SetStartSlp (cSPIN_Rsrc_T* pRsrc, u8 param){
	u32 b32x0 = param & BIT_LEN(8);
	cSPIN_Set_Param(pRsrc, cSPIN_ST_SLP, b32x0);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	cSPIN_EromWrite(pRsrc, cSPIN_ST_SLP, b32x0);
	return TRUE;
}
static bool cSPIN_GetStartSlp (cSPIN_Rsrc_T* pRsrc, u8 *param){
	*param = (u8)cSPIN_Get_Param(pRsrc, cSPIN_ST_SLP);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	return TRUE;
}

static bool cSPIN_SetFnSlpAcc (cSPIN_Rsrc_T* pRsrc, u8 param){
	u32 b32x0 = param & BIT_LEN(8);
	cSPIN_Set_Param(pRsrc, cSPIN_FN_SLP_ACC, b32x0);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	cSPIN_EromWrite(pRsrc, cSPIN_FN_SLP_ACC, b32x0);
	return TRUE;
}
static bool cSPIN_GetFnSlpAcc (cSPIN_Rsrc_T* pRsrc, u8 *param){
	*param = (u8)cSPIN_Get_Param(pRsrc, cSPIN_FN_SLP_ACC);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	return TRUE;
}

static bool cSPIN_SetFnSlpDec (cSPIN_Rsrc_T* pRsrc, u8 param){
	u32 b32x0 = param & BIT_LEN(8);
	cSPIN_Set_Param(pRsrc, cSPIN_FN_SLP_DEC, b32x0);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	cSPIN_EromWrite(pRsrc, cSPIN_FN_SLP_DEC, b32x0);
	return TRUE;
}
static bool cSPIN_GetFnSlpDec (cSPIN_Rsrc_T* pRsrc, u8 *param){
	*param = (u8)cSPIN_Get_Param(pRsrc, cSPIN_FN_SLP_DEC);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	return TRUE;
}

static bool cSPIN_SetKTherm (cSPIN_Rsrc_T* pRsrc, u8 param){
	u32 b32x0 = param & BIT_LEN(4);
	cSPIN_Set_Param(pRsrc, cSPIN_K_THERM, b32x0);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	cSPIN_EromWrite(pRsrc, cSPIN_K_THERM, b32x0);
	return TRUE;
}
static bool cSPIN_GetKTherm (cSPIN_Rsrc_T* pRsrc, u8 *param){
	*param = (u8)cSPIN_Get_Param(pRsrc, cSPIN_K_THERM) & BIT_LEN(4);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	return TRUE;
}

static bool cSPIN_GetAdcOut (cSPIN_Rsrc_T* pRsrc, u8 *param){
	*param = (u8)cSPIN_Get_Param(pRsrc, cSPIN_ADC_OUT);
	return isCmdErr(pRsrc);
}

static bool cSPIN_SetOCDTh (cSPIN_Rsrc_T* pRsrc, u8 param){
	u32 b32x0 = param & BIT_LEN(4);
	cSPIN_Set_Param(pRsrc, cSPIN_OCD_TH, b32x0);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	cSPIN_EromWrite(pRsrc, cSPIN_OCD_TH, b32x0);
	return TRUE;
}
static bool cSPIN_GetOCDTh (cSPIN_Rsrc_T* pRsrc, u8 *param){
	*param = (u8)cSPIN_Get_Param(pRsrc, cSPIN_OCD_TH);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	return TRUE;
}

static bool cSPIN_SetStallTh (cSPIN_Rsrc_T* pRsrc, u8 param){
	u32 b32x0 = param & BIT_LEN(7);
	cSPIN_Set_Param(pRsrc, cSPIN_STALL_TH, b32x0);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	cSPIN_EromWrite(pRsrc, cSPIN_STALL_TH, b32x0);
	return TRUE;
}
static bool cSPIN_GetStallTh (cSPIN_Rsrc_T* pRsrc, u8 *param){
	*param = (u8)cSPIN_Get_Param(pRsrc, cSPIN_STALL_TH);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	return TRUE;
}

static bool cSPIN_SetStepSel (cSPIN_Rsrc_T* pRsrc, u8 param){
	u32 b32x0 = param & BIT_LEN(3);
	u32 b32x1 = (u8)cSPIN_Get_Param(pRsrc, cSPIN_STEP_MODE) & (0xff ^ BIT_LEN(3));
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	b32x0 |= b32x1;
	cSPIN_Set_Param(pRsrc, cSPIN_STEP_MODE, b32x0);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	cSPIN_EromWrite(pRsrc, cSPIN_STEP_MODE, b32x0);
	return TRUE;
}
static bool cSPIN_GetStepSel (cSPIN_Rsrc_T* pRsrc, u8 *param){
	*param = (u8)cSPIN_Get_Param(pRsrc, cSPIN_STEP_MODE)&BIT_LEN(3);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	return TRUE;
}

static bool cSPIN_SetSyncSel (cSPIN_Rsrc_T* pRsrc, u8 param){
	u32 b32x0 = (param & BIT_LEN(3))<<4;
	u32 b32x1 = (u8)cSPIN_Get_Param(pRsrc, cSPIN_STEP_MODE);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	b32x1 &= ((BIT_LEN(3)<<4)^0xff);
	b32x0 |= b32x1;
	cSPIN_Set_Param(pRsrc, cSPIN_STEP_MODE, b32x0);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	cSPIN_EromWrite(pRsrc, cSPIN_STEP_MODE, b32x0);
	return TRUE;
}
static bool cSPIN_GetSyncSel (cSPIN_Rsrc_T* pRsrc, u8 *param){
	*param = ((u8)cSPIN_Get_Param(pRsrc, cSPIN_STEP_MODE)>>4) & BIT_LEN(3);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	return TRUE;
}

static bool cSPIN_SetSyncEn (cSPIN_Rsrc_T* pRsrc, u8 param){
	u32 b32x0;
	u32 b32x1 = (u8)cSPIN_Get_Param(pRsrc, cSPIN_STEP_MODE) & 0x7f;
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	b32x0 = b32x1;
	if(param > 0)	b32x0 |= 0x80;
	cSPIN_Set_Param(pRsrc, cSPIN_STEP_MODE, b32x0);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	cSPIN_EromWrite(pRsrc, cSPIN_STEP_MODE, b32x0);
	return TRUE;
}
static bool cSPIN_GetSyncEn (cSPIN_Rsrc_T* pRsrc, u8 *param){
	*param = ((u8)cSPIN_Get_Param(pRsrc, cSPIN_STEP_MODE)>>7);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	return TRUE;
}

#if(USING_L6480)
static bool cSPIN_SetGateTcc (cSPIN_Rsrc_T* pRsrc, u8 param){
//	u32 b32x0 = param & BIT_LEN(22);
//	cSPIN_Set_Param(pRsrc, cSPIN_ABS_POS, b32x0);
//	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
//	cSPIN_EromWrite(pRsrc, cSPIN_ABS_POS, b32x0);
//	return TRUE;

//	
//	
//	
	u16 reg = 0;
	reg = (u16)cSPIN_Get_Param(pRsrc, cSPIN_GATECFG1);
	if(isCmdErr(pRsrc)==CMD_FAIL)	return CMD_FAIL;
	reg &= (0xffff<<5);	reg |= param;
	cSPIN_Set_Param(pRsrc, cSPIN_GATECFG1, reg);
	return isCmdErr(pRsrc);
}
static bool cSPIN_GetGateTcc (cSPIN_Rsrc_T* pRsrc, u8 *param){
	*param = (u16)cSPIN_Get_Param(pRsrc, cSPIN_GATECFG1)&0x1f;
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	return TRUE;
}

static bool cSPIN_SetGateCurrent (cSPIN_Rsrc_T* pRsrc, u8 param){
//	u32 b32x0 = param & BIT_LEN(22);
//	cSPIN_Set_Param(pRsrc, cSPIN_ABS_POS, b32x0);
//	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
//	cSPIN_EromWrite(pRsrc, cSPIN_ABS_POS, b32x0);
//	return TRUE;
//	
//	




	u16 reg = 0;
	reg = (u16)cSPIN_Get_Param(pRsrc, cSPIN_GATECFG1);
	if(isCmdErr(pRsrc)==CMD_FAIL)	return CMD_FAIL;
	reg &= 0xff1f;	reg |= (param&0x07)<<5;
	cSPIN_Set_Param(pRsrc, cSPIN_GATECFG1, reg);
	return isCmdErr(pRsrc);
}
static bool cSPIN_GetGateCurrent (cSPIN_Rsrc_T* pRsrc, u8 *param){
	
	*param = (cSPIN_Get_Param(pRsrc, cSPIN_GATECFG1)>>5)&0x07;
	
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	return TRUE;
}

static bool cSPIN_SetGateTBoost (cSPIN_Rsrc_T* pRsrc, u8 param){
//	u32 b32x0 = param & BIT_LEN(22);
//	cSPIN_Set_Param(pRsrc, cSPIN_ABS_POS, b32x0);
//	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
//	cSPIN_EromWrite(pRsrc, cSPIN_ABS_POS, b32x0);
//	return TRUE;
//	
//	



	u16 reg = 0;
	reg = (u16)cSPIN_Get_Param(pRsrc, cSPIN_GATECFG1);
	if(isCmdErr(pRsrc)==CMD_FAIL)	return CMD_FAIL;
	reg &= (0xffff^(0x07<<8));	reg |= (u16)(param&0x07)<<8;
	cSPIN_Set_Param(pRsrc, cSPIN_GATECFG1, reg);
	return isCmdErr(pRsrc);
}
static bool cSPIN_GetGateTBoost (cSPIN_Rsrc_T* pRsrc, u8 *param){
	*param = (u8)(cSPIN_Get_Param(pRsrc, cSPIN_GATECFG1)>>8)&0x07;
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	return TRUE;
}

static bool cSPIN_SetGateTDeadTime (cSPIN_Rsrc_T* pRsrc, u8 param){
//	u32 b32x0 = param & BIT_LEN(22);
//	cSPIN_Set_Param(pRsrc, cSPIN_ABS_POS, b32x0);
//	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
//	cSPIN_EromWrite(pRsrc, cSPIN_ABS_POS, b32x0);
//	return TRUE;
//	
//	




	u8 reg = 0;
	reg = (u8)cSPIN_Get_Param(pRsrc, cSPIN_GATECFG2);
	if(isCmdErr(pRsrc)==CMD_FAIL)	return CMD_FAIL;
	reg &= 0xff<<5;	reg |= (param&0x1f);
	cSPIN_Set_Param(pRsrc, cSPIN_GATECFG2, reg);
	return isCmdErr(pRsrc);
}
static bool cSPIN_GetGateTDeadTime (cSPIN_Rsrc_T* pRsrc, u8 *param){
	*param = (u8)cSPIN_Get_Param(pRsrc, cSPIN_GATECFG2)&0x1f;
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	return TRUE;
}

static bool cSPIN_SetGateTBlank (cSPIN_Rsrc_T* pRsrc, u8 param){
//	u32 b32x0 = param & BIT_LEN(22);
//	cSPIN_Set_Param(pRsrc, cSPIN_ABS_POS, b32x0);
//	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
//	cSPIN_EromWrite(pRsrc, cSPIN_ABS_POS, b32x0);
//	return TRUE;
//	
//	

	u8 reg = 0;
	reg = (u8)cSPIN_Get_Param(pRsrc, cSPIN_GATECFG2);
	if(isCmdErr(pRsrc)==CMD_FAIL)	return CMD_FAIL;
	reg &= 0xff>>3;	reg |= (param&0x07)<<5;
	cSPIN_Set_Param(pRsrc, cSPIN_GATECFG2, reg);
	return isCmdErr(pRsrc);
}
static bool cSPIN_GetGateTBlank (cSPIN_Rsrc_T* pRsrc, u8 *param){
	*param = (u8)(cSPIN_Get_Param(pRsrc, cSPIN_GATECFG2)&0x07)>>5;
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	return TRUE;
}
#endif		//(USING_L6480)
/************************
	*config
************************/
static bool cSPIN_SetConfig (cSPIN_Rsrc_T* pRsrc, u16 param){
	u32 b32x0 = param & BIT_LEN(16);
	cSPIN_Set_Param(pRsrc, cSPIN_CONFIG, b32x0);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	cSPIN_EromWrite(pRsrc, cSPIN_CONFIG, b32x0);
	return TRUE;
}

static bool cSPIN_GetConfig (cSPIN_Rsrc_T* pRsrc, u16 *param){
	*param = (u16)cSPIN_Get_Param(pRsrc, cSPIN_CONFIG);
	if(isCmdErr(pRsrc)==TRUE)	return (FALSE);
	return TRUE;
}


/**
  * @brief  Issues cSPIN Run command.
  * @param  direction Movement direction (FWD, REV)
  * @param  speed over 3 bytes
  * @retval None
  */
static void cSPIN_Run(cSPIN_Rsrc_T* pRsrc, cSPIN_Direction_T direction, u32 speed)
{
	u8 txbuf[4];
	u32 spd = speed & 0x00ffffff;
	
	txbuf[0] = (u8)cSPIN_RUN | (u8)direction;
	txbuf[3] = (u8)(spd);	spd >>= 8;
	txbuf[2] = (u8)(spd);	spd >>= 8;
	txbuf[1] = (u8)(spd);
	cSPIN_TransmitReceiveX4(pRsrc, txbuf, NULL);	
	
//	/* Send RUN operation code to cSPIN */
//	cSPIN_Write_Byte(pRsrc, (u8)cSPIN_RUN | (u8)direction);
//	/* Send speed - byte 2 data cSPIN */
//	cSPIN_Write_Byte(pRsrc, (u8)(speed >> 16));
//	/* Send speed - byte 1 data cSPIN */
//	cSPIN_Write_Byte(pRsrc, (u8)(speed >> 8));
//	/* Send speed - byte 0 data cSPIN */
//	cSPIN_Write_Byte(pRsrc, (u8)(speed));
}

static void cSPIN_RunTimeout		(cSPIN_Rsrc_T* pRsrc, cSPIN_Direction_T direction, uint32_t speed, u32 timeout){
	pRsrc->runTmr = timeout;
	cSPIN_Run(pRsrc, direction, speed);
}

/**
  * @brief  Issues cSPIN Step Clock command.
  * @param  direction Movement direction (FWD, REV)
  * @retval None
  */
static void cSPIN_Step_Clock(cSPIN_Rsrc_T* pRsrc, cSPIN_Direction_T direction)
{
	/* Send StepClock operation code to cSPIN */
//	cSPIN_Write_Byte(pRsrc, (u8)cSPIN_STEP_CLOCK | (u8)direction);
	u8 txbuf[4] = {0};
	txbuf[0] = (u8)cSPIN_STEP_CLOCK | (u8)direction;
	cSPIN_TransmitReceiveX4(pRsrc, txbuf, NULL);
}

/**
  * @brief  Issues cSPIN Move command.
  * @param  direction Movement direction
  * @param  n_step number of steps
  * @retval None
  */
static void cSPIN_Move(cSPIN_Rsrc_T* pRsrc, cSPIN_Direction_T direction, u32 n_step)
{
	u8 txbuf[4];
	u32 x = n_step & 0x00ffffff;
	
	txbuf[0] = (u8)cSPIN_MOVE | (u8)direction;
	txbuf[3] = (u8)(x);	x >>= 8;
	txbuf[2] = (u8)(x);	x >>= 8;
	txbuf[1] = (u8)(x);
	cSPIN_TransmitReceiveX4(pRsrc, txbuf, NULL);	

//	/* Send Move operation code to cSPIN */
//	cSPIN_Write_Byte(pRsrc, (u8)cSPIN_MOVE | (u8)direction);
//	/* Send n_step - byte 2 data cSPIN */
//	cSPIN_Write_Byte(pRsrc, (u8)(n_step >> 16));
//	/* Send n_step - byte 1 data cSPIN */
//	cSPIN_Write_Byte(pRsrc, (u8)(n_step >> 8));
//	/* Send n_step - byte 0 data cSPIN */
//	cSPIN_Write_Byte(pRsrc, (u8)(n_step));
}

/**
  * @brief  Issues cSPIN Go To command.
  * @param  abs_pos absolute position where requested to move
  * @retval None
  */
static void cSPIN_Go_To(cSPIN_Rsrc_T* pRsrc, u32 abs_pos)
{
	u8 txbuf[4];
	u32 x = abs_pos & 0x00ffffff;
	
	txbuf[0] = cSPIN_GO_TO;
	txbuf[3] = (u8)(x);	x >>= 8;
	txbuf[2] = (u8)(x);	x >>= 8;
	txbuf[1] = (u8)(x);
	cSPIN_TransmitReceiveX4(pRsrc, txbuf, NULL);	
	
//	/* Send GoTo operation code to cSPIN */
//	cSPIN_Write_Byte(pRsrc, cSPIN_GO_TO);
//	/* Send absolute position parameter - byte 2 data to cSPIN */
//	cSPIN_Write_Byte(pRsrc, (u8)(abs_pos >> 16));
//	/* Send absolute position parameter - byte 1 data to cSPIN */
//	cSPIN_Write_Byte(pRsrc, (u8)(abs_pos >> 8));
//	/* Send absolute position parameter - byte 0 data to cSPIN */
//	cSPIN_Write_Byte(pRsrc, (u8)(abs_pos));
}

/**
  * @brief  Issues cSPIN Go To Dir command.
  * @param  direction movement direction
  * @param  abs_pos absolute position where requested to move
  * @retval None
  */
static void cSPIN_Go_To_Dir(cSPIN_Rsrc_T* pRsrc, cSPIN_Direction_T direction, u32 abs_pos)
{
	u8 txbuf[4];
	u32 x = abs_pos & 0x00ffffff;
	
	txbuf[0] = (u8)cSPIN_GO_TO_DIR | (u8)direction;
	txbuf[3] = (u8)(x);	x >>= 8;
	txbuf[2] = (u8)(x);	x >>= 8;
	txbuf[1] = (u8)(x);
	cSPIN_TransmitReceiveX4(pRsrc, txbuf, NULL);	
	
//	/* Send GoTo_DIR operation code to cSPIN */
//	cSPIN_Write_Byte(pRsrc, (u8)cSPIN_GO_TO_DIR | (u8)direction);
//	/* Send absolute position parameter - byte 2 data to cSPIN */
//	cSPIN_Write_Byte(pRsrc, (u8)(abs_pos >> 16));
//	/* Send absolute position parameter - byte 1 data to cSPIN */
//	cSPIN_Write_Byte(pRsrc, (u8)(abs_pos >> 8));
//	/* Send absolute position parameter - byte 0 data to cSPIN */
//	cSPIN_Write_Byte(pRsrc, (u8)(abs_pos));
}

/**
  * @brief  Issues cSPIN Go Until command.
  * @param  action
  * @param  direction movement direction
  * @param  speed
  * @retval None
  */
static void cSPIN_Go_Until(cSPIN_Rsrc_T* pRsrc, cSPIN_Action_T action, cSPIN_Direction_T direction, u32 speed)
{
	u8 txbuf[4];
	u32 x = speed & 0x00ffffff;
	
	txbuf[0] = (u8)cSPIN_GO_UNTIL | (u8)direction;
	txbuf[3] = (u8)(x);	x >>= 8;
	txbuf[2] = (u8)(x);	x >>= 8;
	txbuf[1] = (u8)(x);
	cSPIN_TransmitReceiveX4(pRsrc, txbuf, NULL);	
	
//	/* Send GoUntil operation code to cSPIN */
//	cSPIN_Write_Byte(pRsrc, (u8)cSPIN_GO_UNTIL | (u8)action | (u8)direction);
//	/* Send speed parameter - byte 2 data to cSPIN */
//	cSPIN_Write_Byte(pRsrc, (u8)(speed >> 16));
//	/* Send speed parameter - byte 1 data to cSPIN */
//	cSPIN_Write_Byte(pRsrc, (u8)(speed >> 8));
//	/* Send speed parameter - byte 0 data to cSPIN */
//	cSPIN_Write_Byte(pRsrc, (u8)(speed));
}

/**
  * @brief  Issues cSPIN Release SW command.
  * @param  action
  * @param  direction movement direction
  * @retval None
  */
static void cSPIN_Release_SW(cSPIN_Rsrc_T* pRsrc, cSPIN_Action_T action, cSPIN_Direction_T direction)
{
	u8 txbuf[4] = {0};
	txbuf[0] = (u8)cSPIN_RELEASE_SW | (u8)action | (u8)direction;
	cSPIN_TransmitReceiveX4(pRsrc, txbuf, NULL);
	
//	/* Send ReleaseSW operation code to cSPIN */
//	cSPIN_Write_Byte(pRsrc, (u8)cSPIN_RELEASE_SW | (u8)action | (u8)direction);
}

/**
  * @brief  Issues cSPIN Go Home command. (Shorted path to zero position)
  * @param  None
  * @retval None
  */
static void cSPIN_Go_Home(cSPIN_Rsrc_T* pRsrc)
{
	u8 txbuf[4] = {0};
	txbuf[0] = cSPIN_GO_HOME;
	cSPIN_TransmitReceiveX4(pRsrc, txbuf, NULL);
//	/* Send GoHome operation code to cSPIN */
//	cSPIN_Write_Byte(pRsrc, cSPIN_GO_HOME);
}

/**
  * @brief  Issues cSPIN Go Mark command.
  * @param  None
  * @retval None
  */
static void cSPIN_Go_Mark(cSPIN_Rsrc_T* pRsrc)
{
	u8 txbuf[4] = {0};
	txbuf[0] = cSPIN_GO_MARK;
	cSPIN_TransmitReceiveX4(pRsrc, txbuf, NULL);
//	/* Send GoMark operation code to cSPIN */
//	cSPIN_Write_Byte(pRsrc, cSPIN_GO_MARK);
}

/**
  * @brief  Issues cSPIN Reset Pos command.
  * @param  None
  * @retval None
  */
static void cSPIN_Reset_Pos(cSPIN_Rsrc_T* pRsrc)
{
	u8 txbuf[4] = {0};
	txbuf[0] = cSPIN_RESET_POS;
	cSPIN_TransmitReceiveX4(pRsrc, txbuf, NULL);
//	/* Send ResetPos operation code to cSPIN */
//	cSPIN_Write_Byte(pRsrc, cSPIN_RESET_POS);
}

/**
  * @brief  Issues cSPIN Reset Device command.
  * @param  None
  * @retval None
  */
static void cSPIN_Default_Device(cSPIN_Rsrc_T* pRsrc){
	u8 buf[4] = {0};

	/* Resets and puts cSPIN into standby mode */
	buf[0] = cSPIN_RESET_DEVICE;
	cSPIN_TransmitReceiveX4(pRsrc, buf, NULL);

	cSPIN_SetOCDTh(pRsrc, 5);
	cSPIN_SetStallTh(pRsrc, 64);
	cSPIN_SetAcc(pRsrc, 150);
	cSPIN_SetDec(pRsrc, 150);
	cSPIN_SetMaxSpd(pRsrc, 50);
	cSPIN_SetFSSpd(pRsrc,1023);
	
	cSPIN_SetKValHold(pRsrc, 6);
	cSPIN_SetKValRun(pRsrc, 10);
	cSPIN_SetKValAcc(pRsrc, 10);
	cSPIN_SetKValDec(pRsrc, 10);
	
	cSPIN_SetIntSpd(pRsrc, 1100);
	cSPIN_SetStartSlp(pRsrc, 15);
	cSPIN_SetFnSlpAcc(pRsrc, 25);
	cSPIN_SetFnSlpDec(pRsrc, 25);
}

static void cSPIN_Reset_Device(cSPIN_Rsrc_T* pRsrc)
{
	u8 buf[4] = {0};
	u32 tmp32;

	/* Resets and puts cSPIN into standby mode */
	buf[0] = cSPIN_RESET_DEVICE;
	cSPIN_TransmitReceiveX4(pRsrc, buf, NULL);

	//reset all reg val using eeprom val
	//reset cSPIN_ABS_POS	22b
	if(cSPIN_EromRead(pRsrc, cSPIN_ABS_POS, &tmp32) == FALSE){
		cSPIN_EromWrite(pRsrc, cSPIN_ABS_POS, cSPIN_Get_Param(pRsrc, cSPIN_ABS_POS)&BIT_LEN(22));
	}
	else	cSPIN_Set_Param(pRsrc, cSPIN_ABS_POS, tmp32&BIT_LEN(22));

	//reset cSPIN_EL_POS	9b
//	if(cSPIN_EromRead(pRsrc, cSPIN_EL_POS, &tmp32) == FALSE){
//		cSPIN_EromWrite(pRsrc, cSPIN_EL_POS, cSPIN_Get_Param(pRsrc, cSPIN_EL_POS)&BIT_LEN(9));
//	}
//	else	cSPIN_Set_Param(pRsrc, cSPIN_EL_POS, tmp32&BIT_LEN(9));

	//reset cSPIN_MARK	22b
	if(cSPIN_EromRead(pRsrc, cSPIN_MARK, &tmp32) == FALSE){		
		cSPIN_EromWrite(pRsrc, cSPIN_MARK, cSPIN_Get_Param(pRsrc, cSPIN_MARK)&BIT_LEN(22));
	}	
	else	cSPIN_Set_Param(pRsrc, cSPIN_MARK, tmp32&BIT_LEN(22));
	
	//reset cSPIN_ACC
	if(cSPIN_EromRead(pRsrc, cSPIN_ACC, &tmp32) == FALSE){		
		cSPIN_EromWrite(pRsrc, cSPIN_ACC, cSPIN_Get_Param(pRsrc, cSPIN_ACC)&BIT_LEN(12));
	}
	else	cSPIN_Set_Param(pRsrc, cSPIN_ACC, tmp32&BIT_LEN(12));
	
	//reset cSPIN_DEC
	if(cSPIN_EromRead(pRsrc, cSPIN_DEC, &tmp32) == FALSE){		
		cSPIN_EromWrite(pRsrc, cSPIN_DEC, cSPIN_Get_Param(pRsrc, cSPIN_DEC)&BIT_LEN(12));
	}
	else	cSPIN_Set_Param(pRsrc, cSPIN_DEC, tmp32&BIT_LEN(12));
	
	//reset cSPIN_MAX_SPEED
	if(cSPIN_EromRead(pRsrc, cSPIN_MAX_SPEED, &tmp32) == FALSE){		
		cSPIN_EromWrite(pRsrc, cSPIN_MAX_SPEED, cSPIN_Get_Param(pRsrc, cSPIN_MAX_SPEED)&BIT_LEN(10));
	}
	else	cSPIN_Set_Param(pRsrc, cSPIN_MAX_SPEED, tmp32&BIT_LEN(10));

	//reset cSPIN_MIN_SPEED
	if(cSPIN_EromRead(pRsrc, cSPIN_MIN_SPEED, &tmp32) == FALSE){
		cSPIN_EromWrite(pRsrc, cSPIN_MIN_SPEED, cSPIN_Get_Param(pRsrc, cSPIN_MIN_SPEED)&BIT_LEN(13));
	}
	else	cSPIN_Set_Param(pRsrc, cSPIN_MIN_SPEED, tmp32&BIT_LEN(13));

	//reset cSPIN_FS_SPD
	if(cSPIN_EromRead(pRsrc, cSPIN_FS_SPD, &tmp32) == FALSE){		
		cSPIN_EromWrite(pRsrc, cSPIN_FS_SPD, cSPIN_Get_Param(pRsrc, cSPIN_FS_SPD)&BIT_LEN(10));
	}
	else	cSPIN_Set_Param(pRsrc, cSPIN_FS_SPD, tmp32&BIT_LEN(10));

	//reset cSPIN_KVAL_HOLD
	if(cSPIN_EromRead(pRsrc, cSPIN_KVAL_HOLD, &tmp32) == FALSE){
		cSPIN_EromWrite(pRsrc, cSPIN_KVAL_HOLD, 5);
	}
	else	cSPIN_Set_Param(pRsrc, cSPIN_KVAL_HOLD, (u8)tmp32);
	//reset cSPIN_KVAL_RUN
	if(cSPIN_EromRead(pRsrc, cSPIN_KVAL_RUN, &tmp32) == FALSE){
		cSPIN_EromWrite(pRsrc, cSPIN_KVAL_RUN, 5);	
	}
	else	cSPIN_Set_Param(pRsrc, cSPIN_KVAL_RUN, (u8)tmp32);
	//reset cSPIN_KVAL_ACC
	if(cSPIN_EromRead(pRsrc, cSPIN_KVAL_ACC, &tmp32) == FALSE){
		cSPIN_EromWrite(pRsrc, cSPIN_KVAL_ACC, 5);	
	}
	else	cSPIN_Set_Param(pRsrc, cSPIN_KVAL_ACC, (u8)tmp32);
	//reset cSPIN_KVAL_DEC
	if(cSPIN_EromRead(pRsrc, cSPIN_KVAL_DEC, &tmp32) == FALSE){
		cSPIN_EromWrite(pRsrc, cSPIN_KVAL_DEC, 5);
	}
	else	cSPIN_Set_Param(pRsrc, cSPIN_KVAL_DEC, (u8)tmp32);
	
	//reset cSPIN_INT_SPD
	if(cSPIN_EromRead(pRsrc, cSPIN_INT_SPD, &tmp32) == FALSE){		
		cSPIN_EromWrite(pRsrc, cSPIN_INT_SPD, cSPIN_Get_Param(pRsrc, cSPIN_INT_SPD)&BIT_LEN(14));
	}
	else	cSPIN_Set_Param(pRsrc, cSPIN_INT_SPD, tmp32&BIT_LEN(14));

	//reset cSPIN_ST_SLP
	if(cSPIN_EromRead(pRsrc, cSPIN_ST_SLP, &tmp32) == FALSE){		
		cSPIN_EromWrite(pRsrc, cSPIN_ST_SLP, cSPIN_Get_Param(pRsrc, cSPIN_ST_SLP)&BIT_LEN(8));
	}
	else	cSPIN_Set_Param(pRsrc, cSPIN_ST_SLP, (u8)tmp32);

	//reset cSPIN_FN_SLP_ACC
	if(cSPIN_EromRead(pRsrc, cSPIN_FN_SLP_ACC, &tmp32) == FALSE){		
		cSPIN_EromWrite(pRsrc, cSPIN_FN_SLP_ACC, cSPIN_Get_Param(pRsrc, cSPIN_FN_SLP_ACC)&BIT_LEN(8));
	}
	else	cSPIN_Set_Param(pRsrc, cSPIN_FN_SLP_ACC, (u8)tmp32);

	//reset cSPIN_FN_SLP_DEC
	if(cSPIN_EromRead(pRsrc, cSPIN_FN_SLP_DEC, &tmp32) == FALSE){		
		cSPIN_EromWrite(pRsrc, cSPIN_FN_SLP_DEC, cSPIN_Get_Param(pRsrc, cSPIN_FN_SLP_DEC)&BIT_LEN(8));
	}
	else	cSPIN_Set_Param(pRsrc, cSPIN_FN_SLP_DEC, (u8)tmp32);

	//reset cSPIN_K_THERM
	if(cSPIN_EromRead(pRsrc, cSPIN_K_THERM, &tmp32) == FALSE){		
		cSPIN_EromWrite(pRsrc, cSPIN_K_THERM, cSPIN_Get_Param(pRsrc, cSPIN_K_THERM)&BIT_LEN(4));
	}
	else	cSPIN_Set_Param(pRsrc, cSPIN_K_THERM, tmp32&BIT_LEN(4));

#if(USING_L6470)
	//reset cSPIN_OCD_TH
	if(cSPIN_EromRead(pRsrc, cSPIN_OCD_TH, &tmp32) == FALSE){
		cSPIN_EromWrite(pRsrc, cSPIN_OCD_TH, cSPIN_Get_Param(pRsrc, cSPIN_OCD_TH)&BIT_LEN(4));
	}
	else	cSPIN_Set_Param(pRsrc, cSPIN_OCD_TH, tmp32&BIT_LEN(4));
	//reset cSPIN_STALL_TH
	if(cSPIN_EromRead(pRsrc, cSPIN_STALL_TH, &tmp32) == FALSE){
		cSPIN_EromWrite(pRsrc, cSPIN_STALL_TH, cSPIN_Get_Param(pRsrc, cSPIN_STALL_TH)&BIT_LEN(7));
	}
	else	cSPIN_Set_Param(pRsrc, cSPIN_STALL_TH, tmp32&BIT_LEN(7));
#endif	

  

	
}

/**
  * @brief  Issues cSPIN Soft Stop command.
  * @param  None
  * @retval None
  */
static void cSPIN_Soft_Stop(cSPIN_Rsrc_T* pRsrc)
{
	u8 txbuf[4] = {0};
	txbuf[0] = cSPIN_SOFT_STOP;
	cSPIN_TransmitReceiveX4(pRsrc, txbuf, NULL);
//	/* Send SoftStop operation code to cSPIN */
//	cSPIN_Write_Byte(pRsrc, cSPIN_SOFT_STOP);
}

/**
  * @brief  Issues cSPIN Hard Stop command.
  * @param  None
  * @retval None
  */
static void cSPIN_Hard_Stop(cSPIN_Rsrc_T* pRsrc)
{
	u8 txbuf[4] = {0};
	txbuf[0] = cSPIN_HARD_STOP;
	cSPIN_TransmitReceiveX4(pRsrc, txbuf, NULL);
//	/* Send HardStop operation code to cSPIN */
//	cSPIN_Write_Byte(pRsrc, cSPIN_HARD_STOP);
}

/**
  * @brief  Issues cSPIN Soft HiZ command.
  * @param  None
  * @retval None
  */
static void cSPIN_Soft_HiZ(cSPIN_Rsrc_T* pRsrc)
{
	u8 txbuf[4] = {0};
	txbuf[0] = cSPIN_SOFT_HIZ;
	cSPIN_TransmitReceiveX4(pRsrc, txbuf, NULL);
//	/* Send SoftHiZ operation code to cSPIN */
//	cSPIN_Write_Byte(pRsrc, cSPIN_SOFT_HIZ);
}

/**
  * @brief  Issues cSPIN Hard HiZ command.
  * @param  None
  * @retval None
  */
static void cSPIN_Hard_HiZ(cSPIN_Rsrc_T* pRsrc)
{
	u8 txbuf[4] = {0};
	txbuf[0] = cSPIN_HARD_HIZ;
	cSPIN_TransmitReceiveX4(pRsrc, txbuf, NULL);
//	/* Send HardHiZ operation code to cSPIN */
//	cSPIN_Write_Byte(pRsrc, cSPIN_HARD_HIZ);
}

/**
  * @brief  Issues cSPIN Get Status command.
  * @param  None
  * @retval Status Register content
  */
static u16 cSPIN_Get_Status(cSPIN_Rsrc_T* pRsrc)
{
	u8 txbuf[4] = {0}, rxbuf[4]={0};
	u16 temp = 0;

	txbuf[0] = cSPIN_GET_STATUS;
	cSPIN_TransmitReceiveX4(pRsrc, txbuf, rxbuf);		
	temp = rxbuf[1];	temp <<= 8;
	temp |= rxbuf[2];
	return temp;
	
//	u16 temp = 0;
//	u16 rx = 0;

//	/* Send GetStatus operation code to cSPIN */
//	cSPIN_Write_Byte(pRsrc, cSPIN_GET_STATUS);
//	/* Send zero byte / receive MSByte from cSPIN */
//	temp = cSPIN_Write_Byte(pRsrc, (u8)(0x00));
//	temp = temp << 8;
//	rx |= temp;
//	/* Send zero byte / receive LSByte from cSPIN */
//	temp = cSPIN_Write_Byte(pRsrc, (u8)(0x00));
//	rx |= temp;
//	return rx;
}

/**
  * @brief  Checks if the cSPIN is Busy by SPI - Busy flag bit in Status Register.
  * @param  None
  * @retval one if chip is busy, otherwise zero
  */
static u8 cSPIN_Busy_SW(cSPIN_Rsrc_T* pRsrc)
{
	if(!(cSPIN_Get_Status(pRsrc) & cSPIN_STATUS_BUSY)) return 0x01;
	else return 0x00;
}

/**
  * @brief  Checks cSPIN Flag signal.
  * @param  None
  * @retval one if Flag signal is active, otherwise zero
  */
static u8 cSPIN_Flag(cSPIN_Rsrc_T* pRsrc)
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
//static void cSPIN_Delay(__IO u32 nCount)
//{
//  for(; nCount!= 0;nCount--);
//}

///**
//  * @brief  Transmits/Receives one byte to/from cSPIN over SPI.
//  * @param  byte Transmited byte
//  * @retval Received byte
//  */
//u8 cSPIN_Write_Byte(cSPIN_Rsrc_T* pRsrc, u8 byte)
//{
//	u8 rtn=0;
//	/* nSS signal activation - low */
//	HAL_GPIO_WritePin(pRsrc->CS.GPIOx, pRsrc->CS.GPIO_Pin, GPIO_PIN_RESET);
//	/* SPI byte send */
//	HAL_SPI_TransmitReceive(pRsrc->SPI_HANDLE, &byte, &rtn, 1, HAL_MAX_DELAY);
//	/* nSS signal deactivation - high */
//	HAL_GPIO_WritePin(pRsrc->CS.GPIOx, pRsrc->CS.GPIO_Pin,GPIO_PIN_SET);
//	return rtn;
//}

/**
  * @brief  Transmits/Receives one byte to/from cSPIN over SPI.
  * @param  byte Transmited byte
  * @retval Received byte
  */
static bool cSPIN_TransmitReceiveX4(cSPIN_Rsrc_T* pRsrc, u8* txbuf, u8* rxbuf)
{
	return (cSPIN_TransmitReceive(pRsrc, txbuf, rxbuf, 4));
}
static bool cSPIN_TransmitReceive(cSPIN_Rsrc_T* pRsrc, u8* txbuf, u8* rxbuf, u8 len)
{
	u8 i,j = 0;
	if(rxbuf == NULL){	//send only
		for(i=0;i<len;i++){
			HAL_GPIO_WritePin(pRsrc->CS.GPIOx, pRsrc->CS.GPIO_Pin, GPIO_PIN_RESET);	
			HAL_SPI_Transmit(pRsrc->SPI_HANDLE, &txbuf[i], 1, HAL_MAX_DELAY);
			HAL_GPIO_WritePin(pRsrc->CS.GPIOx, pRsrc->CS.GPIO_Pin,GPIO_PIN_SET);	
		}
	}
	else{
		for(i=0;i<len;i++){
			HAL_GPIO_WritePin(pRsrc->CS.GPIOx, pRsrc->CS.GPIO_Pin, GPIO_PIN_RESET);
			HAL_SPI_TransmitReceive(pRsrc->SPI_HANDLE, &txbuf[i], &rxbuf[i], 1, HAL_MAX_DELAY);
			HAL_GPIO_WritePin(pRsrc->CS.GPIOx, pRsrc->CS.GPIO_Pin,GPIO_PIN_SET);
		}
	}	
	//send 4 NOP
	for(i=0;i<4;i++){
		HAL_GPIO_WritePin(pRsrc->CS.GPIOx, pRsrc->CS.GPIO_Pin, GPIO_PIN_RESET);
		HAL_SPI_Transmit(pRsrc->SPI_HANDLE, &j, 1, HAL_MAX_DELAY);
		HAL_GPIO_WritePin(pRsrc->CS.GPIOx, pRsrc->CS.GPIO_Pin,GPIO_PIN_SET);
	}
	return TRUE;
}

/**
  * @brief  Resets CSPIN and puts it into standby mode
  * @param  None
  * @retval None
  */
//static void cSPIN_Reset_And_Standby(cSPIN_Rsrc_T* pRsrc)
//{
//	HAL_GPIO_WritePin(pRsrc->nStby_nReset.GPIOx, pRsrc->nStby_nReset.GPIO_Pin,GPIO_PIN_RESET);
//  cSPIN_Delay(10000);
//  HAL_GPIO_WritePin(pRsrc->nStby_nReset.GPIOx, pRsrc->nStby_nReset.GPIO_Pin,GPIO_PIN_SET);
//}

/**
  * @brief  Fills-in cSPIN configuration structure with default values.
  * @param  cSPIN_RegsStruct structure address (pointer to struct)
  * @retval None
  */
//static void cSPIN_Regs_Struct_Reset(cSPIN_Rsrc_T* pRsrc)
//{
////     //for hsk big stepper
////	pRsrc->reg.ABS_POS		= 0;
////	pRsrc->reg.EL_POS 		= 0;
////	pRsrc->reg.MARK 		= 0;
////	pRsrc->reg.ACC 		= 50;
////	pRsrc->reg.DEC 		= 50;
////	pRsrc->reg.MAX_SPEED 	= 20;     //0..1023
////	pRsrc->reg.MIN_SPEED 	= 0;
////	pRsrc->reg.FS_SPD 		= 0x3ff;
////#if defined(motor)
////	pRsrc->reg.KVAL_HOLD 	= 30;
////	pRsrc->reg.KVAL_RUN 	= 50;
////	pRsrc->reg.KVAL_ACC 	= 50;
////	pRsrc->reg.KVAL_DEC 	= 50;
////	pRsrc->reg.INT_SPD 		= 5;      //0..16384
////	pRsrc->reg.ST_SLP 		= 5;      //0..255
////	pRsrc->reg.FN_SLP_ACC = 5;         //0..255
////	pRsrc->reg.FN_SLP_DEC = 5;         //0..255
////	pRsrc->reg.K_THERM 		= 0;
////	pRsrc->reg.STALL_TH 	= 0x01;
////#endif /* defined(motor) */
////#if defined(L6482)
//////	pRsrc->reg.TVAL_HOLD 	= 0x29;
//////	pRsrc->reg.TVAL_RUN 	= 0x29;
//////	pRsrc->reg.TVAL_ACC 	= 0x29;
//////	pRsrc->reg.TVAL_DEC 	= 0x29;
//////	pRsrc->reg.T_FAST 		= 0x19;
//////	pRsrc->reg.TON_MIN 		= 0x19;
//////	pRsrc->reg.TOFF_MIN 	= 0x19;
////#endif /* defined(L6482) */
////	pRsrc->reg.OCD_TH 		= 0x01;
////	pRsrc->reg.STEP_MODE 	= 0x7;
////	pRsrc->reg.ALARM_EN 	= 0xFF;
////	pRsrc->reg.GATECFG1 	= 0;
////	pRsrc->reg.GATECFG2 	= 0;
////	pRsrc->reg.CONFIG 		= 0x2C88;
//     
//     
//     
//   //for HSK little stepper
//	pRsrc->reg.ABS_POS		= 0;
//	pRsrc->reg.EL_POS 		= 0;
//	pRsrc->reg.MARK 		= 0;
//	pRsrc->reg.ACC 		= 50;
//	pRsrc->reg.DEC 		= 50;
//	pRsrc->reg.MAX_SPEED 	= 10;     //0..1023
//	pRsrc->reg.MIN_SPEED 	= 0;
//	pRsrc->reg.FS_SPD 		= 0x3ff;
//#if defined(motor)
//	pRsrc->reg.KVAL_HOLD 	= 10;
//	pRsrc->reg.KVAL_RUN 	= 20;
//	pRsrc->reg.KVAL_ACC 	= 20;
//	pRsrc->reg.KVAL_DEC 	= 20;
//	pRsrc->reg.INT_SPD 		= 2;      //0..16384
//	pRsrc->reg.ST_SLP 		= 5;      //0..255
//	pRsrc->reg.FN_SLP_ACC = 10;         //0..255
//	pRsrc->reg.FN_SLP_DEC = 10;         //0..255
//	pRsrc->reg.K_THERM 		= 0;
//	pRsrc->reg.STALL_TH 	= 0x01;
//#endif /* defined(motor) */
//#if defined(L6482)
////	pRsrc->reg.TVAL_HOLD 	= 0x29;
////	pRsrc->reg.TVAL_RUN 	= 0x29;
////	pRsrc->reg.TVAL_ACC 	= 0x29;
////	pRsrc->reg.TVAL_DEC 	= 0x29;
////	pRsrc->reg.T_FAST 		= 0x19;
////	pRsrc->reg.TON_MIN 		= 0x19;
////	pRsrc->reg.TOFF_MIN 	= 0x19;
//#endif /* defined(L6482) */
//	pRsrc->reg.OCD_TH 		= 0x01;
//	pRsrc->reg.STEP_MODE 	= 0x7;
//	pRsrc->reg.ALARM_EN 	= 0xFF;
//	pRsrc->reg.GATECFG1 	= 0;
//	pRsrc->reg.GATECFG2 	= 0;
//	//pRsrc->reg.CONFIG 		= 0x2C88;

//}
// 


/**
  * @brief  Configures cSPIN internal registers with values in the config structure.
  * @param  cSPIN_RegsStruct Configuration structure address (pointer to configuration structure)
  * @retval None
  */
//static void cSPIN_Registers_SetAll(cSPIN_Rsrc_T* pRsrc)
//{
//	cSPIN_Set_Param(pRsrc, cSPIN_ABS_POS, pRsrc->reg.ABS_POS);
//	cSPIN_Set_Param(pRsrc, cSPIN_EL_POS, pRsrc->reg.EL_POS);
//	cSPIN_Set_Param(pRsrc, cSPIN_MARK, pRsrc->reg.MARK);
//	cSPIN_Set_Param(pRsrc, cSPIN_ACC, pRsrc->reg.ACC);
//	cSPIN_Set_Param(pRsrc, cSPIN_DEC, pRsrc->reg.DEC);
//	cSPIN_Set_Param(pRsrc, cSPIN_MAX_SPEED, pRsrc->reg.MAX_SPEED);
//	cSPIN_Set_Param(pRsrc, cSPIN_MIN_SPEED, pRsrc->reg.MIN_SPEED);
//	cSPIN_Set_Param(pRsrc, cSPIN_FS_SPD, pRsrc->reg.FS_SPD);
//#if defined(motor)
//	cSPIN_Set_Param(pRsrc, cSPIN_KVAL_HOLD, pRsrc->reg.KVAL_HOLD);
//	cSPIN_Set_Param(pRsrc, cSPIN_KVAL_RUN, pRsrc->reg.KVAL_RUN);
//	cSPIN_Set_Param(pRsrc, cSPIN_KVAL_ACC, pRsrc->reg.KVAL_ACC);
//	cSPIN_Set_Param(pRsrc, cSPIN_KVAL_DEC, pRsrc->reg.KVAL_DEC);
//	cSPIN_Set_Param(pRsrc, cSPIN_INT_SPD, pRsrc->reg.INT_SPD);
//	cSPIN_Set_Param(pRsrc, cSPIN_ST_SLP, pRsrc->reg.ST_SLP);
//	cSPIN_Set_Param(pRsrc, cSPIN_FN_SLP_ACC, pRsrc->reg.FN_SLP_ACC);
//	cSPIN_Set_Param(pRsrc, cSPIN_FN_SLP_DEC, pRsrc->reg.FN_SLP_DEC);
//	cSPIN_Set_Param(pRsrc, cSPIN_K_THERM, pRsrc->reg.K_THERM);
//	cSPIN_Set_Param(pRsrc, cSPIN_STALL_TH, pRsrc->reg.STALL_TH);
//#endif /* defined(motor) */
//#if defined(L6482)
//	cSPIN_Set_Param(pRsrc, cSPIN_TVAL_HOLD, pRsrc->reg.TVAL_HOLD);
//	cSPIN_Set_Param(pRsrc, cSPIN_TVAL_RUN, pRsrc->reg.TVAL_RUN);
//	cSPIN_Set_Param(pRsrc, cSPIN_TVAL_ACC, pRsrc->reg.TVAL_ACC);
//	cSPIN_Set_Param(pRsrc, cSPIN_TVAL_DEC, pRsrc->reg.TVAL_DEC);
//	cSPIN_Set_Param(pRsrc, cSPIN_T_FAST, pRsrc->reg.T_FAST);
//	cSPIN_Set_Param(cSPIN_TON_MIN, pRsrc->reg.TON_MIN);
//	cSPIN_Set_Param(pRsrc, cpRsrc, SPIN_TOFF_MIN, pRsrc->reg.TOFF_MIN);
//#endif /* defined(L6482) */
//	cSPIN_Set_Param(pRsrc, cSPIN_OCD_TH, pRsrc->reg.OCD_TH);
//	cSPIN_Set_Param(pRsrc, cSPIN_STEP_MODE, pRsrc->reg.STEP_MODE);
//	cSPIN_Set_Param(pRsrc, cSPIN_ALARM_EN, pRsrc->reg.ALARM_EN);
//	cSPIN_Set_Param(pRsrc, cSPIN_GATECFG1, pRsrc->reg.GATECFG1);
//	cSPIN_Set_Param(pRsrc, cSPIN_GATECFG2, pRsrc->reg.GATECFG2);
//	cSPIN_Set_Param(pRsrc, cSPIN_CONFIG, pRsrc->reg.CONFIG);
//}

///**
//  * @brief  Configures cSPIN internal registers with values in the config structure.
//  * @param  cSPIN_RegsStruct Configuration structure address (pointer to configuration structure)
//  * @retval None
//  */
//static void cSPIN_Registers_GetFew(cSPIN_Rsrc_T* pRsrc){
//	//¶ÁÈ¡
////	pRsrc->reg.ABS_POS = cSPIN_Get_Param(pRsrc, cSPIN_ABS_POS);
////	pRsrc->reg.SPEED = cSPIN_Get_Param(pRsrc, cSPIN_SPEED);
////	pRsrc->reg.ADC_OUT = cSPIN_Get_Param(pRsrc, cSPIN_ADC_OUT);
////	pRsrc->reg.STATUS = cSPIN_Get_Param(pRsrc, cSPIN_STATUS);	
//}

//static void cSPIN_Registers_GetAll(cSPIN_Rsrc_T* pRsrc)
//{	//
//	cSPIN_Registers_GetFew(pRsrc);
//	pRsrc->reg.EL_POS = cSPIN_Get_Param(pRsrc, cSPIN_EL_POS);
//	pRsrc->reg.MARK = cSPIN_Get_Param(pRsrc, cSPIN_MARK);
//	pRsrc->reg.ACC = cSPIN_Get_Param(pRsrc, cSPIN_ACC);
//	pRsrc->reg.DEC = cSPIN_Get_Param(pRsrc, cSPIN_DEC);
//	pRsrc->reg.MAX_SPEED = cSPIN_Get_Param(pRsrc, cSPIN_MAX_SPEED);
//	pRsrc->reg.MIN_SPEED = cSPIN_Get_Param(pRsrc, cSPIN_MIN_SPEED);
//	pRsrc->reg.FS_SPD = cSPIN_Get_Param(pRsrc, cSPIN_FS_SPD);
//#if defined(motor)
//	pRsrc->reg.KVAL_HOLD = cSPIN_Get_Param(pRsrc, cSPIN_KVAL_HOLD);
//	pRsrc->reg.KVAL_RUN = cSPIN_Get_Param(pRsrc, cSPIN_KVAL_RUN);
//	pRsrc->reg.KVAL_ACC = cSPIN_Get_Param(pRsrc, cSPIN_KVAL_ACC);
//	pRsrc->reg.KVAL_DEC = cSPIN_Get_Param(pRsrc, cSPIN_KVAL_DEC);
//	pRsrc->reg.INT_SPD = cSPIN_Get_Param(pRsrc, cSPIN_INT_SPD);
//	pRsrc->reg.ST_SLP = cSPIN_Get_Param(pRsrc, cSPIN_ST_SLP);
//	pRsrc->reg.FN_SLP_ACC = cSPIN_Get_Param(pRsrc, cSPIN_FN_SLP_ACC);
//	pRsrc->reg.FN_SLP_DEC = cSPIN_Get_Param(pRsrc, cSPIN_FN_SLP_DEC);
//	pRsrc->reg.K_THERM = cSPIN_Get_Param(pRsrc, cSPIN_K_THERM);
//	pRsrc->reg.STALL_TH = cSPIN_Get_Param(pRsrc, cSPIN_STALL_TH);
//#endif /* defined(motor) */
//#if defined(L6482)
//	pRsrc->reg.TVAL_HOLD = cSPIN_Get_Param(pRsrc, cSPIN_TVAL_HOLD);
//	pRsrc->reg.TVAL_RUN = cSPIN_Get_Param(pRsrc, cSPIN_TVAL_RUN);
//	pRsrc->reg.TVAL_ACC = cSPIN_Get_Param(pRsrc, cSPIN_TVAL_ACC);
//	pRsrc->reg.TVAL_DEC = cSPIN_Get_Param(pRsrc, cSPIN_TVAL_DEC);
//	pRsrc->reg.T_FAST = cSPIN_Get_Param(pRsrc, cSPIN_T_FAST);
//	pRsrc->reg.TON_MIN = cSPIN_Get_Param(cSPIN_TON_MIN, pRsrc->reg.TON_MIN);
//	pRsrc->reg.TOFF_MIN = cSPIN_Get_Param(pRsrc, cpRsrc, SPIN_TOFF_MIN);
//#endif /* defined(L6482) */
//	pRsrc->reg.OCD_TH = cSPIN_Get_Param(pRsrc, cSPIN_OCD_TH);
//	pRsrc->reg.STEP_MODE = cSPIN_Get_Param(pRsrc, cSPIN_STEP_MODE);
//	pRsrc->reg.ALARM_EN = cSPIN_Get_Param(pRsrc, cSPIN_ALARM_EN);
//	pRsrc->reg.GATECFG1 = cSPIN_Get_Param(pRsrc, cSPIN_GATECFG1);
//	pRsrc->reg.GATECFG2 = cSPIN_Get_Param(pRsrc, cSPIN_GATECFG2);
//	pRsrc->reg.CONFIG = cSPIN_Get_Param(pRsrc, cSPIN_CONFIG);
//}

/*******************************************************************************
* Function Name  : cSPIN_tickExe
* Description    : per
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void cSPIN_tickExe(cSPIN_Rsrc_T* pRsrc, u8 pollingTm){
	pRsrc->tick += pollingTm;
	if(pRsrc->tick < MOTOR_APP_POLLING_CYC)	return;
	pRsrc->tick = 0;
	
	//auto stop run
	if(pRsrc->runTmr){
		if(pRsrc->runTmr > MOTOR_APP_POLLING_CYC)
			pRsrc->runTmr -= MOTOR_APP_POLLING_CYC;
		else{
			pRsrc->runTmr = 0;
			cSPIN_Soft_Stop(pRsrc);
		}
	}

	//sampling reference sw
	if(pRsrc->SW_L.GPIOx!=NULL){
		pRsrc->refLSeries <<= 1;
		if(HAL_GPIO_ReadPin(pRsrc->SW_L.GPIOx, pRsrc->SW_L.GPIO_Pin) == GPIO_PIN_SET)
			pRsrc->refLSeries |= 1;		
	}
	if(pRsrc->SW_R.GPIOx!=NULL){
		pRsrc->refRSeries <<= 1;
		if(HAL_GPIO_ReadPin(pRsrc->SW_R.GPIOx, pRsrc->SW_R.GPIO_Pin) == GPIO_PIN_SET)
			pRsrc->refRSeries |= 1;
	}
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

/*******************************************************************************
* Function Name  : cmd
* Description    : command
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static void cSPIN_EnRptStatus(cSPIN_Rsrc_T* pRsrc){
	pRsrc->flags |= 1<<MOTOR_RPT_STATUS_BIT;
}
static void cSPIN_DisRptStatus(cSPIN_Rsrc_T* pRsrc){
	pRsrc->flags &= (1<<MOTOR_RPT_STATUS_BIT)^0xffff;
}
static void cSPIN_EnRptAbsPos(cSPIN_Rsrc_T* pRsrc){
	pRsrc->flags |= 1<<MOTOR_RPT_ABSPOS_BIT;
}
static void cSPIN_DisRptAbsPos(cSPIN_Rsrc_T* pRsrc){
	pRsrc->flags &= (1<<MOTOR_RPT_ABSPOS_BIT)^0xffff;
}
static void cSPIN_EnRptAdcOut(cSPIN_Rsrc_T* pRsrc){
	pRsrc->flags |= 1<<MOTOR_RPT_ADCOUT_BIT;
}
static void cSPIN_DisRptAdcOut(cSPIN_Rsrc_T* pRsrc){
	pRsrc->flags &= (1<<MOTOR_RPT_ADCOUT_BIT)^0xffff;
}

static void cSPIN_EromWrite(cSPIN_Rsrc_T* pRsrc, u8 addr, u32 val){
	u32 tmp = val;
	u8 buf[4];
	
	if(pRsrc->EepromWrite == NULL)	return;
	
	buf[0] = tmp;	tmp >>= 8;
	buf[1] = tmp;	tmp >>= 8;
	buf[2] = tmp;	
	buf[3] = 0xa5 ^ buf[0]^ buf[1]^ buf[2];
		
	pRsrc->EepromWrite(pRsrc->eepromAddrBase+addr*4, buf, 4);
}

static bool cSPIN_EromRead(cSPIN_Rsrc_T* pRsrc, u8 addr, u32* val){
	u32 tmp32 = 0;
	u8 buf[4] = {0}, tmp8;
	
	if(pRsrc->EepromRead == NULL)	return (FALSE);
	
	pRsrc->EepromRead(pRsrc->eepromAddrBase + addr*4, buf, 4);
		
	tmp8 = 0xa5 ^ buf[0]^ buf[1]^ buf[2];
	if(tmp8 == buf[3]){
		tmp32 = buf[2];		tmp32 <<= 8;
		tmp32 |= buf[1];	tmp32 <<= 8;
		tmp32 |= buf[0];
		*val = tmp32;
		return (TRUE);
	}
	return (FALSE);
}

/** @} */  
/******************* (C) COPYRIGHT 2013 STMicroelectronics *****END OF FILE****/
