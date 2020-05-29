/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : cspin_command.c
* Author             : Tiko Zhong
* Date First Issued  : 12/01/2015
* Description        : This file provides a set of functions needed to manage the
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "cspin_command.h"
#include "string.h"
#include "misc.h"
#include "board.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define FUN_LEN 64
#define L6480_DEF_CNT 8
CMD_DEF_T L6480_CMD_DEF[L6480_DEF_CNT] = {
	{(const char*)"fwd", (const char*)"1"},
	{(const char*)"rev", (const char*)"0"},
	{(const char*)"rst", (const char*)"0"},
	{(const char*)"cpy", (const char*)"8"},
};

const char L6480_HELP[] = {
	"Stepper Task Commands:"
	"\n sys.help(\"stepper_t\")"
	"\n sys.newdev(\"stepper_t\", \"devname\", DevIndx)"
	"\n sys.disposedev(\"devname\")"
	"\n sys.listdev(\"stepper_t\")"
	"\nStepper device Commands:"
	"\n %dev.rename(\"newName\")"
	"\n %dev.setAbsPos(0xFFFFFFFF)"
	"\n %dev.getAbsPos()"
	"\n %dev.setMarkPos(%val)"
	"\n %dev.getMarkPos()"
	"\n %dev.motor[*].setAcc(%val)"
	"\n %dev.getAcc()"
	"\n %dev.setDec(%val)"
	"\n %dev.getDec()"
	"\n %dev.setMaxSpd(%val)"
	"\n %dev.getMaxSpd()"
	"\n %dev.setMinSpd(%val)"
	"\n %dev.getMinSpd()"
	"\n %dev.SetLSpdOpt(%b)"
	"\n %dev.GetLSpdOpt()"
	"\n %dev.setFsSpd(%val)"
	"\n %dev.getFsSpd()"
	"\n %dev.setBoostMode(%val)"
	"\n %dev.getBoostMode()"
	"\n %dev.setAccDuty(u8)"
	"\n %dev.getAccDuty()"
	"\n %dev.setDecDuty(u8)"
	"\n %dev.getDecDuty()"
	"\n %dev.setRunDuty(u8)"
	"\n %dev.getRunDuty()"
	"\n %dev.setHoldDuty(u8)"
	"\n %dev.getHoldDuty()"
	"\n %dev.setIntSpd(%val)"
	"\n %dev.getIntSpd()"
	"\n %dev.setStartSlope(%val)"
	"\n %dev.getstartslope()"
	"\n %dev.setAccFinalSlope(%val)"
	"\n %dev.getAccFinalSlope()"
	"\n %dev.setDecFinalSlope(%val)"
	"\n %dev.getDecFinalSlope()"
	"\n %dev.SetKTherm(%hex)"
	"\n %dev.GetKTherm()"
	"\n %dev.SetOcdTh(%hex)"
	"\n %dev.GetOcdTh()"
	"\n %dev.SetStallTh(%hex)"
	"\n %dev.GetStallTh()"
	"\n %dev.SetStepSel(%hex)"
	"\n %dev.GetStepSel()"
	"\n %dev.SetSyncSel(%hex)"
	"\n %dev.GetSyncSel()"
	"\n %dev.SetSyncEn(%hex)"
	"\n %dev.GetSyncEn()"
	"\n %dev.SetGateTcc"
	"\n %dev.GetGateTcc"
	"\n %dev.SetGateI"
	"\n %dev.GetGateI"
	"\n %dev.SetGateTboost"
	"\n %dev.GetGateTBoost"
	"\n %dev.GetGateTDT"
	"\n %dev.SetGateTBlank"
	"\n %dev.GetGateTBlank"
	"\n %dev.SetSwMode"
	"\n %dev.GetSwMode"
	"\n %dev.SetVsCompEn"
	"\n %dev.GetVsCompEn"
	"\n %dev.SetOCSDEn"
	"\n %dev.GetOCSDEn"
	"\n %dev.SetUVLOVal"
	"\n %dev.GetUVLOVal"
	"\n %dev.SetVccVal"
	"\n %dev.GetVccVal"
	"\n %dev.SetPwmFrq"
	"\n %dev.GetPwmFrq"
	"\n %dev.getAdcOut()"
	"\n %dev.getStatus()"
	"\n %dev.getSpeed()"
	"\n %dev.run(fwd/rev,spd)"
	"\n %dev.move(fwd/rev,%nstep)"
	"\n %dev.goto(%absPos)"
	"\n %dev.gotoDir(fwd/rev,absPos)"
	"\n %dev.gotoAbs(%absPos)"
	"\n %dev.goUntil(cpy/rst,fwd/rev,speed)"
	"\n %dev.releaseSw(cpy/rst,fwd/rev)"
	"\n %dev.goHome()"
	"\n %dev.goMark()"
	"\n %dev.resetPos()"
	"\n %dev.softStop()"
	"\n %dev.hardStop()"
	"\n %dev.softHiz()"
	"\n %dev.hardHiz()\r\n"
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
//		//´òÓ¡
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
%dev.setup()
%dev.set_param(reg,val)
%dev.get_param()
%dev.run(fwd/rev,spd)
%dev.Step_Clock(fwd/rev)
%dev.move(fwd/rev,nstep)
%dev.go_to(absPos)
%dev.go_to_dir(fwd/rev,absPos)
%dev.go_until(cpy/rst,fwd/rev,speed)
%dev.release_sw(cpy/rst,fwd/rev) 
%dev.go_home()
%dev.go_mark()
%dev.reset_pos()
%dev.soft_stop()
%dev.hard_stop()
%dev.soft_hiz()
%dev.hard_hiz()
%dev.get_status()
*******************************************************************************/
//s8 l6480Cmd(void* p, const char *CMD, char* rtnBuf, u16 len)
s16 l6480Cmd(void* p, const char *CMD, RESP_T* rtnPkt){
	cSPIN_Rsrc_T* pRsrc;
	u16 x16;
	const char* FUNC;
	s32 num0,num1,num2,num3;     
	u16 tmp16,x160,x161,x162,x163;
	u8 tmp8;
	cSPIN_DEV_T* pDev = p;
	
	pRsrc = &pDev->rsrc;	
	//check if same name
	x16 = strlen(pRsrc->name);
	if(strncmp(pRsrc->name, (const char*)CMD, x16) )	return -1;
	FUNC = &CMD[x16];

	/***********************************************************
	@	dev.help()
	************************************************************/
	if(strncmp(FUNC, ".help  ", strlen(".help  "))==0){
		rtnPkt->ROM = L6480_HELP;
		return answerIt(rtnPkt, "+ok@%s.help()\n", pRsrc->name);
	}	
	
	else if(strncmp(FUNC, ".status  ", strlen(".status  "))==0){
		tmp16 = pDev->Get_Status(&pDev->rsrc);
		return answerIt(rtnPkt,   "+ok@%s.status(0x%x)\r\n", pRsrc->name, tmp16);
	}     
	else if(sscanf(FUNC, ".readreg %d", &num0)==1){
		num1 = pDev->Get_Param(&pDev->rsrc, (cSPIN_Registers_TypeDef)num0);
		return answerIt(rtnPkt,   "+ok@%s.readreg(0x%x, 0x%x)\r\n", pRsrc->name, num0, num1);
	}
	else if(sscanf(FUNC, ".writereg %d %d", &num0, &num1)==2){
		pDev->Set_Param(&pDev->rsrc, (cSPIN_Registers_TypeDef)num0, num1);
		return answerIt(rtnPkt,   "+ok@%s.writereg(%d,%d)\r\n", pRsrc->name, num0,num1);
	}
	//run
	else if(sscanf(FUNC, ".runtimeout %d %d %d ", &num0, &num1, &num2)==3 ){
		pDev->RunTimeout(&pDev->rsrc, (cSPIN_Direction_T)num0, num1, num2);
		return answerIt(rtnPkt,   "+ok@%s.runtimeout(%d,%d,%d)\r\n", pRsrc->name, num0,num1,num2);
	}
	else if(sscanf(FUNC, ".run %d %d ", &num0, &num1)==2){
		pDev->Run(&pDev->rsrc, (cSPIN_Direction_T)num0, num1);
		return answerIt(rtnPkt, "+ok@%s.run(%d,%d)\r\n", pRsrc->name, num0,num1);
	}
	//move
	else if(sscanf(FUNC, ".move %d %d ", &num0, &num1)==2){
		pDev->Move(&pDev->rsrc, (cSPIN_Direction_T)num0, num1);
		return answerIt(rtnPkt,   "+ok@%s.move(%d,%d)\r\n", pRsrc->name, num0,num1);
	}
	//stop 
	else if(sscanf(FUNC, ".stop %d ", &num0)==1){
		switch(num0){
			case 0:	//softStop, without hold, mosfet in hiz
				pDev->Soft_HiZ(&pDev->rsrc);
				return answerIt(rtnPkt,   "+ok@%s.stop(\"soft_hiz\")\r\n", pRsrc->name);					
			case 1:	//softStop, with hold
				pDev->Soft_Stop(&pDev->rsrc);
				return answerIt(rtnPkt,   "+ok@%s.stop(\"soft_hold\")\r\n", pRsrc->name);
			case 2:	//hardStop, without hold, mosfet in hiz
				pDev->Hard_HiZ(&pDev->rsrc);
				return answerIt(rtnPkt,   "+ok@%s.stop(\"hard_hiz\")\r\n", pRsrc->name);				
			case 3:	//hardStop, with hold
				pDev->Hard_Stop(&pDev->rsrc);
				return answerIt(rtnPkt,   "+ok@%s.stop(\"hard_hold\")\r\n", pRsrc->name);
			default:	//softStop, with hold
				pDev->Soft_Stop(&pDev->rsrc);
				return answerIt(rtnPkt,   "+ok@%s.stop(\"soft_hold\")\r\n", pRsrc->name);
		}
	}
	//reset
	else if(strncmp(FUNC, ".reset  ", strlen(".reset  "))==0){
		pDev->ResetDev(&pDev->rsrc);
		return answerIt(rtnPkt,   "+ok@%s.reset()\r\n", pRsrc->name);
	}
	//default
	else if(strncmp(FUNC, ".default  ", strlen(".default  "))==0){
		pDev->Default(&pDev->rsrc);
		return answerIt(rtnPkt,   "+ok@%s.default()\r\n", pRsrc->name);
	}
	//set drive current
	else if(sscanf(FUNC, ".setkval %d %d %d %d ", &num0,&num1,&num2,&num3)==4){
		pDev->RegSetKValAcc(&pDev->rsrc, (u8)num0);
		pDev->RegSetKValRun(&pDev->rsrc, (u8)num1);
		pDev->RegSetKValDec(&pDev->rsrc, (u8)num2);
		pDev->RegSetKValHold(&pDev->rsrc, (u8)num3);
		return answerIt(rtnPkt,   "+ok@%s.setkval(acc#%d,run#%d,dec#%d,hold#%d)\r\n", pRsrc->name, (u8)num0,(u8)num1,(u8)num2,(u8)num3);
	}
	else if(strncmp(FUNC, ".getkval  ", strlen(".getkval  "))==0){
		pDev->RegGetKValAcc(&pDev->rsrc, (u8*)&x160);
		pDev->RegGetKValRun(&pDev->rsrc, (u8*)&x161);
		pDev->RegGetKValDec(&pDev->rsrc, (u8*)&x162);
		pDev->RegGetKValHold(&pDev->rsrc, &tmp8);
		return answerIt(rtnPkt,   "+ok@%s.getkval(acc#%d,run#%d,dec#%d,hold#%d)\r\n", pRsrc->name, (u8)x160,(u8)x161,(u8)x162,tmp8);
	}
	else if(sscanf(FUNC, ".setkval_acc %d ", &num0)==1){
		pDev->RegSetKValAcc(&pDev->rsrc, num0);
		return answerIt(rtnPkt,   "+ok@%s.setkval_acc(%d)\r\n", pRsrc->name, (u8)num0);
	}
	else if(sscanf(FUNC, ".setkval_run %d ", &num0)==1){
		pDev->RegSetKValRun(&pDev->rsrc, num0);
		return answerIt(rtnPkt,   "+ok@%s.setkval_run(%d)\r\n", pRsrc->name, (u8)num0);
	}
	else if(sscanf(FUNC, ".setkval_dec %d ", &num0)==1){
		pDev->RegSetKValDec(&pDev->rsrc, num0);
		return answerIt(rtnPkt,   "+ok@%s.setkval_dec(%d)\r\n", pRsrc->name, (u8)num0);
	}
	else if(sscanf(FUNC, ".setkval_hold %d ", &num0)==1){
		pDev->RegSetKValHold(&pDev->rsrc, num0);
		return answerIt(rtnPkt,   "+ok@%s.setkval_hold(%d)\r\n", pRsrc->name, (u8)num0);
	}
	else if(strncmp(FUNC, ".getkval_acc  ", strlen(".getkval_acc  "))==0){
		pDev->RegGetKValAcc(&pDev->rsrc, &tmp8);
		return answerIt(rtnPkt,   "+ok@%s.getkval_acc(%d)\r\n", pRsrc->name, tmp8);
	}
	else if(strncmp(FUNC, ".getkval_run  ", strlen(".getkval_run  "))==0){
		pDev->RegGetKValAcc(&pDev->rsrc, &tmp8);
		return answerIt(rtnPkt,   "+ok@%s.getkval_run(%d)\r\n", pRsrc->name, tmp8);
	}
	else if(strncmp(FUNC, ".getkval_dec  ", strlen(".getkval_dec  "))==0){
		pDev->RegGetKValAcc(&pDev->rsrc, &tmp8);
		return answerIt(rtnPkt,   "+ok@%s.getkval_dec(%d)\r\n", pRsrc->name, tmp8);
	}
	else if(strncmp(FUNC, ".getkval_hold  ", strlen(".getkval_hold  "))==0){
		pDev->RegGetKValHold(&pDev->rsrc, &tmp8);
		return answerIt(rtnPkt,   "+ok@%s.getkval_hold(%d)\r\n", pRsrc->name, tmp8);
	}
	//set speed
	else if(sscanf(FUNC, ".setspeed %d %d %d ", &num0,&num1,&num2)==3){
		pDev->RegSetAcc(&pDev->rsrc, num0);
		pDev->RegSetDec(&pDev->rsrc, num1);
		pDev->RegSetMaxSpd(&pDev->rsrc, num2);
		return answerIt(rtnPkt,   "+ok@%s.setspeed(acc#%d,dec#%d,max#%d)\r\n", pRsrc->name, (u16)num0,(u16)num1,(u16)num2);
	}
	else if(strncmp(FUNC, ".getspeed  ", strlen(".getspeed  "))==0){
		pDev->RegGetAcc(&pDev->rsrc, &x160);
		pDev->RegGetDec(&pDev->rsrc, &x161);
		pDev->RegGetMaxSpd(&pDev->rsrc, &x162);
		return answerIt(rtnPkt,   "+ok@%s.getspeed(acc#%d,dec#%d,max#%d)\r\n", pRsrc->name, x160,x161,x162);
	}
	//full step speed
	else if(sscanf(FUNC, ".setfullspeed %d ", &num0)==1){
		pDev->RegSetFSSpd(&pDev->rsrc, (u16)num0);
		return answerIt(rtnPkt,   "+ok@%s.setfullspeed(%d)\r\n", pRsrc->name, (u16)num0); 
	}
	else if(strncmp(FUNC, ".getfullspeed  ", strlen(".getfullspeed  "))==0){
		pDev->RegGetFSSpd(&pDev->rsrc, &x160);
		return answerIt(rtnPkt,   "+ok@%s.getfullspeed(%d)\r\n", pRsrc->name, x160);
	}
	//back eletromative force comp
	else if(sscanf(FUNC, ".setbemc %d %d %d %d ", &num0,&num1,&num2,&num3)==4){
		pDev->RegSetIntSpd(&pDev->rsrc, (u16)num0);
		pDev->RegSetStartSlp(&pDev->rsrc, (u8)num1);
		pDev->RegSetFnSlpAcc(&pDev->rsrc, (u8)num2);
		pDev->RegSetFnSlpDec(&pDev->rsrc, (u8)num3);
		return answerIt(rtnPkt,   "+ok@%s.setbemc(intSpd#%d,stSlp#%d,fnSlpAcc#%d,fnSlpDec#%d)\r\n", pRsrc->name, (u16)num0,(u8)num1,(u8)num2,(u8)num3);
	}
	else if(strncmp(FUNC, ".getbemc  ", strlen(".getbemc  "))==0){
		pDev->RegGetIntSpd(&pDev->rsrc, (u16*)&x160);
		pDev->RegGetStartSlp(&pDev->rsrc, (u8*)&x161);
		pDev->RegGetFnSlpAcc(&pDev->rsrc, (u8*)&x162);
		pDev->RegGetFnSlpDec(&pDev->rsrc, (u8*)&x163);
		return answerIt(rtnPkt,   "+ok@%s.getbemc(intSpd#%d,stSlp#%d,fnSlpAcc#%d,fnSlpDec#%d)\r\n", pRsrc->name, (u16)x160,(u8)x161,(u8)x162,(u8)x163);
	}

	//over current threshold
	else if(sscanf(FUNC, ".setocd %d ", &num0)==1){
		pDev->RegSetOCDTh(&pDev->rsrc, (u8)num0);
		return answerIt(rtnPkt,   "+ok@%s.setocd(%d)\r\n", pRsrc->name, (u8)num0);
	}
	else if(strncmp(FUNC, ".getocd  ", strlen(".getocd  "))==0){
		pDev->RegGetOCDTh(&pDev->rsrc, (u8*)&num0);
		return answerIt(rtnPkt,   "+ok@%s.getocd(%d)\r\n", pRsrc->name, (u8)num0);				
	}
	//over stall threshold 
	else if(sscanf(FUNC, ".setstall %d ", &num0)==1){
		pDev->RegSetStallTh(&pDev->rsrc, (u8)num0);
		return answerIt(rtnPkt,   "+ok@%s.setstall(%d)\r\n", pRsrc->name, (u8)num0);
	}
	else if(strncmp(FUNC, ".getstall  ", strlen(".getstall  "))==0){
		pDev->RegGetStallTh(&pDev->rsrc, (u8*)&num0);
		return answerIt(rtnPkt,   "+ok@%s.getstall(%d)\r\n", pRsrc->name, (u8)num0);				
	}

	//setfullspd 
	else if(sscanf(FUNC, ".setfullspd %d ", &num0)==1){
		pDev->RegSetFSSpd(&pDev->rsrc, (u16)num0);
		return answerIt(rtnPkt,   "+ok@%s.setfullspd(%d)\r\n", pRsrc->name, (u16)num0);
	}
	else if(strncmp(FUNC, ".getfullspd  ", strlen(".getfullspd  "))==0){
		pDev->RegGetFSSpd(&pDev->rsrc, (u16*)&num0);				
		return answerIt(rtnPkt,   "+ok@%s.getfullspd(%d)\r\n", pRsrc->name, (u16)num0);			
	}

	/***********************************************************
	@	_set_param() Group
	************************************************************/
	/***********************************************************
	@	motor[*].setAbsPos(0xFFFFFFFF)
	@ val:[-power(2,21),power(2,21)-1]
	************************************************************/
	else if(sscanf(FUNC, ".setabspos %d ", &num0)==1){
		pDev->RegSetAbsPos(&pDev->rsrc, (u16)num0);
		return answerIt(rtnPkt,   "+ok@%s.setabspos(%d)\r\n", pRsrc->name, (u16)num0);
	}

	/***********************************************************
	"\n %dev.getAbsPos()
	@ return	:
	************************************************************/

	
	/***********************************************************
	"\n %dev.setMarkPos(%val)
	@ val:[-power(2,21),power(2,21)-1]
	************************************************************/
	else if(sscanf(FUNC, ".setmarkpos %d ", &num0)==1){
		pDev->RegSetMarkPos(&pDev->rsrc, (u16)num0);
		return answerIt(rtnPkt,   "+ok@%s.setmarkpos(%d)\r\n", pRsrc->name, (u16)num0);
	}

	/***********************************************************
	"\n %dev.getMarkPos()
	@ return	:
	************************************************************/


	/***********************************************************
	"\n %dev.motor[*].setAcc(%val)
	@ val:in uStep
	************************************************************/

	
	/***********************************************************
	"\n %dev.getAcc()
	@ return	:
	************************************************************/

	
	/***********************************************************
	"\n %dev.setDec(%val)
	@ val:in uStep
	************************************************************/


	/***********************************************************
	"\n %dev.getDec()
	@ return	:
	************************************************************/


	/***********************************************************
	"\n %dev.setMaxSpd(%val)
	@ val:in uStep
	************************************************************/


	/***********************************************************
	"\n %dev.getMaxSpd()
	@ return	:
	************************************************************/


	/***********************************************************
	"\n %dev.setMinSpd(%val)
	@ val:in uStep/64
	************************************************************/


	/***********************************************************
	"\n %dev.getMinSpd()
	@ return	:
	************************************************************/


	/***********************************************************
	"\n %dev.SetLSpdOpt(%b)
	@ val:in 
	************************************************************/


	/***********************************************************
	"\n %dev.GetLSpdOpt()
	@ val:in 
	************************************************************/


	/***********************************************************
	"\n %dev.setFsSpd(%val)
	@ val:in uStep
	************************************************************/


	/***********************************************************
	"\n %dev.getFsSpd()
	@ return	:
	************************************************************/


	/***********************************************************
	"\n %dev.setBoostMode(%val)
	@ val:1:enable
	************************************************************/


	/***********************************************************
	"\n %dev.getBoostMode()
	@ return	:
	************************************************************/


	/***********************************************************
	"\n %dev.setAccDuty(u8)
	@ return	:
	************************************************************/


	/***********************************************************
	"\n %dev.getAccDuty()
	@ return	:
	************************************************************/


	/***********************************************************
	"\n %dev.setDecDuty(u8)
	@ return	:
	************************************************************/


	/***********************************************************
	"\n %dev.getDecDuty()
	@ return	:
	************************************************************/


	/***********************************************************
	"\n %dev.setRunDuty(u8)
	@ return	:
	************************************************************/


	/***********************************************************
	"\n %dev.getRunDuty()
	@ return	:
	************************************************************/


	/***********************************************************
	"\n %dev.setHoldDuty(u8)
	@ return	:
	************************************************************/


	/***********************************************************
	"\n %dev.getHoldDuty()
	@ return	:
	************************************************************/


	/***********************************************************
	"\n %dev.setIntSpd(%val)
	@ val:in uStep
	************************************************************/

	
	/***********************************************************
	"\n %dev.getIntSpd()
	@ return	:
	************************************************************/


	/***********************************************************
	"\n %dev.setStartSlope(%val)
	@ val:in u*s/ustep
	************************************************************/


	/***********************************************************
	"\n %dev.getstartslope()
	@ return	:
	************************************************************/


	/***********************************************************
	"\n %dev.setAccFinalSlope(%val)
	@ val:in u*s/ustep
	************************************************************/


	/***********************************************************
	"\n %dev.getAccFinalSlope()
	@ return	:
	************************************************************/


	/***********************************************************
	"\n %dev.setDecFinalSlope(%val)
	@ val:in u*s/ustep
	************************************************************/


	/***********************************************************
	"\n %dev.getDecFinalSlope()
	@ return	:
	************************************************************/


	/***********************************************************
	"\n %dev.SetKTherm(%hex)
	@ val:in 
	************************************************************/


	/***********************************************************
	"\n %dev.GetKTherm()
	@ val:in 
	************************************************************/


	/***********************************************************
	"\n %dev.SetOcdTh(%hex)
	@ val:in 
	************************************************************/


	/***********************************************************
	"\n %dev.GetOcdTh()
	@ val:in 
	************************************************************/


	/***********************************************************
	"\n %dev.SetStallTh(%hex)
	@ val:in 
	************************************************************/


	/***********************************************************
	"\n %dev.GetStallTh()
	@ val:in 
	************************************************************/


	/***********************************************************
	"\n %dev.SetStepSel(%hex)
	@ val:
	************************************************************/
	else if(sscanf(FUNC, ".setstepsel %d ", &num0)==1){
		pDev->RegSetStepSel(&pDev->rsrc, (u16)num0);
		return answerIt(rtnPkt,   "+ok@%s.setstepsel(%d)\r\n", pRsrc->name, (u16)num0);
	}

	/***********************************************************
	"\n %dev.GetStepSel()
	@ val:in 
	************************************************************/
	else if(strncmp(FUNC, ".getstepsel  ", strlen(".getstepsel  "))==0){
		pDev->RegGetStepSel(&pDev->rsrc, (u8*)&num0);
		return answerIt(rtnPkt,   "+ok@%s.getstepsel(0x%x)\r\n", pRsrc->name, (u8)num0);				
	}

	/***********************************************************
	"\n %dev.SetSyncSel(%hex)
	@ val:
	************************************************************/
	else if(sscanf(FUNC, ".setsyncsel %d ", &num0)==1){
		pDev->RegSetSyncSel(&pDev->rsrc, (u16)num0);
		return answerIt(rtnPkt,   "+ok@%s.setsyncsel(%d)\r\n", pRsrc->name, (u16)num0);
	}

	/***********************************************************
	"\n %dev.GetSyncSel()
	@ val:
	************************************************************/
	else if(strncmp(FUNC, ".getsyncsel  ", strlen(".getsyncsel  "))==0){
		pDev->RegGetSyncSel(&pDev->rsrc, (u8*)&num0);
		return answerIt(rtnPkt,   "+ok@%s.getsyncsel(0x%x)\r\n", pRsrc->name, (u8)num0);				
	}
	
	/***********************************************************
	"\n %dev.SetSyncEn(%hex)
	@ 
	************************************************************/
	else if(sscanf(FUNC, ".setsyncen %d ", &num0)==1){
		pDev->RegSetSyncEn(&pDev->rsrc, (u16)num0);
		return answerIt(rtnPkt,   "+ok@%s.setsyncen(%d)\r\n", pRsrc->name, (u16)num0);
	}

	/***********************************************************
	"\n %dev.GetSyncEn()
	@ val:
	************************************************************/
	else if(strncmp(FUNC, ".getsyncen  ", strlen(".getsyncen  "))==0){
		pDev->RegGetSyncEn(&pDev->rsrc, (u8*)&num0);
		return answerIt(rtnPkt,   "+ok@%s.getsyncen(%d)\r\n", pRsrc->name, (u8)num0);				
	}

	/***********************************************************
	"\n %dev.SetGateTcc
	@ 
	************************************************************/


	/***********************************************************
	"\n %dev.GetGateTcc
	@ 
	************************************************************/


	/***********************************************************
	"\n %dev.SetGateI
	@ 
	************************************************************/


	/***********************************************************
	"\n %dev.GetGateI
	@ 
	************************************************************/


	/***********************************************************
	"\n %dev.SetGateTboost
	@ 
	************************************************************/


	/***********************************************************
	"\n %dev.GetGateTBoost
	@ 
	************************************************************/


	/***********************************************************
	"\n %dev.SetGateTDT
	@ 
	************************************************************/


	/***********************************************************
	"\n %dev.GetGateTDT
	@ 
	************************************************************/


	/***********************************************************
	"\n %dev.SetGateTBlank
	@ 
	************************************************************/

	/***********************************************************
	"\n %dev.GetGateTBlank
	@ 
	************************************************************/

	
	/***********************************************************
	"\n %dev.SetSwMode
	@ 
	************************************************************/

	
	/***********************************************************
	"\n %dev.GetSwMode
	@ 
	************************************************************/

	
	/***********************************************************
	"\n %dev.SetVsCompEn
	@ 
	************************************************************/


	/***********************************************************
	"\n %dev.GetVsCompEn
	@ 
	************************************************************/

	
	/***********************************************************
	"\n %dev.SetOCSDEn
	@ 
	************************************************************/

	
	/***********************************************************
	"\n %dev.GetOCSDEn
	@ 
	************************************************************/

	
	/***********************************************************
	"\n %dev.SetUVLOVal
	@ 
	************************************************************/

	
	/***********************************************************
	"\n %dev.GetUVLOVal
	@ 
	************************************************************/


	/***********************************************************
	"\n %dev.SetVccVal
	@ 
	************************************************************/

	
	/***********************************************************
	"\n %dev.GetVccVal
	@ 
	************************************************************/

	
	/***********************************************************
	"\n %dev.SetPwmFrq
	@ 
	************************************************************/

	
	/***********************************************************
	"\n %dev.GetPwmFrq
	@ 
	************************************************************/


	/***********************************************************
	"\n %dev.getAdcOut()
	@ return	:
	************************************************************/


	/***********************************************************
	"\n %dev.getStatus()
	@ return	:
	************************************************************/


	/***********************************************************
	"\n %dev.getSpeed()
	@ return	:
	************************************************************/



	/***********************************************************
	"\n %dev.StepClock((fwd/rev)
	@ spd	: steps/s, 
		This command can be given anytime and is immediately executed.
	************************************************************/
	else if(sscanf(FUNC, ".stepclock %d ", &num0)==1){
		pDev->Step_Clock(&pDev->rsrc, (cSPIN_Direction_T)num0);
		return answerIt(rtnPkt,   "+ok@%s.stepclock(%d)\r\n", pRsrc->name, (u16)num0);
	}

	/***********************************************************
	"\n %dev.run(fwd/rev,spd)
	@ spd	: steps/s, 
		This command can be given anytime and is immediately executed.
	************************************************************/


	/***********************************************************
	"\n %dev.move(fwd/rev,%nstep)
	@ spd	: steps/s, 
		This command can be given anytime and is immediately executed.
	************************************************************/


	/***********************************************************
	"\n %dev.goto(%absPos)
	@ absPos	: 
		This command can be given anytime and is immediately executed.
	************************************************************/


	/***********************************************************
	"\n %dev.gotoDir(fwd/rev,absPos)
	@ spd	: steps/s, 
		This command can be given anytime and is immediately executed.
	************************************************************/

	
	/***********************************************************
	"\n %dev.gotoAbs(%absPos)
	@ absPos	: 
		This command can be given anytime and is immediately executed.
	************************************************************/


	/***********************************************************
	"\n %dev.goUntil(cpy/rst,fwd/rev,speed)
	@ spd	: steps/s, 
		This command can be given anytime and is immediately executed.
	************************************************************/


	/***********************************************************
	"\n %dev.releaseSw(cpy/rst,fwd/rev) 
	@ 
		This command can be given anytime and is immediately executed.
	************************************************************/

	
	/***********************************************************
	"\n %dev.goHome()
	@ 
		This command can be given anytime and is immediately executed.
	************************************************************/


	/***********************************************************
	"\n %dev.goMark()
	@ 
		This command can be given anytime and is immediately executed.
	************************************************************/


	/***********************************************************
	"\n %dev.resetPos()
	@ 
		This command can be given anytime and is immediately executed.
	************************************************************/


	/***********************************************************
	"\n %dev.softStop()
	@ 
		This command can be given anytime and is immediately executed.
	************************************************************/

	
	/***********************************************************
	"\n %dev.hardStop()
	@ 
		This command can be given anytime and is immediately executed.
	************************************************************/


	/***********************************************************
	"\n %dev.softHiz()
	@ 
		This command can be given anytime and is immediately executed.
	************************************************************/

	
	/***********************************************************
	"\n %dev.hardHiz()
	@ 
		This command can be given anytime and is immediately executed.
	************************************************************/


//	/***********************************************************
//	"\n %dev.enableSatusReport
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
//	"\n %dev.disableSatusReport
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
//	"\n %dev.enableAbsPosReport
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
//	"\n %dev.disableAbsPosReport
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
//	"\n %dev.enableAdcOutReport
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
//	"\n %dev.disableAdcOutReport
//	@ 
//	************************************************************/
//	if(strNCmp(&cmdStr[j],(const u8*)"disableadcoutreport()",21)==0 ){
//		pDev->disableRptAdcOut(pRsrc);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
//		huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
//		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".disableAdcOutReport()\r\n");	
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
