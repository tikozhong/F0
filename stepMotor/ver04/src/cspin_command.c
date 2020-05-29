/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : cspin_command.c
* Author             : Tiko Zhong
* Date First Issued  : 12/01/2015
* Description        : This file provides a set of functions needed to manage the
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "cspin_command.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define FUN_LEN 64
#define L6480_DEF_CNT 8
CMD_DEF_T L6480_CMD_DEF[L6480_DEF_CNT] = {
	{(const u8*)"fwd", (const u8*)"1"},
	{(const u8*)"rev", (const u8*)"0"},
	{(const u8*)"rst", (const u8*)"0"},
	{(const u8*)"cpy", (const u8*)"8"},
	{(const u8*)"bl", (const u8*)"2"},
	{(const u8*)"rh", (const u8*)"4"},
	{(const u8*)"gh", (const u8*)"5"},
	{(const u8*)"bh", (const u8*)"6"},
};

const u8 L6480_HELP[] = {
	"Stepper Task Commands:"
	"\n sys.help(\"stepper_t\")"
	"\n sys.newdev(\"stepper_t\", \"devname\", DevIndx)"
	"\n sys.disposedev(\"devname\")"
	"\n sys.listdev(\"stepper_t\")"
	"\nStepper device Commands:"
	"\n %name.rename(\"newName\")"
	"\n %name.setAbsPos(0xFFFFFFFF)"
	"\n %name.getAbsPos()"
	"\n %name.setMarkPos(%val)"
	"\n %name.getMarkPos()"
	"\n %name.motor[*].setAcc(%val)"
	"\n %name.getAcc()"
	"\n %name.setDec(%val)"
	"\n %name.getDec()"
	"\n %name.setMaxSpd(%val)"
	"\n %name.getMaxSpd()"
	"\n %name.setMinSpd(%val)"
	"\n %name.getMinSpd()"
	"\n %name.SetLSpdOpt(%b)"
	"\n %name.GetLSpdOpt()"
	"\n %name.setFsSpd(%val)"
	"\n %name.getFsSpd()"
	"\n %name.setBoostMode(%val)"
	"\n %name.getBoostMode()"
	"\n %name.setAccDuty(u8)"
	"\n %name.getAccDuty()"
	"\n %name.setDecDuty(u8)"
	"\n %name.getDecDuty()"
	"\n %name.setRunDuty(u8)"
	"\n %name.getRunDuty()"
	"\n %name.setHoldDuty(u8)"
	"\n %name.getHoldDuty()"
	"\n %name.setIntSpd(%val)"
	"\n %name.getIntSpd()"
	"\n %name.setStartSlope(%val)"
	"\n %name.getstartslope()"
	"\n %name.setAccFinalSlope(%val)"
	"\n %name.getAccFinalSlope()"
	"\n %name.setDecFinalSlope(%val)"
	"\n %name.getDecFinalSlope()"
	"\n %name.SetKTherm(%hex)"
	"\n %name.GetKTherm()"
	"\n %name.SetOcdTh(%hex)"
	"\n %name.GetOcdTh()"
	"\n %name.SetStallTh(%hex)"
	"\n %name.GetStallTh()"
	"\n %name.SetStepSel(%hex)"
	"\n %name.GetStepSel()"
	"\n %name.SetSyncSel(%hex)"
	"\n %name.GetSyncSel()"
	"\n %name.SetSyncEn(%hex)"
	"\n %name.GetSyncSelEn()"
	"\n %name.SetGateTcc"
	"\n %name.GetGateTcc"
	"\n %name.SetGateI"
	"\n %name.GetGateI"
	"\n %name.SetGateTboost"
	"\n %name.GetGateTBoost"
	"\n %name.GetGateTDT"
	"\n %name.SetGateTBlank"
	"\n %name.GetGateTBlank"
	"\n %name.SetSwMode"
	"\n %name.GetSwMode"
	"\n %name.SetVsCompEn"
	"\n %name.GetVsCompEn"
	"\n %name.SetOCSDEn"
	"\n %name.GetOCSDEn"
	"\n %name.SetUVLOVal"
	"\n %name.GetUVLOVal"
	"\n %name.SetVccVal"
	"\n %name.GetVccVal"
	"\n %name.SetPwmFrq"
	"\n %name.GetPwmFrq"
	"\n %name.getAdcOut()"
	"\n %name.getStatus()"
	"\n %name.getSpeed()"
	"\n %name.run(fwd/rev,spd)"
	"\n %name.move(fwd/rev,%nstep)"
	"\n %name.goto(%absPos)"
	"\n %name.gotoDir(fwd/rev,absPos)"
	"\n %name.gotoAbs(%absPos)"
	"\n %name.goUntil(cpy/rst,fwd/rev,speed)"
	"\n %name.releaseSw(cpy/rst,fwd/rev)"
	"\n %name.goHome()"
	"\n %name.goMark()"
	"\n %name.resetPos()"
	"\n %name.softStop()"
	"\n %name.hardStop()"
	"\n %name.softHiz()"
	"\n %name.hardHiz()\r\n"
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
//u8 motor_command(UART_DEV *huartDev, cSPIN_DEV_T* pDev,u8* str);
//u8 motorCmdX1(UART_DEV *huartDev, const u8* cmdStr, cSPIN_DEV_T* pDev);

//	motorDevs[0].rsrc.callBackRefLTurnOn = motor0LSwTurOn;
//	motorDevs[0].rsrc.callBackRefLRelease = motor0LSwRelease;
//	motorDevs[0].rsrc.callBackRefRTurnOn = motor0RSwTurOn;
//	motorDevs[0].rsrc.callBackRefRRelease = motor0RSwRelease;
//	motorDevs[0].rsrc.callBackStatusChanged = motor0StatusChanged;
//	motorDevs[0].rsrc.callBackAbsPosChanged = motor0AbsPosChanged;
//	motorDevs[0].rsrc.callBackAdcOutChanged = motor0AdcOutChanged;

/* Public function prototypes -----------------------------------------------*/	

/*******************************************************************************
* Function Name  : reportAllregs
* Description    : auto report
* Input          : - huart : using this uart to print
									: - cmdStr : the command string
* Output         : None
* Return         : None 
*******************************************************************************/
//void motorReportAll(UART_DEV *huartDev, cSPIN_DEV_T *pDev, const u8* RtpNo){
//	u8 cmd[32] = {0};

//	if(pDev->rsrc.flags&(1<<MOTOR_REPORT)){
//		strCat(cmd, 32, (const u8*)pDev->name);
//		strCat(cmd, 32, (const u8*)".reportAllregs()");
//		huartDev->PrintStr(&huartDev->Rsrc, RtpNo);
//		motorCmdX1(huartDev, (const u8*)cmd, pDev);
//	}
//}

/*******************************************************************************
* Function Name  : outputReportFew
* Description    : auto report
* Input          : - huart : using this uart to print
									: - cmdStr : the command string
* Output         : None
* Return         : None 
*******************************************************************************/
//void motorReportFew(UART_DEV *huartDev, cSPIN_DEV_T *pDev, const u8* RtpNo){
//	u32 x32;	u16 x16;	u8 x8;
//	cSPIN_Rsrc_T* pRsrc = &pDev->rsrc;

//	while(1){
//		//打印
//		huartDev->PrintStr(&huartDev->Rsrc, RtpNo);
//		//		u32 ABS_POS;
//		if(pDev->RegGetAbsPos(pRsrc, &x32) == CMD_FAIL){
//			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err@report few\r\n");
//			break;
//		}
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%ABS_POS=");
//		huartDev->PrintHex32(&huartDev->Rsrc, x32);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t");
//		//		u32 SPEED;
//		if(pDev->RegGetSpeed(pRsrc, &x32) == CMD_FAIL){
//			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err@report few\r\n");
//			break;
//		}
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%SPEED=");
//		huartDev->PrintHex32(&huartDev->Rsrc, x32);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t");
//		//		u8  ADC_OUT;
//		if(pDev->RegGetAdcOut(pRsrc, &x8) == CMD_FAIL){
//			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err@report few\r\n");
//			break;
//		}
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%ADC_OUT=");
//		huartDev->PrintHex8(&huartDev->Rsrc, x8);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t");
//		//		u16 STATUS;
//		x16 = pDev->Get_Status(pRsrc);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%STATUS=");
//		huartDev->PrintHex16(&huartDev->Rsrc, x16);	
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t");
//		
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");			
//		break;
//	}
//}

/*******************************************************************************
* Function Name  : motorCmd 
* Description    : to branch
* Input          : - huart : using this uart to print
									: - cmdStr : the command string
* Output         : None
* Return         : None
*******************************************************************************/
//u8 motorCmd(UART_DEV *huartDev, const u8* cmdStr, cSPIN_DEV_T* pDev, u8 count){

//	u8 i,j,k;
//	
//	for(i=0; i<count; i++){
//		for(j=0,k=0; j<16; j++){
//			if(pDev[i].name[j] == 0)	break;
//			else{
//				if(pDev[i].name[j] == cmdStr[j])	continue;
//				else{	k =1; break;}
//			}
//		}
//		if(k)	continue;
//		if(cmdStr[j++] != '.' )	continue;	
//		return (motorCmdX1(huartDev, cmdStr, &pDev[i]));
//	}
//	return 0;
//}

/*******************************************************************************
* Function Name  : motorCmd
* Description    : motorCmd function commands
* Input          : - huart : using this uart to print
									: - cmdStr : the command string
* Output         : None
* Return         : None
%name.setup()
%name.set_param(reg,val)
%name.get_param()
%name.run(fwd/rev,spd)
%name.Step_Clock(fwd/rev)
%name.move(fwd/rev,nstep)
%name.go_to(absPos)
%name.go_to_dir(fwd/rev,absPos)
%name.go_until(cpy/rst,fwd/rev,speed)
%name.release_sw(cpy/rst,fwd/rev) 
%name.go_home()
%name.go_mark()
%name.reset_pos()
%name.soft_stop()
%name.hard_stop()
%name.soft_hiz()
%name.hard_hiz()
%name.get_status()
*******************************************************************************/
u8 l6480Cmd(cSPIN_DEV_T* pDev, const u8 *cmdStr, STR_PIECE_T *Content, COMPLIER_DEV_T* thsComplier){
	u8 funStr[FUN_LEN], *rtnStr, rtnStrSz;
	cSPIN_Rsrc_T* pRsrc;
	s8 tmpS8;
	u8 x8;
	u16 x16;
	u32 val,x32;
	PARAM_RSRC_T* e;

	pRsrc = &pDev->rsrc;	
	//check if same name
	if(strncmp((const char*)pRsrc->name, (const char*)cmdStr, strlen((const char*)pRsrc->name)) )	return 0;
	
	rtnStr = (u8*)Content->memPool;
	rtnStrSz = BYTES_PER_MAIL;	
	e = &thsComplier->rsrc;
	tmpS8 = thsComplier->Comply(e, cmdStr, L6480_CMD_DEF, L6480_DEF_CNT);
	
	pRsrc = &pDev->rsrc;
	
	/***********************************************************
	@	device.help()
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".help");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			strCpy(rtnStr, rtnStrSz, (const u8*)"+ok@");
			strCatX(rtnStr, pRsrc->name, (const u8*)".help()\n", NULL);
			Content->addr[1] = (u32)L6480_HELP;
		}
		else	RESPONSE.Err(rtnStr, rtnStrSz, cmdStr);
		return 1;
	}	
	
	/***********************************************************
	@	_set_param() Group
	************************************************************/
	/***********************************************************
	@	motor[*].setAbsPos(0xFFFFFFFF)
	@ val:[-power(2,21),power(2,21)-1]
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".setabspos");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 1){
			if(pDev->RegSetAbsPos(pRsrc, *(u32*)e->params[0]) == CMD_PASS){
				strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.getAbsPos()
	@ return	:
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".getabspos");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			if(pDev->RegGetAbsPos(pRsrc, &x32) == CMD_PASS){
				RESPONSE.Ok_D(rtnStr, rtnStrSz, funStr, &x32);
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}
	
	/***********************************************************
	"\n %name.setMarkPos(%val)
	@ val:[-power(2,21),power(2,21)-1]
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".setmarkpos");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 1){
			if(pDev->RegSetMarkPos(pRsrc, *(u32*)e->params[0]) == CMD_PASS){
				strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.getMarkPos()
	@ return	:
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".getmarkpos");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			if(pDev->RegGetMarkPos(pRsrc, &x32) == CMD_PASS){
				RESPONSE.Ok_D(rtnStr, rtnStrSz, funStr, &x32);
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.motor[*].setAcc(%val)
	@ val:in uStep
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".setacc");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 1){
			if(pDev->RegSetAcc(pRsrc, *(u32*)e->params[0]) == CMD_PASS){
				strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}
	
	/***********************************************************
	"\n %name.getAcc()
	@ return	:
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".getacc");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			if(pDev->RegGetAcc(pRsrc, &x16) == CMD_PASS){
				x32 = x16;
				RESPONSE.Ok_D(rtnStr, rtnStrSz, funStr, &x32);
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}
	
	/***********************************************************
	"\n %name.setDec(%val)
	@ val:in uStep
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".setdec");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 1){
			if(pDev->RegSetDec(pRsrc, *(u32*)e->params[0]) == CMD_PASS){
				strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.getDec()
	@ return	:
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".getdec");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			if(pDev->RegGetDec(pRsrc, &x16) == CMD_PASS){
				x32 = x16;
				RESPONSE.Ok_D(rtnStr, rtnStrSz, funStr, &x32);
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.setMaxSpd(%val)
	@ val:in uStep
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".setmaxspd");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 1){
			if(pDev->RegSetMaxSpd(pRsrc, *(u32*)e->params[0]) == CMD_PASS){
				strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.getMaxSpd()
	@ return	:
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".getmaxspd");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			if(pDev->RegGetMaxSpd(pRsrc, &x16) == CMD_PASS){
				x32 = x16;
				RESPONSE.Ok_D(rtnStr, rtnStrSz, funStr, &x32);
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.setMinSpd(%val)
	@ val:in uStep/64
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".setminspd");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 1){
			if(pDev->RegSetMinSpd(pRsrc, *(u32*)e->params[0]) == CMD_PASS){
				strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.getMinSpd()
	@ return	:
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".getminspd");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			if(pDev->RegGetMinSpd(pRsrc, &x16) == CMD_PASS){
				x32 = x16;
				RESPONSE.Ok_D(rtnStr, rtnStrSz, funStr, &x32);
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.SetLSpdOpt(%b)
	@ val:in 
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".setlspdopt");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 1){
			if(pDev->RegSetLowSpdOpt(pRsrc, *(u32*)e->params[0]) == CMD_PASS){
				strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.GetLSpdOpt()
	@ val:in 
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".getlspdopt");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			if(pDev->RegGetLowSpdOpt(pRsrc,&x8) == CMD_PASS){
				x32 = x16;
				RESPONSE.Ok_D(rtnStr, rtnStrSz, funStr, &x32);
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.setFsSpd(%val)
	@ val:in uStep
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".setfsspd");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 1){
			if(pDev->RegSetFSSpd(pRsrc, *(u32*)e->params[0]) == CMD_PASS){
				strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.getFsSpd()
	@ return	:
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".getfsspd");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			if(pDev->RegGetFSSpd(pRsrc,&x16) == CMD_PASS){
				x32 = x16;
				RESPONSE.Ok_D(rtnStr, rtnStrSz, funStr, &x32);
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.setBoostMode(%val)
	@ val:1:enable
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".setboostmode");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 1){
			if(pDev->RegSetBoostMode(pRsrc, *(u32*)e->params[0]) == CMD_PASS){
				strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.getBoostMode()
	@ return	:
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".getBoostMode");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			if(pDev->RegGetBoostMode(pRsrc, &x8) == CMD_PASS){
				x32 = x8;
				RESPONSE.Ok_D(rtnStr, rtnStrSz, funStr, &x32);
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.setAccDuty(u8)
	@ return	:
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".setaccduty");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 1){
			if(pDev->RegSetKValAcc(pRsrc, *(u32*)e->params[0]) == CMD_PASS){
				strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.getAccDuty()
	@ return	:
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".getaccduty");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			if(pDev->RegGetKValAcc(pRsrc,&x8)==CMD_PASS){
				x32 = x8;
				RESPONSE.Ok_D(rtnStr, rtnStrSz, funStr, &x32);
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.setDecDuty(u8)
	@ return	:
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".setdecduty");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 1){
			if(pDev->RegSetKValDec(pRsrc, *(u32*)e->params[0]) == CMD_PASS){
				strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.getDecDuty()
	@ return	:
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".getdecduty");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			if(pDev->RegGetKValDec(pRsrc, &x8)==CMD_PASS){
				x32 = x8;
				RESPONSE.Ok_D(rtnStr, rtnStrSz, funStr, &x32);
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.setRunDuty(u8)
	@ return	:
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".setrunduty");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 1){
			if(pDev->RegSetKValRun(pRsrc, *(u32*)e->params[0]) == CMD_PASS){
				strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.getRunDuty()
	@ return	:
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".getrunduty");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			if(pDev->RegGetKValRun(pRsrc,&x8)==CMD_PASS){
				x32 = x8;
				RESPONSE.Ok_D(rtnStr, rtnStrSz, funStr, &x32);
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.setHoldDuty(u8)
	@ return	:
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".setholdduty");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 1){
			if(pDev->RegSetKValHold(pRsrc, *(u32*)e->params[0]) == CMD_PASS){
				strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.getHoldDuty()
	@ return	:
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".getholdduty");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			if(pDev->RegGetKValHold(pRsrc,&x8) == CMD_PASS){
				x32 = x8;
				RESPONSE.Ok_D(rtnStr, rtnStrSz, funStr, &x32);
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.setIntSpd(%val)
	@ val:in uStep
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".setintspd");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 1){
			if(pDev->RegSetIntSpd(pRsrc, *(u32*)e->params[0]) == CMD_PASS){
				strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}
	
	/***********************************************************
	"\n %name.getIntSpd()
	@ return	:
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".getintspd");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			if(pDev->RegGetIntSpd(pRsrc,&x16) == CMD_PASS){
				x32 = x16;
				RESPONSE.Ok_D(rtnStr, rtnStrSz, funStr, &x32);
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.setStartSlope(%val)
	@ val:in u*s/ustep
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".setstartslope");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 1){
			if(pDev->RegSetStartSlp(pRsrc, *(u32*)e->params[0]) == CMD_PASS){
				strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.getstartslope()
	@ return	:
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".getstartslope");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			if(pDev->RegGetStartSlp(pRsrc,&x8) == CMD_PASS){
				x32 = x8;
				RESPONSE.Ok_D(rtnStr, rtnStrSz, funStr, &x32);
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.setAccFinalSlope(%val)
	@ val:in u*s/ustep
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".setaccfinalslope");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 1){
			if(pDev->RegSetFnSlpAcc(pRsrc, *(u32*)e->params[0]) == CMD_PASS){
				strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.getAccFinalSlope()
	@ return	:
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".getaccfinalslope");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			if(pDev->RegGetFnSlpAcc(pRsrc,&x8) == CMD_PASS){
				x32 = x8;
				RESPONSE.Ok_D(rtnStr, rtnStrSz, funStr, &x32);
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.setDecFinalSlope(%val)
	@ val:in u*s/ustep
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".setdecfinalslope");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 1){
			if(pDev->RegSetFnSlpDec(pRsrc, *(u32*)e->params[0]) == CMD_PASS){
				strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.getDecFinalSlope()
	@ return	:
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".getdecfinalslope");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			if(pDev->RegGetFnSlpDec(pRsrc,&x8) == CMD_PASS){
				x32 = x8;
				RESPONSE.Ok_D(rtnStr, rtnStrSz, funStr, &x32);
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.SetKTherm(%hex)
	@ val:in 
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".setktherm");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 1){
			if(pDev->RegSetKTherm(pRsrc, *(u32*)e->params[0]) == CMD_PASS){
				strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.GetKTherm()
	@ val:in 
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".getktherm");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			if(pDev->RegGetKTherm(pRsrc,&x8) == CMD_PASS){
				x32 = x8;
				RESPONSE.Ok_D(rtnStr, rtnStrSz, funStr, &x32);
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.SetOcdTh(%hex)
	@ val:in 
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".setocdth");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 1){
			if(pDev->RegSetOCDTh(pRsrc, *(u32*)e->params[0]) == CMD_PASS){
				strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.GetOcdTh()
	@ val:in 
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".getocdth");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			if(pDev->RegGetOCDTh(pRsrc,&x8) == CMD_PASS){
				x32 = x8;
				RESPONSE.Ok_D(rtnStr, rtnStrSz, funStr, &x32);
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.SetStallTh(%hex)
	@ val:in 
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".setstallth");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 1){
			if(pDev->RegSetStallTh(pRsrc, *(u32*)e->params[0]) == CMD_PASS){
				strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.GetStallTh()
	@ val:in 
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".getstallth");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			if(pDev->RegGetStallTh(pRsrc,&x8) == CMD_PASS){
				x32 = x8;
				RESPONSE.Ok_D(rtnStr, rtnStrSz, funStr, &x32);
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.SetStepSel(%hex)
	@ val:
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".setstepsel");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 1){
			if(pDev->RegSetStepSel(pRsrc, *(u32*)e->params[0]) == CMD_PASS){
				strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.GetStepSel()
	@ val:in 
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".getstepsel");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			if(pDev->RegGetStepSel(pRsrc,&x8) == CMD_PASS){
				x32 = x8;
				RESPONSE.Ok_D(rtnStr, rtnStrSz, funStr, &x32);
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.SetSyncSel(%hex)
	@ val:
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".setsyncsel");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 1){
			if(pDev->RegSetSyncSel(pRsrc, *(u32*)e->params[0]) == CMD_PASS){
				strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.GetSyncSel()
	@ val:
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".getsyncsel");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			if(pDev->RegGetSyncSel(pRsrc,&x8) == CMD_PASS){
				x32 = x8;
				RESPONSE.Ok_D(rtnStr, rtnStrSz, funStr, &x32);
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}
	
	/***********************************************************
	"\n %name.SetSyncEn(%hex)
	@ 
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".setsyncen");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 1){
			if(pDev->RegSetSyncEn(pRsrc, *(u32*)e->params[0]) == CMD_PASS){
				strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.GetSyncSelEn()
	@ val:
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".getsyncen");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			if(pDev->RegGetSyncEn(pRsrc,&x8) == CMD_PASS){
				x32 = x8;
				RESPONSE.Ok_D(rtnStr, rtnStrSz, funStr, &x32);
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.SetGateTcc
	@ 
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".setgatetcc");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 1){
			if(pDev->RegSetGateTcc(pRsrc, *(u32*)e->params[0]) == CMD_PASS){
				strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.GetGateTcc
	@ 
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".getgatetcc");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			if(pDev->RegGetGateTcc(pRsrc,&x8) == CMD_PASS){
				x32 = x8;
				RESPONSE.Ok_D(rtnStr, rtnStrSz, funStr, &x32);
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.SetGateI
	@ 
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".setgatei");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 1){
			if(pDev->RegSetGateCurrent(pRsrc, *(u32*)e->params[0]) == CMD_PASS){
				strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.GetGateI
	@ 
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".getgatei");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			if(pDev->RegGetGateCurrent(pRsrc,&x8) == CMD_PASS){
				x32 = x8;
				RESPONSE.Ok_D(rtnStr, rtnStrSz, funStr, &x32);
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.SetGateTboost
	@ 
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".setgatetboost");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 1){
			if(pDev->RegSetGateTBoost(pRsrc, *(u32*)e->params[0]) == CMD_PASS){
				strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.GetGateTBoost
	@ 
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".getgatetboost");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			if(pDev->RegGetGateTBoost(pRsrc,&x8) == CMD_PASS){
				x32 = x8;
				RESPONSE.Ok_D(rtnStr, rtnStrSz, funStr, &x32);
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.SetGateTDT
	@ 
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".setgatetdt");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 1){
			if(pDev->RegSetGateTDeadTime(pRsrc, *(u32*)e->params[0]) == CMD_PASS){
				strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.GetGateTDT
	@ 
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".getgatetdt");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			if(pDev->RegGetGateTDeadTime(pRsrc,&x8) == CMD_PASS){
				x32 = x8;
				RESPONSE.Ok_D(rtnStr, rtnStrSz, funStr, &x32);
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.SetGateTBlank
	@ 
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".setgatetblank");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 1){
			if(pDev->RegSetGateTBlank(pRsrc, *(u32*)e->params[0]) == CMD_PASS){
				strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.GetGateTBlank
	@ 
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".getgatetblank");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			if(pDev->RegGetGateTBlank(pRsrc,&x8) == CMD_PASS){
				x32 = x8;
				RESPONSE.Ok_D(rtnStr, rtnStrSz, funStr, &x32);
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}
	
	/***********************************************************
	"\n %name.SetSwMode
	@ 
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".setswmode");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 1){
			if(pDev->RegSetSwMode(pRsrc, *(u32*)e->params[0]) == CMD_PASS){
				strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}
	
	/***********************************************************
	"\n %name.GetSwMode
	@ 
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".getswmode");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			if(pDev->RegGetSwMode(pRsrc,&x8) == CMD_PASS){
				x32 = x8;
				RESPONSE.Ok_D(rtnStr, rtnStrSz, funStr, &x32);
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}
	
	/***********************************************************
	"\n %name.SetVsCompEn
	@ 
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".setvscompen");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 1){
			if(pDev->RegSetVsCompEn(pRsrc, *(u32*)e->params[0]) == CMD_PASS){
				strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.GetVsCompEn
	@ 
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".getvscompen");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			if(pDev->RegGetVsCompEn(pRsrc,&x8) == CMD_PASS){
				x32 = x8;
				RESPONSE.Ok_D(rtnStr, rtnStrSz, funStr, &x32);
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}
	
	/***********************************************************
	"\n %name.SetOCSDEn
	@ 
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".setocsden");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 1){
			if(pDev->RegSetOCSDEn(pRsrc, *(u32*)e->params[0]) == CMD_PASS){
				strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}
	
	/***********************************************************
	"\n %name.GetOCSDEn
	@ 
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".getocsden");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			if(pDev->RegGetOCSDEn(pRsrc,&x8) == CMD_PASS){
				x32 = x8;
				RESPONSE.Ok_D(rtnStr, rtnStrSz, funStr, &x32);
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}
	
	/***********************************************************
	"\n %name.SetUVLOVal
	@ 
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".setuvloval");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 1){
			if(pDev->RegSetUVLOVal(pRsrc, *(u32*)e->params[0]) == CMD_PASS){
				strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}
	
	/***********************************************************
	"\n %name.GetUVLOVal
	@ 
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".getuvloval");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			if(pDev->RegGetUVLOVal(pRsrc,&x8) == CMD_PASS){
				x32 = x8;
				RESPONSE.Ok_D(rtnStr, rtnStrSz, funStr, &x32);
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.SetVccVal
	@ 
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".setvccval");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 1){
			if(pDev->RegSetVccVal(pRsrc, *(u32*)e->params[0]) == CMD_PASS){
				strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}
	
	/***********************************************************
	"\n %name.GetVccVal
	@ 
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".getvccval");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			if(pDev->RegGetVccVal(pRsrc,&x8) == CMD_PASS){
				x32 = x8;
				RESPONSE.Ok_D(rtnStr, rtnStrSz, funStr, &x32);
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}
	
	/***********************************************************
	"\n %name.SetPwmFrq
	@ 
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".setpwmfrq");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 1){
			if(pDev->RegSetPwmFrq(pRsrc, *(u32*)e->params[0]) == CMD_PASS){
				strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}
	
	/***********************************************************
	"\n %name.GetPwmFrq
	@ 
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".getpwmfrq");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			if(pDev->RegGetPwmFrq(pRsrc,&x8) == CMD_PASS){
				x32 = x8;
				RESPONSE.Ok_D(rtnStr, rtnStrSz, funStr, &x32);
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.getAdcOut()
	@ return	:
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".getadcout");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			if(pDev->RegGetAdcOut(pRsrc,&x8)==CMD_PASS){
				pRsrc->reg.STATUS = pDev->Get_Param(pRsrc, cSPIN_STATUS);
				x32 = x8;
				RESPONSE.Ok_D(rtnStr, rtnStrSz, funStr, &x32);
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.getStatus()
	@ return	:
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".getstatus");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			x16 = pDev->Get_Status(pRsrc);
			x32 = x16;
			RESPONSE.Ok_D(rtnStr, rtnStrSz, funStr, &x32);
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.getSpeed()
	@ return	:
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".getspeed");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			x32 = pDev->Get_Param(pRsrc, cSPIN_SPEED);
			RESPONSE.Ok_D(rtnStr, rtnStrSz, funStr, &x32);
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.run(fwd/rev,spd)
	@ spd	: steps/s, 
		This command can be given anytime and is immediately executed.
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".run");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 2){
			pDev->Run(pRsrc, (cSPIN_Direction_T)*(u32*)e->params[0], Speed_Steps_to_Par(*(u32*)e->params[1]));
			strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.move(fwd/rev,%nstep)
	@ spd	: steps/s, 
		This command can be given anytime and is immediately executed.
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".move");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 2){
			pDev->Move(pRsrc, (cSPIN_Direction_T)*(u32*)e->params[0], *(u32*)e->params[1]);
			strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.goto(%absPos)
	@ absPos	: 
		This command can be given anytime and is immediately executed.
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".goto");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 1){
			pDev->Go_To(pRsrc, *(u32*)e->params[0]);
			strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.gotoDir(fwd/rev,absPos)
	@ spd	: steps/s, 
		This command can be given anytime and is immediately executed.
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".gotodir");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 2){
			pDev->Go_To_Dir(pRsrc, (cSPIN_Direction_T)*(u32*)e->params[0], *(u32*)e->params[1]);
			strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}
	
	/***********************************************************
	"\n %name.gotoAbs(%absPos)
	@ absPos	: 
		This command can be given anytime and is immediately executed.
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".gotoabs");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 1){
			if(pDev->RegGetAbsPos(&pDev->rsrc,&x32) == CMD_PASS){
				val = *(u32*)e->params[0];
				if(x32 > val)				pDev->Go_To_Dir(pRsrc, FWD, val);
				else if(x32 < val) 	pDev->Go_To_Dir(pRsrc, REV, val);
				strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
			}
			else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_func_fail");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.goUntil(cpy/rst,fwd/rev,speed)
	@ spd	: steps/s, 
		This command can be given anytime and is immediately executed.
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".gountil");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 3){
			pDev->Go_Until(pRsrc, (cSPIN_Action_T)*(u32*)e->params[0], (cSPIN_Direction_T)*(u32*)e->params[1], Speed_Steps_to_Par(*(u32*)e->params[2]));
			strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.releaseSw(cpy/rst,fwd/rev) 
	@ 
		This command can be given anytime and is immediately executed.
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".releasesw");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 2){
			pDev->Release_SW(pRsrc, (cSPIN_Action_T)*(u32*)e->params[0], (cSPIN_Direction_T)*(u32*)e->params[1]);
			strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}
	
	/***********************************************************
	"\n %name.goHome()
	@ 
		This command can be given anytime and is immediately executed.
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".gohome");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			pDev->Go_Home(pRsrc);
			strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.goMark()
	@ 
		This command can be given anytime and is immediately executed.
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".gomark");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			pDev->Go_Mark(pRsrc);
			strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.resetPos()
	@ 
		This command can be given anytime and is immediately executed.
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".resetpos");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			pDev->Reset_Pos(pRsrc);
			strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.softStop()
	@ 
		This command can be given anytime and is immediately executed.
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".softstop");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			pDev->Soft_Stop(pRsrc);
			strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}
	
	/***********************************************************
	"\n %name.hardStop()
	@ 
		This command can be given anytime and is immediately executed.
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".hardstop");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			pDev->Hard_Stop(pRsrc);
			strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

	/***********************************************************
	"\n %name.softHiz()
	@ 
		This command can be given anytime and is immediately executed.
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".softhiz");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			pDev->Soft_HiZ(pRsrc);
			strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}
	
	/***********************************************************
	"\n %name.hardHiz()
	@ 
		This command can be given anytime and is immediately executed.
	************************************************************/
	strCpy(funStr, FUN_LEN, pRsrc->name);
	strcat((char*)funStr, (const char*)".hardhiz");
	if(isSameStr(e->funName, funStr) == 1 ){
		if(tmpS8 == 0){
			pDev->Hard_HiZ(pRsrc);
			strCatX(rtnStr, (const u8*)"+ok@", cmdStr, (const u8*)"\r\n");
		}
		else	RESPONSE.Err_S(rtnStr, rtnStrSz, funStr, (const u8*)"err_param_cnt");
		return 1;
	}

//	/***********************************************************
//	"\n %name.enableSatusReport
//	@ 
//	************************************************************/
//	if(strNCmp(&cmdStr[j],(const u8*)"enablestatusreport()",20)==0 ){
//		pDev->enableRptStatus(pRsrc);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
//		huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".enableStatusReport()\r\n");	
//		return 1;
//	}
//	/***********************************************************
//	"\n %name.disableSatusReport
//	@ 
//	************************************************************/
//	if(strNCmp(&cmdStr[j],(const u8*)"disablestatusreport()",21)==0 ){
//		pDev->disableRptStatus(pRsrc);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
//		huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".enableStatusReport()\r\n");	
//		return 1;
//	}
//	/***********************************************************
//	"\n %name.enableAbsPosReport
//	@ 
//	************************************************************/
//	if(strNCmp(&cmdStr[j],(const u8*)"enableabsposreport()",20)==0 ){
//		pDev->enableRptAbsPos(pRsrc);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
//		huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".enableAbsPosReport()\r\n");	
//		return 1;
//	}
//	/***********************************************************
//	"\n %name.disableAbsPosReport
//	@ 
//	************************************************************/
//	if(strNCmp(&cmdStr[j],(const u8*)"disableabsposreport()",21)==0 ){
//		pDev->disableRptAbsPos(pRsrc);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
//		huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".disableAbsPosReport()\r\n");	
//		return 1;
//	}
//	/***********************************************************
//	"\n %name.enableAdcOutReport
//	@ 
//	************************************************************/
//	if(strNCmp(&cmdStr[j],(const u8*)"enableadcoutreport()",20)==0 ){
//		pDev->enableRptAdcOut(pRsrc);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
//		huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".enableAdcOutReport()\r\n");	
//		return 1;
//	}
//	/***********************************************************
//	"\n %name.disableAdcOutReport
//	@ 
//	************************************************************/
//	if(strNCmp(&cmdStr[j],(const u8*)"disableadcoutreport()",21)==0 ){
//		pDev->disableRptAdcOut(pRsrc);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
//		huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".disableAdcOutReport()\r\n");	
//		return 1;
//	}
	
	/***********************************************************
	"\n %name.reportAllRegs()
	@ 
		This command can be given anytime and is immediately executed.
	************************************************************/
//	if(strNCmp(&cmdStr[j],(const u8*)"reportallregs()",15)==0 ){
//		//读取所有寄存器
//		pDev->Get_All(&pDev->rsrc);
//		huartDev->PrintStr(&huartDev->Rsrc,(const u8*)"+ok @");
//		huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".reportallregs()\t");
//		//		u32 ABS_POS;
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"ABS_POS@");
//		huartDev->PrintHex32(&huartDev->Rsrc, pRsrc->reg.ABS_POS);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t");
//		//		u16 EL_POS;
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"EL_POS@");
//		huartDev->PrintHex16(&huartDev->Rsrc, pRsrc->reg.EL_POS);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t");
//		//		u32 MARK;
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"MARK@");
//		huartDev->PrintHex32(&huartDev->Rsrc, pRsrc->reg.MARK);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t");
//		//		u32 SPEED;
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"SPEED@");
//		huartDev->PrintHex32(&huartDev->Rsrc, pRsrc->reg.SPEED);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t");
//		//		u16 ACC;
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"ACC@");
//		huartDev->PrintHex16(&huartDev->Rsrc, pRsrc->reg.ACC);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t");
//		//		u16 DEC;
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"DEC@");
//		huartDev->PrintHex16(&huartDev->Rsrc, pRsrc->reg.DEC);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t");
//		//		u16 MAX_SPEED;
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"MAX_SPEED@");
//		huartDev->PrintHex16(&huartDev->Rsrc, pRsrc->reg.MAX_SPEED);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t");
//		//		u16 MIN_SPEED;
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"MIN_SPEED@");
//		huartDev->PrintHex16(&huartDev->Rsrc, pRsrc->reg.MIN_SPEED);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t");
//		//		u16 FS_SPD;
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"FS_SPD@");
//		huartDev->PrintHex16(&huartDev->Rsrc, pRsrc->reg.FS_SPD);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t");
//		//		u8  KVAL_HOLD;
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"KVAL_HOLD@");
//		huartDev->PrintHex8(&huartDev->Rsrc, pRsrc->reg.KVAL_HOLD);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t");
//		//		u8  KVAL_RUN;
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"KVAL_RUN@");
//		huartDev->PrintHex8(&huartDev->Rsrc, pRsrc->reg.KVAL_RUN);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t");
//		//		u8  KVAL_ACC;
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"KVAL_ACC@");
//		huartDev->PrintHex8(&huartDev->Rsrc, pRsrc[i].reg.KVAL_ACC);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t");
//		//		u8  KVAL_DEC;
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"KVAL_DEC@");
//		huartDev->PrintHex8(&huartDev->Rsrc, pRsrc->reg.KVAL_DEC);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t");
//		//		u16 INT_SPD;
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"INT_SPD@");
//		huartDev->PrintHex16(&huartDev->Rsrc, pRsrc->reg.INT_SPD);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t");
//		//		u8  ST_SLP;
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"ST_SLP@");
//		huartDev->PrintHex8(&huartDev->Rsrc, pRsrc->reg.ST_SLP);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t");
//		//		u8  FN_SLP_ACC;
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"FN_SLP_ACC@");
//		huartDev->PrintHex8(&huartDev->Rsrc, pRsrc->reg.FN_SLP_ACC);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t");
//		//		u8  FN_SLP_DEC;
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"FN_SLP_DEC@");
//		huartDev->PrintHex8(&huartDev->Rsrc, pRsrc->reg.FN_SLP_DEC);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t");
//		//		u8  K_THERM;
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"K_THERM@");
//		huartDev->PrintHex8(&huartDev->Rsrc, pRsrc->reg.K_THERM);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t");
//		//		u8  ADC_OUT;
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"ADC_OUT@");
//		huartDev->PrintHex8(&huartDev->Rsrc, pRsrc->reg.ADC_OUT);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t");
//		//		u8  OCD_TH;
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"OCD_TH@");
//		huartDev->PrintHex8(&huartDev->Rsrc, pRsrc->reg.OCD_TH);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t");
//		//		u8  STALL_TH;
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"STALL_TH@");
//		huartDev->PrintHex8(&huartDev->Rsrc, pRsrc->reg.STALL_TH);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t");
//		//		u8  STEP_MODE;
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"STEP_MODE@");
//		huartDev->PrintHex8(&huartDev->Rsrc, pRsrc->reg.STEP_MODE);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t");
//		//		u8  ALARM_EN;
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"ALARM_EN@");
//		huartDev->PrintHex8(&huartDev->Rsrc, pRsrc->reg.ALARM_EN);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t");
//		//		u16 GATECFG1;
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"GATECFG1@");
//		huartDev->PrintHex16(&huartDev->Rsrc, pRsrc->reg.GATECFG1);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t");
//		//		u8  GATECFG2;  
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"GATECFG2@");
//		huartDev->PrintHex8(&huartDev->Rsrc, pRsrc->reg.GATECFG2);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t");
//		//		u16 CONFIG;
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"CONFIG@");
//		huartDev->PrintHex16(&huartDev->Rsrc, pRsrc->reg.CONFIG);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t");
//		//		u16 STATUS;
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"STATUS@");
//		huartDev->PrintHex16(&huartDev->Rsrc, pRsrc->reg.STATUS);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t");

//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");	
//	
//		return 1;
//	}	
	return 0;
}

//void motorReportAbsPos(UART_DEV *huartDev,const u8* leading, u32 pos){
//	huartDev->PrintStr(&huartDev->Rsrc, leading);
//	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".get_abs_pos()\t%ABS_POS=");
//	huartDev->PrintHex32(&huartDev->Rsrc, pos);
//	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");		
//}

//void motorReportStatus(UART_DEV *huartDev,const u8* leading, u16 sta){
//	huartDev->PrintStr(&huartDev->Rsrc, leading);
//	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".get_status()\t");
//	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%STATUS=");
//	huartDev->PrintHex16(&huartDev->Rsrc, sta);
//	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");
//}

//void motorReportAdcOut(UART_DEV *huartDev,const u8* leading, u8 adc){
//	huartDev->PrintStr(&huartDev->Rsrc, leading);
//	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".get_adc_out()\t%ADC_OUT=");
//	huartDev->PrintHex8(&huartDev->Rsrc, adc);
//	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");
//}


/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
