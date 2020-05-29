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
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
u8 motor_command(UART_DEV *huartDev, cSPIN_DEV_T* pDev,u8* str);
u8 motorCmdX1(UART_DEV *huartDev, const u8* cmdStr, cSPIN_DEV_T* pDev);

//	motorDevs[0].rsrc.callBackRefLTurnOn = motor0LSwTurOn;
//	motorDevs[0].rsrc.callBackRefLRelease = motor0LSwRelease;
//	motorDevs[0].rsrc.callBackRefRTurnOn = motor0RSwTurOn;
//	motorDevs[0].rsrc.callBackRefRRelease = motor0RSwRelease;
//	motorDevs[0].rsrc.callBackStatusChanged = motor0StatusChanged;
//	motorDevs[0].rsrc.callBackAbsPosChanged = motor0AbsPosChanged;
//	motorDevs[0].rsrc.callBackAdcOutChanged = motor0AdcOutChanged;

/* Public function prototypes -----------------------------------------------*/	
/*******************************************************************************
* Function Name  : motorCmd
* Description    : motorCmd function commands
* Input          : - huart : using this uart to print
								 : - cmdStr : the command string
* Output         : None
* Return         : None
*******************************************************************************/
void motorCmdHelp(UART_DEV *huartDev){
	//help@motorCmd
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"ReportNo. \"[RPT3000]\"\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"motor commands:\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t%name.setup()\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t%name.set_abs_pos(%val)\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t%name.set_mark_pos(%val)\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t%name.set_acc(%val)\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t%name.set_dec(%val)\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t%name.set_max_spd(%val)\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t%name.set_min_spd(%val)\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t%name.set_fs_spd(%val)\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t%name.set_int_spd(%val)\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t%name.set_config(%val)\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t%name.set_status(%val)\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t%name.get_abs_pos()\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t%name.get_mark_pos()\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t%name.get_acc()\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t%name.get_dec()\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t%name.get_max_spd()\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t%name.get_min_spd()\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t%name.get_fs_spd()\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t%name.get_int_spd()\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t%name.get_config()\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t%name.get_status()\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t%name.run(fwd/rev,spd)\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t%name.move(fwd/rev,%nstep)\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t%name.go_to(%absPos)\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t%name.go_to_dir(fwd/rev,absPos)\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t%name.go_until(cpy/rst,fwd/rev,speed)\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t%name.release_sw(cpy/rst,fwd/rev) \r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t%name.go_home()\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t%name.go_mark()\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t%name.reset_pos()\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t%name.soft_stop()\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t%name.hard_stop()\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t%name.soft_hiz()\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t%name.hard_hiz()\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t%name.get_status()\r\n");
//	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t%name.reportall()\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t%name.enablerptfew()\r\n");
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t%name.disablerptfew()\r\n");
}

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
void motorReportFew(UART_DEV *huartDev, cSPIN_DEV_T *pDev, const u8* RtpNo){
	u32 x32;	u16 x16;	u8 x8;
	cSPIN_Rsrc_T* pRsrc = &pDev->rsrc;

	while(1){
		//打印
		huartDev->PrintStr(&huartDev->Rsrc, RtpNo);
		//		u32 ABS_POS;
		if(pDev->RegGetAbsPos(pRsrc, &x32) == CMD_FAIL){
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err@report few\r\n");
			break;
		}
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%ABS_POS=");
		huartDev->PrintHex32(&huartDev->Rsrc, x32);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t");
		//		u32 SPEED;
		if(pDev->RegGetSpeed(pRsrc, &x32) == CMD_FAIL){
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err@report few\r\n");
			break;
		}
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%SPEED=");
		huartDev->PrintHex32(&huartDev->Rsrc, x32);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t");
		//		u8  ADC_OUT;
		if(pDev->RegGetAdcOut(pRsrc, &x8) == CMD_FAIL){
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err@report few\r\n");
			break;
		}
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%ADC_OUT=");
		huartDev->PrintHex8(&huartDev->Rsrc, x8);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t");
		//		u16 STATUS;
		x16 = pDev->Get_Status(pRsrc);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%STATUS=");
		huartDev->PrintHex16(&huartDev->Rsrc, x16);	
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\t");
		
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");			
		break;
	}
}


/*******************************************************************************
* Function Name  : motorCmd 
* Description    : to branch
* Input          : - huart : using this uart to print
									: - cmdStr : the command string
* Output         : None
* Return         : None
*******************************************************************************/
u8 motorCmd(UART_DEV *huartDev, const u8* cmdStr, cSPIN_DEV_T* pDev, u8 count){
	u8 i,j,k;
	
	for(i=0; i<count; i++){
		for(j=0,k=0; j<16; j++){
			if(pDev[i].name[j] == 0)	break;
			else{
				if(pDev[i].name[j] == cmdStr[j])	continue;
				else{	k =1; break;}
			}
		}
		if(k)	continue;
		if(cmdStr[j++] != '.' )	continue;	
		return (motorCmdX1(huartDev, cmdStr, &pDev[i]));
	}
	return 0;
}

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
u8 motorCmdX1(UART_DEV *huartDev, const u8* cmdStr, cSPIN_DEV_T* pDev){
	u8 i,j,conditions,x8;
	u16 x16;
	u32 val,x32;
	
	cSPIN_Rsrc_T* pRsrc = &pDev->rsrc;
	/***********************************************************
	@	seek to the end of name
	@	
	************************************************************/
	for(j=0; j<16; j++){
		if(pDev->name[j] == 0)	break;
		else{
			if(pDev->name[j] == cmdStr[j])	continue;
			else	return 0;
		}
	}
	if(cmdStr[j++] != '.')	return 0;


	/***********************************************************
	@	%name.setup()
	************************************************************/	
	if(strNCmp(&cmdStr[j],(const u8*)"setup()",7)==0 ){
		cSPIN_Setup(pDev);	
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");	
		huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".setup()\r\n");
		return 1;
	}

	/***********************************************************
	@	_set_param() Group
	************************************************************/
	/***********************************************************
	@	motor[*].set_abs_pos(0xFFFFFFFF)
	@ val:[-power(2,21),power(2,21)-1]
	************************************************************/
	conditions = 0;
	if(strNCmp(&cmdStr[j], (const u8*)"set_abs_pos(0x",14)==0 )	conditions++;
	i=firstHexFrStrN(&cmdStr[j+14], &val, 8);	if(i)							conditions++;
	if(strNCmp(&cmdStr[j+14+i], (const u8*)")",1)==0 )					conditions++;
	if(conditions == 3){
		if(pDev->RegSetAbsPos(pRsrc, val) == CMD_PASS)
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
		else
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");		
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)cmdStr);		
		return 1;
	}
	
	/***********************************************************
	@	%name.get_abs_pos()
	@ return	:
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"get_abs_pos()",13)==0 ){
		if(pDev->RegGetAbsPos(pRsrc, &x32) == CMD_PASS){
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
			huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".get_abs_pos()\t%ABS_POS=");
			huartDev->PrintHex32(&huartDev->Rsrc, x32);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");		
		}
		else{
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");		
			huartDev->PrintStr(&huartDev->Rsrc, cmdStr);
		}
		return 1;
	}	
	
	/***********************************************************
	@	%name.set_mark_pos(%val)
	@ val:[-power(2,21),power(2,21)-1]
	************************************************************/
	conditions = 0;
	if(strNCmp(&cmdStr[j],(const u8*)"set_mark_pos(0x",15)==0 )	conditions++;
	i = firstHexFrStrN(&cmdStr[j+15], &val,8);	if(i)						conditions++;
	if(strNCmp((u8*)&cmdStr[j+15+i],(const u8*)")",1)==0 )			conditions++;
	if(conditions==3){
		if(pDev->RegSetMarkPos(pRsrc, val) == CMD_PASS)
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
		else
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");		
		huartDev->PrintStr(&huartDev->Rsrc, cmdStr);		
		return 1;
	}

	/***********************************************************
	@	%name.get_mark_pos()
	@ return	:
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"get_mark_pos()",14)==0 ){
		if(pDev->RegGetMarkPos(pRsrc, &x32) == CMD_PASS){
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
			huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".get_mark_pos()\t");
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%MARK=");
			huartDev->PrintHex32(&huartDev->Rsrc, x32);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");	
		}
		else{
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");		
			huartDev->PrintStr(&huartDev->Rsrc, cmdStr);
		}
		return 1;
	}

	/***********************************************************
	@	%name.motor[*].set_acc(%val)
	@ val:in uStep
	************************************************************/
	conditions = 0;
	if(strNCmp((u8*)&cmdStr[j],(const u8*)"set_acc(0x",10)==0 )	conditions++;
	i = firstHexFrStrN(&cmdStr[j+10], &val, 4);	if(i)						conditions++;
	if(strNCmp((u8*)&cmdStr[j+10+i],(const u8*)")",1)==0 )			conditions++;
	if(conditions == 3){
		if(pDev->RegSetAcc(pRsrc,(u16)val) == CMD_PASS)
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
		else
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");
		huartDev->PrintStr(&huartDev->Rsrc, cmdStr);	
		return 1;
	}
	
	/***********************************************************
	@	%name.get_acc()
	@ return	:
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"get_acc()",9)==0 ){
		if(pDev->RegGetAcc(pRsrc, &x16) == CMD_PASS){
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
			huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".get_acc()\t");
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%ACC=");
			huartDev->PrintHex16(&huartDev->Rsrc, x16);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");	
		}
		else{
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");
			huartDev->PrintStr(&huartDev->Rsrc, cmdStr);
		}
		return 1;
	}
	
	/***********************************************************
	@	%name.set_dec(%val)
	@ val:in uStep
	************************************************************/
	conditions = 0;
	if(strNCmp((u8*)&cmdStr[j],(const u8*)"set_dec(0x",10)==0 )	conditions++;
	i = firstHexFrStrN(&cmdStr[j+10], &val, 4);	if(i)						conditions++;
	if(strNCmp((u8*)&cmdStr[j+10+i],(const u8*)")",1)==0 )			conditions++;
	if(conditions == 3){
		if(pDev->RegSetDec(pRsrc,(u16)val) == CMD_PASS)
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
		else
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");
		huartDev->PrintStr(&huartDev->Rsrc, cmdStr);
		return 1;
	}

	/***********************************************************
	@	%name.get_dec()
	@ return	:
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"get_dec()",9)==0 ){
		if(pDev->RegGetDec(pRsrc, &x16) == CMD_PASS){
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
			huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".get_dec()\t");
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%DEC=");
			huartDev->PrintHex16(&huartDev->Rsrc, x16);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");	
		}
		else{
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");		
			huartDev->PrintStr(&huartDev->Rsrc, cmdStr);
		}
		return 1;
	}	

	/***********************************************************
	@	%name.set_max_spd(%val)
	@ val:in uStep
	************************************************************/
	conditions = 0;
	if(strNCmp((u8*)&cmdStr[j],(const u8*)"set_max_spd(0x",14)==0 )	conditions++;
	i=firstHexFrStrN(&cmdStr[j+14], &val, 4);	if(i)									conditions++;
	if(strNCmp((u8*)&cmdStr[j+14+i],(const u8*)")",1)==0 )					conditions++;
	if(conditions == 3){
		if(pDev->RegSetMaxSpd(pRsrc,(u16)val) == CMD_PASS)
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
		else
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");
		huartDev->PrintStr(&huartDev->Rsrc, cmdStr);	
		return 1;
	}

	/***********************************************************
	@	%name.get_max_spd()
	@ return	:
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"get_max_spd()",13)==0 ){
		if(pDev->RegGetMaxSpd(pRsrc, &x16) == CMD_PASS){
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
			huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".get_max_spd()\t");
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%MAX_SPEED=");
			huartDev->PrintHex16(&huartDev->Rsrc, x16);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");
		}
		else{
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");		
			huartDev->PrintStr(&huartDev->Rsrc, cmdStr);
		}
		return 1;
	}

	/***********************************************************
	@	%name.set_min_spd(%val)
	@ val:in uStep/64
	************************************************************/
	conditions = 0;
	if(strNCmp((u8*)&cmdStr[j],(const u8*)"set_min_spd(0x",14)==0 )	conditions++;
	i=firstHexFrStrN(&cmdStr[j+14], &val, 4);	if(i)									conditions++;
	if(strNCmp((u8*)&cmdStr[j+14+i],(const u8*)")",1)==0 )					conditions++;
	if(conditions == 3){
		if(pDev->RegSetMinSpd(pRsrc,(u16)val) == CMD_PASS)
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
		else
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");
		huartDev->PrintStr(&huartDev->Rsrc, cmdStr);	
		return 1;
	}

	/***********************************************************
	@	%name.get_min_spd()
	@ return	:
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"get_min_spd()",13)==0 ){
		if(pDev->RegGetMinSpd(pRsrc,&x16) == CMD_PASS){
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
			huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".get_min_spd()\t");
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%MIN_SPEED=");
			huartDev->PrintHex16(&huartDev->Rsrc, x16);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");	
		}
		else{
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");		
			huartDev->PrintStr(&huartDev->Rsrc, cmdStr);
		}
		return 1;
	}

	/***********************************************************
	@	%name.SetLSpdOpt(%b)
	@ val:in 
	************************************************************/
	conditions = 0;
	if(strNCmp(&cmdStr[j],(const u8*)"setlspdopt(0x",13)==0 )	conditions++;
	i=firstHexFrStrN(&cmdStr[j+13], &val,2);	if(i)						conditions++;
	if(strNCmp(&cmdStr[j+13+i],(const u8*)")",1)==0 )					conditions++;
	if(conditions == 3){
		if(pDev->RegSetLowSpdOpt(pRsrc,(bool)val) == CMD_PASS)
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
		else
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");
		huartDev->PrintStr(&huartDev->Rsrc, cmdStr);	
		return 1;
	}

	/***********************************************************
	@	%name.GetLSpdOpt()
	@ val:in 
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"getlspdopt()",12)==0 ){
		if(pDev->RegGetLowSpdOpt(pRsrc,&x8) == CMD_PASS){
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
			huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".GetLSpdOpt()\t");
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%LOW_SPD_OPT=");
			huartDev->PrintHex8(&huartDev->Rsrc, x8);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");			
		}
		else{
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");		
			huartDev->PrintStr(&huartDev->Rsrc, cmdStr);
		}
		return 1;
	}

	/***********************************************************
	@	%name.set_fs_spd(%val)
	@ val:in uStep
	************************************************************/
	conditions = 0;
	if(strNCmp((u8*)&cmdStr[j],(const u8*)"set_fs_spd(0x",13)==0 )	conditions++;
	i = firstHexFrStrN(&cmdStr[j+13], &val, 4);	if(i)								conditions++;
	if(strNCmp((u8*)&cmdStr[j+13+i],(const u8*)")",1)==0 )					conditions++;
	if(conditions == 3){
		if(pDev->RegSetFSSpd(pRsrc,(u16)val) == CMD_PASS)
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
		else
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");
		huartDev->PrintStr(&huartDev->Rsrc, cmdStr);	
		return 1;
	}
	
	/***********************************************************
	@	%name.get_fs_spd()
	@ return	:
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"get_fs_spd()",12)==0 ){
		if(pDev->RegGetFSSpd(pRsrc, &x16) == CMD_PASS){
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
			huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".get_fs_spd()\t");
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%FS_SPD=");
			huartDev->PrintHex16(&huartDev->Rsrc, x16);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");		
		}
		else{
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");		
			huartDev->PrintStr(&huartDev->Rsrc, cmdStr);
		}
		return 1;
	}

	/***********************************************************
	@	%name.set_boost_mode(%val)
	@ val:1:enable
	************************************************************/
	conditions = 0;
	if(strNCmp((u8*)&cmdStr[j],(const u8*)"set_boost_mode(0x",17)==0 )	conditions++;
	i=firstHexFrStrN(&cmdStr[j+17], &val,2);	if(i)											conditions++;
	if(strNCmp((u8*)&cmdStr[j+17+i],(const u8*)")",1)==0 )							conditions++;
	if(conditions == 3){
		if(pDev->RegSetBoostMode(pRsrc,(bool)val) == CMD_PASS)
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
		else
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");
		huartDev->PrintStr(&huartDev->Rsrc, cmdStr);	
		return 1;
	}

	/***********************************************************
	@	%name.get_boost_mode()
	@ return	:
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"get_boost_mode()",16)==0 ){
		if(pDev->RegGetBoostMode(pRsrc, &x8) == CMD_PASS){
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
			huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".get_fs_spd()\t");
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%BOOST_MODE=");
			huartDev->PrintHex8(&huartDev->Rsrc, x8);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");		
		}
		else{
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");		
			huartDev->PrintStr(&huartDev->Rsrc, cmdStr);
		}
		return 1;
	}

	/***********************************************************
	@	%name.setAccDuty(u8)
	@ return	:
	************************************************************/
	conditions = 0;
	if(strNCmp(&cmdStr[j],(const u8*)"setaccduty(0x",13)==0 )	conditions++;
	i=firstHexFrStrN(&cmdStr[j+13], &val,2);	if(i)						conditions++;
	if(strNCmp(&cmdStr[j+13+i],(const u8*)")",1)==0 )					conditions++;
	if(conditions == 3){
		if(pDev->RegSetKValAcc(pRsrc,(u8)val) == CMD_PASS)
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
		else
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");
		huartDev->PrintStr(&huartDev->Rsrc, cmdStr);	
		return 1;
	}

	/***********************************************************
	@	%name.getAccDuty()
	@ return	:
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"getaccduty()",12)==0 ){
		if(pDev->RegGetKValAcc(pRsrc,&x8)==CMD_PASS){
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
			huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".getAccDuty()\t");
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%KVAL_ACC=");
			huartDev->PrintHex8(&huartDev->Rsrc, x8);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");		
		}
		else{
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");		
			huartDev->PrintStr(&huartDev->Rsrc, cmdStr);
		}
		return 1;
	}

	/***********************************************************
	@	%name.setDecDuty(u8)
	@ return	:
	************************************************************/
	conditions = 0;
	if(strNCmp(&cmdStr[j],(const u8*)"setdecduty(0x",13)==0 )	conditions++;
	i=firstHexFrStrN(&cmdStr[j+13], &val, 2);	if(i)						conditions++;
	if(strNCmp(&cmdStr[j+13+i],(const u8*)")",1)==0 )					conditions++;
	if(conditions == 3){
		if(pDev->RegSetKValDec(pRsrc,(u8)val) == CMD_PASS)
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
		else
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");
		huartDev->PrintStr(&huartDev->Rsrc, cmdStr);	
		return 1;
	}

	/***********************************************************
	@	%name.getDecDuty()
	@ return	:
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"getdecduty()",12)==0 ){
		if(pDev->RegGetKValDec(pRsrc, &x8)==CMD_PASS){
			pRsrc->reg.KVAL_DEC = pDev->Get_Param(pRsrc, cSPIN_KVAL_DEC);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
			huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".getDecDuty()\t");
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%KVAL_DEC=");
			huartDev->PrintHex8(&huartDev->Rsrc, x8);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");		
		}
		else{
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");		
			huartDev->PrintStr(&huartDev->Rsrc, cmdStr);
		}
		return 1;
	}

	/***********************************************************
	@	%name.setRunDuty(u8)
	@ return	:
	************************************************************/
	conditions = 0;
	if(strNCmp(&cmdStr[j],(const u8*)"setrunduty(0x",13)==0 )	conditions++;
	i=firstHexFrStrN(&cmdStr[j+13], &val, 2);	if(i)						conditions++;
	if(strNCmp(&cmdStr[j+13+i],(const u8*)")",1)==0 )					conditions++;
	if(conditions == 3){
		if(pDev->RegSetKValRun(pRsrc,(u8)val) == CMD_PASS)
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
		else
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");
		huartDev->PrintStr(&huartDev->Rsrc, cmdStr);	
		return 1;
	}

	/***********************************************************
	@	%name.getRunDuty()
	@ return	:
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"getrunduty()",12)==0 ){
		if(pDev->RegGetKValRun(pRsrc,&x8)==CMD_PASS){
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
			huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".getRunDuty()\t");
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%KVAL_RUN=");
			huartDev->PrintHex8(&huartDev->Rsrc, x8);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");		
		}
		else{
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");		
			huartDev->PrintStr(&huartDev->Rsrc, cmdStr);
		}
		return 1;
	}

	/***********************************************************
	@	%name.setHoldDuty(u8)
	@ return	:
	************************************************************/
	conditions = 0;
	if(strNCmp(&cmdStr[j],(const u8*)"setholdduty(0x",14)==0 )	conditions++;
	i=firstHexFrStrN(&cmdStr[j+14], &val,2);	if(i)							conditions++;
	if(strNCmp(&cmdStr[j+14+i],(const u8*)")",1)==0 )						conditions++;
	if(conditions == 3){
		if(pDev->RegSetKValHold(pRsrc,(u8)val) == CMD_PASS)
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
		else
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");
		huartDev->PrintStr(&huartDev->Rsrc, cmdStr);	
		return 1;
	}

	/***********************************************************
	@	%name.getHoldDuty()
	@ return	:
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"getholdduty()",12)==0 ){
		if(pDev->RegGetKValHold(pRsrc,&x8) == CMD_PASS){
			pRsrc->reg.KVAL_HOLD = pDev->Get_Param(pRsrc, cSPIN_KVAL_HOLD);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
			huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".getHoldDuty()\t");
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%KVAL_HOLD=");
			huartDev->PrintHex8(&huartDev->Rsrc, x8);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");		
		}
		else{
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");		
			huartDev->PrintStr(&huartDev->Rsrc, cmdStr);
		}
		return 1;
	}

	/***********************************************************
	@	%name.set_int_spd(%val)
	@ val:in uStep
	************************************************************/
	conditions = 0;
	if(strNCmp(&cmdStr[j],(const u8*)"set_int_spd(0x",14)==0 )	conditions++;
	i=firstHexFrStrN(&cmdStr[j+14], &val,4);	if(i)							conditions++;
	if(strNCmp(&cmdStr[j+14+i],(const u8*)")",1)==0 )						conditions++;
	if(conditions == 3){
		if(pDev->RegSetIntSpd(pRsrc,(u16)val) == CMD_PASS)
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
		else
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");
		huartDev->PrintStr(&huartDev->Rsrc, cmdStr);	
		return 1;
	}

	/***********************************************************
	@	%name.get_int_spd()
	@ return	:
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"get_int_spd()",13)==0 ){
		if(pDev->RegGetIntSpd(pRsrc,&x16) == CMD_PASS){
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
			huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".get_int_spd()\t");
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%INT_SPD=");
			huartDev->PrintHex16(&huartDev->Rsrc, x16);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");		
		}
		else{
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");
			huartDev->PrintStr(&huartDev->Rsrc, cmdStr);
		}
		return 1;
	}

	/***********************************************************
	@	%name.setStartSlope(%val)
	@ val:in u*s/ustep
	************************************************************/
	conditions = 0;
	if(strNCmp(&cmdStr[j],(const u8*)"setstartslope(0x",16)==0 )	conditions++;
	i=firstHexFrStrN(&cmdStr[j+16], &val,2);	if(i)								conditions++;
	if(strNCmp(&cmdStr[j+16+i],(const u8*)")",1)==0 )							conditions++;
	if(conditions == 3){
		if(pDev->RegSetStartSlp(pRsrc,(u8)val) == CMD_PASS)
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
		else
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");
		huartDev->PrintStr(&huartDev->Rsrc, cmdStr);	
		return 1;
	}

	/***********************************************************
	@	%name.getstartslope()
	@ return	:
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"getstartslope()",15)==0 ){
		if(pDev->RegGetStartSlp(pRsrc,&x8) == CMD_PASS){
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
			huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".getStartSlope()\t");
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%ST_SLP=");
			huartDev->PrintHex8(&huartDev->Rsrc, x8);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");		
		}
		else{
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");		
			huartDev->PrintStr(&huartDev->Rsrc, cmdStr);
		}
		return 1;
	}

	/***********************************************************
	@	%name.setAccFinalSlope(%val)
	@ val:in u*s/ustep
	************************************************************/
	conditions = 0;
	if(strNCmp(&cmdStr[j],(const u8*)"setaccfinalslope(0x",19)==0 )	conditions++;
	i=firstHexFrStrN(&cmdStr[j+19], &val,2);	if(i)									conditions++;
	if(strNCmp(&cmdStr[j+19+i],(const u8*)")",1)==0 )								conditions++;
	if(conditions == 3){
		if(pDev->RegSetFnSlpAcc(pRsrc,(u8)val) == CMD_PASS)
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
		else
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");
		huartDev->PrintStr(&huartDev->Rsrc, cmdStr);	
		return 1;
	}

	/***********************************************************
	@	%name.getAccFinalSlope()
	@ return	:
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"getaccfinalslope()",18)==0 ){
		if(pDev->RegGetFnSlpAcc(pRsrc,&x8) == CMD_PASS){
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
			huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".getAccFinalSlope()\t");
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%FN_SLP_ACC=");
			huartDev->PrintHex8(&huartDev->Rsrc, x8);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");		
		}
		else{
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");		
			huartDev->PrintStr(&huartDev->Rsrc, cmdStr);
		}
		return 1;
	}

	/***********************************************************
	@	%name.setDecFinalSlope(%val)
	@ val:in u*s/ustep
	************************************************************/
	conditions = 0;
	if(strNCmp(&cmdStr[j],(const u8*)"setdecfinalslope(0x",19)==0 )	conditions++;
	i=firstHexFrStrN(&cmdStr[j+19], &val, 2);	if(i)									conditions++;
	if(strNCmp(&cmdStr[j+19+i],(const u8*)")",1)==0 )								conditions++;
	if(conditions == 3){
		if(pDev->RegSetFnSlpDec(pRsrc,(u8)val) == CMD_PASS)
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
		else
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");
		huartDev->PrintStr(&huartDev->Rsrc, cmdStr);	
		return 1;
	}

	/***********************************************************
	@	%name.getDecFinalSlope()
	@ return	:
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"getdecfinalslope()",18)==0 ){
		if(pDev->RegGetFnSlpDec(pRsrc,&x8) == CMD_PASS){
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
			huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".getDecFinalSlope()\t");
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%FN_SLP_DEC=");
			huartDev->PrintHex8(&huartDev->Rsrc, x8);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");
		}
		else{
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");		
			huartDev->PrintStr(&huartDev->Rsrc, cmdStr);
		}
		return 1;
	}

	/***********************************************************
	@	%name.Set_K_THERM(%hex)
	@ val:in 
	************************************************************/
	conditions = 0;
	if(strNCmp(&cmdStr[j],(const u8*)"set_k_therm(0x",14)==0 )	conditions++;
	i=firstHexFrStrN(&cmdStr[j+14], &val, 2);	if(i)							conditions++;
	if(strNCmp(&cmdStr[j+14+i],(const u8*)")",1)==0 )						conditions++;
	if(conditions == 3){
		if(pDev->RegSetKTherm(pRsrc,(u8)val) == CMD_PASS)
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
		else
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");
		huartDev->PrintStr(&huartDev->Rsrc, cmdStr);	
		return 1;
	}

	/***********************************************************
	@	%name.Get_K_THERM()
	@ val:in 
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"get_k_therm()",13)==0 ){
		if(pDev->RegGetKTherm(pRsrc,&x8) == CMD_PASS){
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
			huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".Get_K_THERM()\t");
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%K_THERM=");
			huartDev->PrintHex8(&huartDev->Rsrc, x8);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");
		}
		else{
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");		
			huartDev->PrintStr(&huartDev->Rsrc, cmdStr);
		}
		return 1;
	}

	/***********************************************************
	@	%name.Set_OCD_TH(%hex)
	@ val:in 
	************************************************************/
	conditions = 0;
	if(strNCmp(&cmdStr[j],(const u8*)"set_ocd_th(0x",13)==0 )	conditions++;
	i=firstHexFrStrN(&cmdStr[j+13], &val, 2);	if(i)						conditions++;
	if(strNCmp(&cmdStr[j+13+i],(const u8*)")",1)==0 )					conditions++;
	if(conditions == 3){
		if(pDev->RegSetOCDTh(pRsrc,(u8)val) == CMD_PASS)
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
		else
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");
		huartDev->PrintStr(&huartDev->Rsrc, cmdStr);	
		return 1;
	}

	/***********************************************************
	@	%name.Get_OCD_TH()
	@ val:in 
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"get_ocd_th()",12)==0 ){
		if(pDev->RegGetOCDTh(pRsrc,&x8) == CMD_PASS){
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
			huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".Get_OCD_TH()\t");
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%OCD_TH=");
			huartDev->PrintHex8(&huartDev->Rsrc, x8);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");
		}
		else{
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");		
			huartDev->PrintStr(&huartDev->Rsrc, cmdStr);
		}
		return 1;
	}

	/***********************************************************
	@	%name.Set_Stall_Th(%hex)
	@ val:in 
	************************************************************/
	conditions = 0;
	if(strNCmp(&cmdStr[j],(const u8*)"set_stall_th(0x",15)==0 )	conditions++;
	i=firstHexFrStrN(&cmdStr[j+15], &val, 2);	if(i)							conditions++;
	if(strNCmp(&cmdStr[j+15+i],(const u8*)")",1)==0 )						conditions++;
	if(conditions == 3){
		if(pDev->RegSetStallTh(pRsrc,(u8)val) == CMD_PASS)
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
		else
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");
		huartDev->PrintStr(&huartDev->Rsrc, cmdStr);	
		return 1;
	}

	/***********************************************************
	@	%name.Get_Stall_Th()
	@ val:in 
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"get_stall_th()",14)==0 ){
		if(pDev->RegGetStallTh(pRsrc,&x8) == CMD_PASS){
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
			huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".Get_Stall_Th()\t");
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%STALL_TH=");
			huartDev->PrintHex8(&huartDev->Rsrc, x8);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");
		}
		else{
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");		
			huartDev->PrintStr(&huartDev->Rsrc, cmdStr);
		}
		return 1;
	}

	/***********************************************************
	@	%name.SetStepSel(%hex)
	@ val:
	************************************************************/
	conditions = 0;
	if(strNCmp(&cmdStr[j],(const u8*)"setstepsel(0x",13)==0 )	conditions++;
	i=firstHexFrStrN(&cmdStr[j+13], &val, 2);	if(i)						conditions++;
	if(strNCmp(&cmdStr[j+13+i],(const u8*)")",1)==0 )					conditions++;
	if(conditions == 3){
		if(pDev->RegSetStepSel(pRsrc, (u8)val) == CMD_PASS)
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
		else
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");
		huartDev->PrintStr(&huartDev->Rsrc, cmdStr);		
		return 1;
	}

	/***********************************************************
	@	%name.GetStepSel()
	@ val:in 
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"getstepsel()",12)==0 ){
		if(pDev->RegGetStepSel(pRsrc,&x8) == CMD_PASS){
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
			huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".GetStepSel()\t");
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%STEP_SEL=");
			huartDev->PrintHex8(&huartDev->Rsrc, x8);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");
		}
		else{
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");		
			huartDev->PrintStr(&huartDev->Rsrc, cmdStr);
		}
		return 1;
	}

	/***********************************************************
	@	%name.SetSyncSel(%hex)
	@ val:
	************************************************************/
	conditions = 0;
	if(strNCmp(&cmdStr[j],(const u8*)"setsyncsel(0x",13)==0 )	conditions++;
	i=firstHexFrStrN(&cmdStr[j+13], &val, 2);	if(i)						conditions++;
	if(strNCmp(&cmdStr[j+13+i],(const u8*)")",1)==0 )					conditions++;
	if(conditions == 3){
		if(pDev->RegSetSyncSel(pRsrc, (u8)val) == CMD_PASS)
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
		else
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");
		huartDev->PrintStr(&huartDev->Rsrc, cmdStr);		
		return 1;
	}

	/***********************************************************
	@	%name.GetSyncSel()
	@ val:
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"getsyncsel()",12)==0 ){
		if(pDev->RegGetSyncSel(pRsrc,&x8) == CMD_PASS){
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
			huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".GetSyncSel()\t");
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%SYNC_SEL=");
			huartDev->PrintHex8(&huartDev->Rsrc, x8);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");
		}
		else{
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");		
			huartDev->PrintStr(&huartDev->Rsrc, cmdStr);
		}
		return 1;
	}
	/***********************************************************
	@	%name.SetSyncEn(%hex)
	@ 
	************************************************************/
	conditions = 0;
	if(strNCmp(&cmdStr[j],(const u8*)"setsyncen(0x",12)==0 )	conditions++;
	i=firstHexFrStrN(&cmdStr[j+12], &val, 2);	if(i)						conditions++;
	if(strNCmp(&cmdStr[j+12+i],(const u8*)")",1)==0 )					conditions++;
	if(conditions == 3){
		if(pDev->RegSetSyncEn(pRsrc, (u8)val) == CMD_PASS)
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
		else
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");
		huartDev->PrintStr(&huartDev->Rsrc, cmdStr);		
		return 1;
	}

	/***********************************************************
	@	%name.GetSyncSelEn()
	@ val:
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"getsyncen()",11)==0 ){
		if(pDev->RegGetSyncEn(pRsrc,&x8) == CMD_PASS){
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
			huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".GetSyncEn()\t");
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%SYNC_EN=");
			huartDev->PrintHex8(&huartDev->Rsrc, x8);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");
		}
		else{
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");		
			huartDev->PrintStr(&huartDev->Rsrc, cmdStr);
		}
		return 1;
	}

	/***********************************************************
	@	%name.SetGateTcc
	@ 
	************************************************************/
	conditions = 0;
	if(strNCmp(&cmdStr[j],(const u8*)"setgatetcc(0x",13)==0 )	conditions++;
	i=firstHexFrStrN(&cmdStr[j+13], &val, 2);	if(i)						conditions++;
	if(strNCmp(&cmdStr[j+13+i],(const u8*)")",1)==0 )					conditions++;
	if(conditions == 3){
		if(pDev->RegSetGateTcc(pRsrc, (u8)val) == CMD_PASS)
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
		else
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");
		huartDev->PrintStr(&huartDev->Rsrc, cmdStr);		
		return 1;
	}
	/***********************************************************
	@	%name.GetGateTcc
	@ 
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"getgatetcc()",12)==0 ){
		if(pDev->RegGetGateTcc(pRsrc,&x8) == CMD_PASS){
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
			huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".GetGateTcc()\t");
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%GateTcc=");
			huartDev->PrintHex8(&huartDev->Rsrc, x8);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");
		}
		else{
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");		
			huartDev->PrintStr(&huartDev->Rsrc, cmdStr);
		}
		return 1;
	}

	/***********************************************************
	@	%name.SetGateI
	@ 
	************************************************************/
	conditions = 0;
	if(strNCmp(&cmdStr[j],(const u8*)"setgatei(0x",11)==0 )	conditions++;
	i=firstHexFrStrN(&cmdStr[j+11], &val, 2);	if(i)					conditions++;
	if(strNCmp(&cmdStr[j+11+i],(const u8*)")",1)==0 )				conditions++;
	if(conditions == 3){
		if(pDev->RegSetGateCurrent(pRsrc, (u8)val) == CMD_PASS)
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
		else
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");
		huartDev->PrintStr(&huartDev->Rsrc, cmdStr);		
		return 1;
	}
	/***********************************************************
	@	%name.GetGateI
	@ 
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"getgatei()",10)==0 ){
		if(pDev->RegGetGateCurrent(pRsrc,&x8) == CMD_PASS){
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
			huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".GetGateI()\t");
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%GateI=");
			huartDev->PrintHex8(&huartDev->Rsrc, x8);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");
		}
		else{
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");		
			huartDev->PrintStr(&huartDev->Rsrc, cmdStr);
		}
		return 1;
	}

	/***********************************************************
	@	%name.SetGateTboost
	@ 
	************************************************************/
	conditions = 0;
	if(strNCmp(&cmdStr[j],(const u8*)"setgatetboost(0x",16)==0 )	conditions++;
	i=firstHexFrStrN(&cmdStr[j+16], &val, 2);	if(i)						conditions++;
	if(strNCmp(&cmdStr[j+16+i],(const u8*)")",1)==0 )					conditions++;
	if(conditions == 3){
		if(pDev->RegSetGateTBoost(pRsrc, (u8)val) == CMD_PASS)
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
		else
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");
		huartDev->PrintStr(&huartDev->Rsrc, cmdStr);		
		return 1;
	}
	/***********************************************************
	@	%name.GetGateTBoost
	@ 
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"getgatetboost()",15)==0 ){
		if(pDev->RegGetGateTBoost(pRsrc,&x8) == CMD_PASS){
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
			huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".GetGateTBoost()\t");
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%GateTBoost=");
			huartDev->PrintHex8(&huartDev->Rsrc, x8);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");
		}
		else{
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");		
			huartDev->PrintStr(&huartDev->Rsrc, cmdStr);
		}
		return 1;
	}

	/***********************************************************
	@	%name.SetGateTDT
	@ 
	************************************************************/
	conditions = 0;
	if(strNCmp(&cmdStr[j],(const u8*)"setgatetdt(0x",13)==0 )	conditions++;
	i=firstHexFrStrN(&cmdStr[j+13], &val, 2);	if(i)						conditions++;
	if(strNCmp(&cmdStr[j+13+i],(const u8*)")",1)==0 )					conditions++;
	if(conditions == 3){
		if(pDev->RegSetGateTDeadTime(pRsrc, (u8)val) == CMD_PASS)
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
		else
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");
		huartDev->PrintStr(&huartDev->Rsrc, cmdStr);		
		return 1;
	}
	/***********************************************************
	@	%name.GetGateTDT
	@ 
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"getgatetdt()",12)==0 ){
		if(pDev->RegGetGateTDeadTime(pRsrc,&x8) == CMD_PASS){
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
			huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".GetGateTDT()\t");
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%GateTDT=");
			huartDev->PrintHex8(&huartDev->Rsrc, x8);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");
		}
		else{
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");		
			huartDev->PrintStr(&huartDev->Rsrc, cmdStr);
		}
		return 1;
	}

	/***********************************************************
	@	%name.SetGateTBlank
	@ 
	************************************************************/
	conditions = 0;
	if(strNCmp(&cmdStr[j],(const u8*)"setgatetblank(0x",16)==0 )	conditions++;
	i=firstHexFrStrN(&cmdStr[j+16], &val, 2);	if(i)						conditions++;
	if(strNCmp(&cmdStr[j+16+i],(const u8*)")",1)==0 )					conditions++;
	if(conditions == 3){
		if(pDev->RegSetGateTBlank(pRsrc, (u8)val) == CMD_PASS)
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
		else
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");
		huartDev->PrintStr(&huartDev->Rsrc, cmdStr);		
		return 1;
	}
	/***********************************************************
	@	%name.GetGateTBlank
	@ 
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"getgatetblank()",15)==0 ){
		if(pDev->RegGetGateTBlank(pRsrc,&x8) == CMD_PASS){
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
			huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".GetGateTBlank()\t");
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%GateTBlank=");
			huartDev->PrintHex8(&huartDev->Rsrc, x8);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");
		}
		else{
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");		
			huartDev->PrintStr(&huartDev->Rsrc, cmdStr);
		}
		return 1;
	}
	
	/***********************************************************
	@	%name.SetSwMode
	@ 
	************************************************************/
	conditions = 0;
	if(strNCmp(&cmdStr[j],(const u8*)"setswmode(0x",12)==0 )	conditions++;
	i=firstHexFrStrN(&cmdStr[j+12], &val, 2);	if(i)						conditions++;
	if(strNCmp(&cmdStr[j+12+i],(const u8*)")",1)==0 )					conditions++;
	if(conditions == 3){
		if(pDev->RegSetSwMode(pRsrc, (u8)val) == CMD_PASS)
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
		else
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");
		huartDev->PrintStr(&huartDev->Rsrc, cmdStr);		
		return 1;
	}
	
	/***********************************************************
	@	%name.GetSwMode
	@ 
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"getswmode()",11)==0 ){
		if(pDev->RegGetSwMode(pRsrc,&x8) == CMD_PASS){
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
			huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".GetSwMode()\t");
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%SwMode=");
			huartDev->PrintHex8(&huartDev->Rsrc, x8);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");
		}
		else{
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");		
			huartDev->PrintStr(&huartDev->Rsrc, cmdStr);
		}
		return 1;
	}
	
	/***********************************************************
	@	%name.SetVsCompEn
	@ 
	************************************************************/
	conditions = 0;
	if(strNCmp(&cmdStr[j],(const u8*)"setvscompen(0x",14)==0 )	conditions++;
	i=firstHexFrStrN(&cmdStr[j+14], &val, 2);	if(i)						conditions++;
	if(strNCmp(&cmdStr[j+14+i],(const u8*)")",1)==0 )					conditions++;
	if(conditions == 3){
		if(pDev->RegSetVsCompEn(pRsrc, (u8)val) == CMD_PASS)
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
		else
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");
		huartDev->PrintStr(&huartDev->Rsrc, cmdStr);		
		return 1;
	}
	
	/***********************************************************
	@	%name.GetVsCompEn
	@ 
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"getvscompen()",13)==0 ){
		if(pDev->RegGetVsCompEn(pRsrc,&x8) == CMD_PASS){
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
			huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".GetVsCompEn()\t");
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%VsCompEn=");
			huartDev->PrintHex8(&huartDev->Rsrc, x8);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");
		}
		else{
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");		
			huartDev->PrintStr(&huartDev->Rsrc, cmdStr);
		}
		return 1;
	}
	
	/***********************************************************
	@	%name.SetOCSDEn
	@ 
	************************************************************/
	conditions = 0;
	if(strNCmp(&cmdStr[j],(const u8*)"setocsden(0x",12)==0 )	conditions++;
	i=firstHexFrStrN(&cmdStr[j+12], &val, 2);	if(i)						conditions++;
	if(strNCmp(&cmdStr[j+12+i],(const u8*)")",1)==0 )					conditions++;
	if(conditions == 3){
		if(pDev->RegSetOCSDEn(pRsrc, (u8)val) == CMD_PASS)
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
		else
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");
		huartDev->PrintStr(&huartDev->Rsrc, cmdStr);		
		return 1;
	}
	
	/***********************************************************
	@	%name.GetOCSDEn
	@ 
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"getocsden()",11)==0 ){
		if(pDev->RegGetOCSDEn(pRsrc,&x8) == CMD_PASS){
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
			huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".GetOCSDEn()\t");
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%OCSDEn=");
			huartDev->PrintHex8(&huartDev->Rsrc, x8);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");
		}
		else{
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");		
			huartDev->PrintStr(&huartDev->Rsrc, cmdStr);
		}
		return 1;
	}
	
	/***********************************************************
	@	%name.SetUVLOVal
	@ 
	************************************************************/
	conditions = 0;
	if(strNCmp(&cmdStr[j],(const u8*)"setuvloval(0x",13)==0 )	conditions++;
	i=firstHexFrStrN(&cmdStr[j+13], &val, 2);	if(i)						conditions++;
	if(strNCmp(&cmdStr[j+13+i],(const u8*)")",1)==0 )					conditions++;
	if(conditions == 3){
		if(pDev->RegSetUVLOVal(pRsrc, (u8)val) == CMD_PASS)
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
		else
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");
		huartDev->PrintStr(&huartDev->Rsrc, cmdStr);		
		return 1;
	}
	
	/***********************************************************
	@	%name.GetUVLOVal
	@ 
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"getuvloval()",11)==0 ){
		if(pDev->RegGetUVLOVal(pRsrc,&x8) == CMD_PASS){
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
			huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".GetUVLOVal()\t");
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%UVLOV=");
			huartDev->PrintHex8(&huartDev->Rsrc, x8);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");
		}
		else{
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");		
			huartDev->PrintStr(&huartDev->Rsrc, cmdStr);
		}
		return 1;
	}	
	
	/***********************************************************
	@	%name.SetVccVal
	@ 
	************************************************************/
	conditions = 0;
	if(strNCmp(&cmdStr[j],(const u8*)"setvccval(0x",12)==0 )	conditions++;
	i=firstHexFrStrN(&cmdStr[j+12], &val, 2);	if(i)						conditions++;
	if(strNCmp(&cmdStr[j+12+i],(const u8*)")",1)==0 )					conditions++;
	if(conditions == 3){
		if(pDev->RegSetVccVal(pRsrc, (u8)val) == CMD_PASS)
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
		else
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");
		huartDev->PrintStr(&huartDev->Rsrc, cmdStr);		
		return 1;
	}
	
	/***********************************************************
	@	%name.GetVccVal
	@ 
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"getvccval()",11)==0 ){
		if(pDev->RegGetVccVal(pRsrc,&x8) == CMD_PASS){
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
			huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".GetVccVal()\t");
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%VCC=");
			huartDev->PrintHex8(&huartDev->Rsrc, x8);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");
		}
		else{
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");		
			huartDev->PrintStr(&huartDev->Rsrc, cmdStr);
		}
		return 1;
	}	
	
	/***********************************************************
	@	%name.SetPwmFrq
	@ 
	************************************************************/
	conditions = 0;
	if(strNCmp(&cmdStr[j],(const u8*)"setpwmfrq(0x",12)==0 )	conditions++;
	i=firstHexFrStrN(&cmdStr[j+12], &val, 2);	if(i)						conditions++;
	if(strNCmp(&cmdStr[j+12+i],(const u8*)")",1)==0 )					conditions++;
	if(conditions == 3){
		if(pDev->RegSetPwmFrq(pRsrc, (u8)val) == CMD_PASS)
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
		else
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");
		huartDev->PrintStr(&huartDev->Rsrc, cmdStr);		
		return 1;
	}
	
	/***********************************************************
	@	%name.GetPwmFrq
	@ 
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"getpwmfrq()",11)==0 ){
		if(pDev->RegGetPwmFrq(pRsrc,&x8) == CMD_PASS){
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
			huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".GetPwmFrq()\t");
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%PWM_FRQ=");
			huartDev->PrintHex8(&huartDev->Rsrc, x8);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");
		}
		else{
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");		
			huartDev->PrintStr(&huartDev->Rsrc, cmdStr);
		}
		return 1;
	}	

	/***********************************************************
	@	%name.get_adc_out()
	@ return	:
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"get_adc_out()",11)==0 ){
		if(pDev->RegGetAdcOut(pRsrc,&x8)==CMD_PASS){
			pRsrc->reg.STATUS = pDev->Get_Param(pRsrc, cSPIN_STATUS);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
			huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".get_adc_out()\t");
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%ADC_OUT=");
			huartDev->PrintHex8(&huartDev->Rsrc, x8);
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");
		}
		else{
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");		
			huartDev->PrintStr(&huartDev->Rsrc, cmdStr);
		}
		return 1;
	}

	/***********************************************************
	@	%name.get_status()
	@ return	:
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"get_status()",12)==0 ){
		//x16 = pDev->Get_Param(pRsrc, cSPIN_STATUS);
		x16 = pDev->Get_Status(pRsrc);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
		huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".get_status()\t");
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%STATUS=");
		huartDev->PrintHex16(&huartDev->Rsrc, x16);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");
		return 1;
	}

	/***********************************************************
	@	%name.get_speed()
	@ return	:
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"get_speed()",11)==0 ){
		x32 = pDev->Get_Param(pRsrc, cSPIN_SPEED);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
		huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".get_speed()\t");
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"SPEED@");
		huartDev->PrintHex32(&huartDev->Rsrc, x32);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");
		return 1;
	}

	/***********************************************************
	@	%name.run(fwd/rev,spd)
	@ spd	: steps/s, 
		This command can be given anytime and is immediately executed.
	************************************************************/
	conditions = 0;
	if(strNCmp(&cmdStr[j],(const u8*)"run(fwd,",8)==0 )	conditions++;
	i=firstNumFrStr(&cmdStr[j], &val);	if(i)							conditions++;
	if(strNCmp(&cmdStr[j+i],(const u8*)")",1)==0 )			conditions++;
	if(conditions == 3){
		pDev->Run(pRsrc, FWD, Speed_Steps_to_Par(val));
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
		huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
		huartDev->PrintSDS(&huartDev->Rsrc, (const u8*)".run(fwd,", val, (const u8*)")\r\n");	
		return 1;
	}
	
	conditions = 0;
	if(strNCmp(&cmdStr[j],(const u8*)"run(rev,",8)==0 )	conditions++;
	i=firstNumFrStr(&cmdStr[j], &val);	if(i)							conditions++;
	if(strNCmp(&cmdStr[j+i],(const u8*)")",1)==0 )			conditions++;
	if(conditions == 3){
		pDev->Run(pRsrc, REV, Speed_Steps_to_Par(val));
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
		huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
		huartDev->PrintSDS(&huartDev->Rsrc, (const u8*)".run(rev,", val, (const u8*)")\r\n");	
		return 1;
	}

	/***********************************************************
	@	%name.move(fwd/rev,%nstep)
	@ spd	: steps/s, 
		This command can be given anytime and is immediately executed.
	************************************************************/
	conditions = 0;
	if(strNCmp(&cmdStr[j],(const u8*)"move(fwd,",9)==0 )	conditions++;
	i=firstNumFrStr(&cmdStr[j], &val);	if(i)							conditions++;
	if(strNCmp(&cmdStr[j+i],(const u8*)")",1)==0 )				conditions++;
	if(conditions == 3){
		pDev->Move(pRsrc, FWD, val);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
		huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
		huartDev->PrintSDS(&huartDev->Rsrc, (const u8*)".move(fwd,", val, (const u8*)")\r\n");		
		return 1;
	}
	conditions = 0;
	if(strNCmp(&cmdStr[j],(const u8*)"move(rev,",9)==0 )	conditions++;
	i=firstNumFrStr(&cmdStr[j], &val);	if(i)								conditions++;
	if(strNCmp(&cmdStr[j+i],(const u8*)")",1)==0 )				conditions++;
	if(conditions == 3){
		pDev->Move(pRsrc, REV, val);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
		huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
		huartDev->PrintSDS(&huartDev->Rsrc, (const u8*)".move(rev,", val, (const u8*)")\r\n");
		return 1;
	}
	
	/***********************************************************
	@	%name.go_to(%absPos)
	@ absPos	: 
		This command can be given anytime and is immediately executed.
	************************************************************/
	conditions = 0;
	if(strNCmp(&cmdStr[j],(const u8*)"go_to(",6)==0 )	conditions++;
	i=firstNumFrStr(&cmdStr[j], &val);	if(i)					conditions++;
	if(strNCmp(&cmdStr[j+i],(const u8*)")",1)==0 )		conditions++;
	if(conditions == 3){
		pDev->Go_To(pRsrc, val);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
		huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
		huartDev->PrintSDS(&huartDev->Rsrc, (const u8*)".go_to(", val, (const u8*)")\r\n");			
		return 1;
	}
	
	/***********************************************************
	@	%name.go_to_dir(fwd/rev,absPos)
	@ spd	: steps/s, 
		This command can be given anytime and is immediately executed.
	************************************************************/
	conditions = 0;
	if(strNCmp(&cmdStr[j],(const u8*)"go_to_dir(fwd,",14)==0 )	conditions++;
	i=firstNumFrStr(&cmdStr[j], &val);	if(i)										conditions++;
	if(strNCmp(&cmdStr[j+i],(const u8*)")",1)==0 )							conditions++;
	if(conditions == 3){
		pDev->Go_To_Dir(pRsrc, FWD, val);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
		huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
		huartDev->PrintSDS(&huartDev->Rsrc, (const u8*)".go_to_dir(fwd,", val, (const u8*)")\r\n");			
		return 1;
	}
	conditions = 0;
	if(strNCmp(&cmdStr[j],(const u8*)"go_to_dir(rev,",14)==0 )	conditions++;
	i=firstNumFrStr(&cmdStr[j], &val);	if(i)										conditions++;
	if(strNCmp(&cmdStr[j+i],(const u8*)")",1)==0 )							conditions++;
	if(conditions == 3){
		pDev->Go_To_Dir(pRsrc, REV, val);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
		huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
		huartDev->PrintSDS(&huartDev->Rsrc, (const u8*)"go_to_dir(rev,", val, (const u8*)")\r\n");			
		return 1;
	}
	
	/***********************************************************
	@	%name.gotoAbs(%absPos)
	@ absPos	: 
		This command can be given anytime and is immediately executed.
	************************************************************/
	conditions = 0;
	if(strNCmp(&cmdStr[j],(const u8*)"gotoAbs(",8)==0 )	conditions++;
	i=firstNumFrStr(&cmdStr[j], &val);	if(i)					conditions++;
	if(strNCmp(&cmdStr[j+i],(const u8*)")",1)==0 )		conditions++;
	if(conditions == 3){
		if(pDev->RegGetAbsPos(&pDev->rsrc,&x32) == CMD_FAIL){
			huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+err @");		
			huartDev->PrintStr(&huartDev->Rsrc, cmdStr);
			return 1;
		}
		if(x32 > val)
			pDev->Go_To_Dir(pRsrc, FWD, val);
		if(x32 < val)
			pDev->Go_To_Dir(pRsrc, REV, val);		
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
		huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
		huartDev->PrintSDS(&huartDev->Rsrc, (const u8*)".go_to(", val, (const u8*)")\r\n");			
		return 1;
	}
	
	
	/***********************************************************
	@	%name.go_until(cpy/rst,fwd/rev,speed)
	@ spd	: steps/s, 
		This command can be given anytime and is immediately executed.
	************************************************************/
	conditions = 0;
	if(strNCmp(&cmdStr[j],(const u8*)"go_until(rst,fwd,",17)==0 )	conditions++;
	i=firstNumFrStr(&cmdStr[j], &val);	if(i)											conditions++;
	if(strNCmp(&cmdStr[j+i],(const u8*)")",1)==0 )								conditions++;
	if(conditions == 3){
		pDev->Go_Until(pRsrc, ACTION_RESET, FWD, Speed_Steps_to_Par(val));
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
		huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
		huartDev->PrintSDS(&huartDev->Rsrc, (const u8*)"go_until(rst,fwd,", val, (const u8*)")\r\n");			
		return 1;
	}
	conditions = 0;
	if(strNCmp(&cmdStr[j], (const u8*)"go_until(rst,rev,",17)==0 )	conditions++;
	i=firstNumFrStr(&cmdStr[j], &val);	if(i)												conditions++;
	if(strNCmp(&cmdStr[j+i],(const u8*)")",1)==0 )									conditions++;
	if(conditions == 3){
		pDev->Go_Until(pRsrc, ACTION_RESET, REV, Speed_Steps_to_Par(val));
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
		huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
		huartDev->PrintSDS(&huartDev->Rsrc, (const u8*)".go_until(rst,rev,", val, (const u8*)")\r\n");			
		return 1;
	}
	conditions = 0;
	if(strNCmp(&cmdStr[j],(const u8*)"go_until(cpy,fwd,",17)==0 )	conditions++;
	i=firstNumFrStr(&cmdStr[j], &val);	if(i)											conditions++;
	if(strNCmp(&cmdStr[j+i],(const u8*)")",1)==0 )								conditions++;
	if(conditions == 3){
		pDev->Go_Until(pRsrc, ACTION_COPY, FWD, Speed_Steps_to_Par(val));
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
		huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
		huartDev->PrintSDS(&huartDev->Rsrc, (const u8*)".go_until(cpy,fwd,", val, (const u8*)")\r\n");			
		return 1;
	}
	conditions = 0;
	if(strNCmp(&cmdStr[j],(const u8*)"go_until(cpy,rev,",17)==0 )	conditions++;
	i=firstNumFrStr(&cmdStr[j], &val);	if(i)											conditions++;
	if(strNCmp(&cmdStr[j+i],(const u8*)")",1)==0 )								conditions++;
	if(conditions == 3){
		pDev->Go_Until(pRsrc, ACTION_COPY, REV, Speed_Steps_to_Par(val));
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
		huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
		huartDev->PrintSDS(&huartDev->Rsrc, (const u8*)".go_until(cpy,rev,", val, (const u8*)")\r\n");			
		return 1;
	}
	
	/***********************************************************
	@	%name.release_sw(cpy/rst,fwd/rev) 
	@ 
		This command can be given anytime and is immediately executed.
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"release_sw(rst,fwd)",19)==0 ){
		pDev->Release_SW(pRsrc, ACTION_RESET, FWD);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
		huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".release_sw(rst,fwd)\r\n");			
		return 1;
	}
	if(strNCmp(&cmdStr[j],(const u8*)"release_sw(rst,rev)",19)==0 ){
		pDev->Release_SW(pRsrc, ACTION_RESET, REV);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
		huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".release_sw(rst,rev)\r\n");			
		return 1;
	}
	if(strNCmp(&cmdStr[j],(const u8*)"release_sw(cpy,fwd)",19)==0 ){
		pDev->Release_SW(pRsrc, ACTION_COPY, FWD);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
		huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".release_sw(cpy,fwd)\r\n");			
		return 1;	
	}
	if(strNCmp(&cmdStr[j],(const u8*)"release_sw(cpy,rev)",19)==0 ){
		pDev->Release_SW(pRsrc, ACTION_COPY, REV);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
		huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".release_sw(cpy,rev)\r\n");			
		return 1;		
	}

	/***********************************************************
	@	%name.go_home()
	@ 
		This command can be given anytime and is immediately executed.
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"go_home()",9)==0 ){
		pDev->Go_Home(pRsrc);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
		huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".go_home()\r\n");		
		return 1;
	}

	/***********************************************************
	@	%name.go_mark()
	@ 
		This command can be given anytime and is immediately executed.
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"go_mark()",9)==0 ){
		pDev->Go_Mark(pRsrc);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
		huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".go_mark()\r\n");		
		return 1;
	}

	/***********************************************************
	@	%name.reset_pos()
	@ 
		This command can be given anytime and is immediately executed.
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"reset_pos()",11)==0 ){
		pDev->Reset_Pos(pRsrc);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");
		huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".reset_pos()\r\n");		
		return 1;
	}

	/***********************************************************
	@	%name.soft_stop()
	@ 
		This command can be given anytime and is immediately executed.
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"soft_stop()",11)==0 ){
		pDev->Soft_Stop(pRsrc);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
		huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".soft_stop()\r\n");			
		return 1;
	}
	
	/***********************************************************
	@	%name.hard_stop()
	@ 
		This command can be given anytime and is immediately executed.
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"hard_stop()",11)==0 ){
		pDev->Hard_Stop(pRsrc);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
		huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".hard_stop()\r\n");		
		return 1;
	}

	/***********************************************************
	@	%name.soft_hiz()
	@ 
		This command can be given anytime and is immediately executed.
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"soft_hiz()",10)==0 ){
		pDev->Soft_HiZ(pRsrc);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
		huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".soft_hiz()\r\n");	
		return 1;
	}
	
	/***********************************************************
	@	%name.hard_hiz()
	@ 
		This command can be given anytime and is immediately executed.
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"hard_hiz()",10)==0 ){
		pDev->Hard_HiZ(pRsrc);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
		huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".hard_hiz()\r\n");	
		return 1;
	}
	
	/***********************************************************
	@	%name.enableSatusReport
	@ 
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"enablestatusreport()",20)==0 ){
		pDev->enableRptStatus(pRsrc);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
		huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".enableStatusReport()\r\n");	
		return 1;
	}
	/***********************************************************
	@	%name.disableSatusReport
	@ 
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"disablestatusreport()",21)==0 ){
		pDev->disableRptStatus(pRsrc);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
		huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".enableStatusReport()\r\n");	
		return 1;
	}
	/***********************************************************
	@	%name.enableAbsPosReport
	@ 
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"enableabsposreport()",20)==0 ){
		pDev->enableRptAbsPos(pRsrc);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
		huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".enableAbsPosReport()\r\n");	
		return 1;
	}
	/***********************************************************
	@	%name.disableAbsPosReport
	@ 
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"disableabsposreport()",21)==0 ){
		pDev->disableRptAbsPos(pRsrc);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
		huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".disableAbsPosReport()\r\n");	
		return 1;
	}
	/***********************************************************
	@	%name.enableAdcOutReport
	@ 
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"enableadcoutreport()",20)==0 ){
		pDev->enableRptAdcOut(pRsrc);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
		huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".enableAdcOutReport()\r\n");	
		return 1;
	}
	/***********************************************************
	@	%name.disableAdcOutReport
	@ 
	************************************************************/
	if(strNCmp(&cmdStr[j],(const u8*)"disableadcoutreport()",21)==0 ){
		pDev->disableRptAdcOut(pRsrc);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"+ok @");		
		huartDev->PrintStr(&huartDev->Rsrc, pDev->name);
		huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".disableAdcOutReport()\r\n");	
		return 1;
	}
	
	/***********************************************************
	@	%name.reportAllRegs()
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

void motorReportAbsPos(UART_DEV *huartDev,const u8* leading, u32 pos){
	huartDev->PrintStr(&huartDev->Rsrc, leading);
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".get_abs_pos()\t%ABS_POS=");
	huartDev->PrintHex32(&huartDev->Rsrc, pos);
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");		
}

void motorReportStatus(UART_DEV *huartDev,const u8* leading, u16 sta){
	huartDev->PrintStr(&huartDev->Rsrc, leading);
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".get_status()\t");
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"%STATUS=");
	huartDev->PrintHex16(&huartDev->Rsrc, sta);
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");
}

void motorReportAdcOut(UART_DEV *huartDev,const u8* leading, u8 adc){
	huartDev->PrintStr(&huartDev->Rsrc, leading);
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)".get_adc_out()\t%ADC_OUT=");
	huartDev->PrintHex8(&huartDev->Rsrc, adc);
	huartDev->PrintStr(&huartDev->Rsrc, (const u8*)"\r\n");
}


/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
