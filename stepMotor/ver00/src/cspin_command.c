/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : cspin_command.c
* Author             : Tiko Zhong
* Date First Issued  : 12/01/2015
* Description        : This file provides a set of functions needed to manage the
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include <string.h> 
#include "mystring.h"
#include "myuart.h"
#include "cspin_command.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/	
/*******************************************************************************
* Function Name  : motorCmd
* Description    : motorCmd function commands
* Input          : - huart : using this uart to print
								 : - cmdStr : the command string
* Output         : None
* Return         : None
*******************************************************************************/
uint8_t cspin_command(UART_HandleTypeDef *huart, cSPIN_Rsrc_T* pRsrc,uint8_t* str, cSPIN_Cmd_PrtEn_T printEn){
	uint32_t val;
	uint8_t* cmdStr;
	
	/* delete SPACE/TAB(others can not be printed symbols) */
	cmdStr = seekToPrntSymb(str);
	
	/***********************************************************
	@	_setup()
	************************************************************/	
	if(strncmp((const char*)cmdStr,"_setup()",8)==0 ){
		cSPIN_Setup(pRsrc);
		if(printEn)	myPrintfStr(huart, (uint8_t*)"_setup()\r\n");
		return 1;
	}
	/***********************************************************
	@	_set_param()
	************************************************************/
	if(strncmp((const char*)cmdStr,"_set_param(abs_pos,",19)==0 ){
		if(firstNumFrStr(cmdStr, &val)){
			cSPIN_Set_Param(pRsrc, cSPIN_ABS_POS, val);
			if(printEn){
				myPrintfStr(huart, (uint8_t*)"_set_param(abs_pos,");
				myPrintfDec(huart,val);
				myPrintfStr(huart, (uint8_t*)")\r\n");				
			}
		}
		else	myPrintfStr(huart, (uint8_t*)"A number must be needed\r\n");
		return 1;
	}
	if(strncmp((const char*)cmdStr,"_set_param(mark,",16)==0 ){
		if(firstNumFrStr(cmdStr, &val)){
			cSPIN_Set_Param(pRsrc, cSPIN_MARK, val);
			if(printEn){
				myPrintfStr(huart, (uint8_t*)"_set_param(mark,");
				myPrintfDec(huart,val);
				myPrintfStr(huart, (uint8_t*)")\r\n");
			}
		}
		else	myPrintfStr(huart, (uint8_t*)"A number must be needed\r\n");
		return 1;
	}
	if(strncmp((const char*)cmdStr,"_set_param(acc,",15)==0 ){
		if(firstNumFrStr(cmdStr, &val)){
			cSPIN_Set_Param(pRsrc, cSPIN_ACC, AccDec_Steps_to_Par(val));
			if(printEn){
				myPrintfStr(huart, (uint8_t*)"_set_param(acc,");
				myPrintfDec(huart,val);
				myPrintfStr(huart, (uint8_t*)")\r\n");
			}
		}
		else	myPrintfStr(huart, (uint8_t*)"A number must be needed\r\n");
		return 1;
	}
	if(strncmp((const char*)cmdStr,"_set_param(dec,",15)==0 ){
		if(firstNumFrStr(cmdStr, &val)){
			cSPIN_Set_Param(pRsrc, cSPIN_DEC, AccDec_Steps_to_Par(val));
			if(printEn){
				myPrintfStr(huart, (uint8_t*)"_set_param(dec,");
				myPrintfDec(huart,val);
				myPrintfStr(huart, (uint8_t*)")\r\n");
			}
		}
		else	myPrintfStr(huart, (uint8_t*)"A number must be needed\r\n");
		return 1;
	}
	if(strncmp((const char*)cmdStr,"_set_param(max_speed,",21)==0 ){
		if(firstNumFrStr(cmdStr, &val)){
			cSPIN_Set_Param(pRsrc, cSPIN_MAX_SPEED, MaxSpd_Steps_to_Par(val));
			if(printEn){
				myPrintfStr(huart, (uint8_t*)"_set_param(max_speed,");
				myPrintfDec(huart,val);
				myPrintfStr(huart, (uint8_t*)")\r\n");
			}
		}
		else	myPrintfStr(huart, (uint8_t*)"A number must be needed\r\n");
		return 1;
	}
	if(strncmp((const char*)cmdStr,"_set_param(min_speed,",21)==0 ){
		if(firstNumFrStr(cmdStr, &val)){
			cSPIN_Set_Param(pRsrc, cSPIN_MIN_SPEED, MinSpd_Steps_to_Par(val));
			if(printEn){
				myPrintfStr(huart, (uint8_t*)"_set_param(min_speed,");
				myPrintfDec(huart,val);
				myPrintfStr(huart, (uint8_t*)")\r\n");
			}
		}
		else	myPrintfStr(huart, (uint8_t*)"A number must be needed\r\n");
		return 1;
	}
	if(strncmp((const char*)cmdStr,"_set_param(fs_spd,",17)==0 ){
		if(firstNumFrStr(cmdStr, &val)){
			cSPIN_Set_Param(pRsrc, cSPIN_FS_SPD, FSSpd_Steps_to_Par(val));
			if(printEn){
				myPrintfStr(huart, (uint8_t*)"_set_param(fs_spd,");
				myPrintfDec(huart,val);
				myPrintfStr(huart, (uint8_t*)")\r\n");
			}
		}
		else	myPrintfStr(huart, (uint8_t*)"A number must be needed\r\n");
		return 1;
	}
	if(strncmp((const char*)cmdStr,"_set_param(int_spd,",18)==0 ){
		if(firstNumFrStr(cmdStr, &val)){
			cSPIN_Set_Param(pRsrc, cSPIN_INT_SPD, IntSpd_Steps_to_Par(val));
			if(printEn){
				myPrintfStr(huart, (uint8_t*)"_set_param(int_spd,");
				myPrintfDec(huart,val);
				myPrintfStr(huart, (uint8_t*)")\r\n");
			}
		}
		else	myPrintfStr(huart, (uint8_t*)"A number must be needed\r\n");
		return 1;
	}
	if(strncmp((const char*)cmdStr,"_set_param(config,",18)==0 ){
		if(firstNumFrStr(cmdStr, &val)){
			cSPIN_Set_Param(pRsrc, cSPIN_CONFIG, val);
			if(printEn){
				myPrintfStr(huart, (uint8_t*)"_set_param(config,");
				myPrintfBin(huart,val);
				myPrintfStr(huart, (uint8_t*)")\r\n");
			}
		}
		else	myPrintfStr(huart, (uint8_t*)"A number must be needed\r\n");
		return 1;
	}
	if(strncmp((const char*)cmdStr,"_set_param(status,",18)==0 ){
		if(firstNumFrStr(cmdStr, &val)){
			cSPIN_Set_Param(pRsrc, cSPIN_STATUS, val);
			if(printEn){
				myPrintfStr(huart, (uint8_t*)"_set_param(status,");
				myPrintfBin(huart,val);
				myPrintfStr(huart, (uint8_t*)")\r\n");
			}
		}
		else	myPrintfStr(huart, (uint8_t*)"A number must be needed\r\n");
		return 1;
	}
	/***********************************************************
	@	_get_param()
	************************************************************/
	if(strncmp((const char*)cmdStr,"_get_param(abs_pos)",19)==0 ){
		pRsrc->reg.ABS_POS = cSPIN_Get_Param(pRsrc, cSPIN_ABS_POS);
		myPrintfStr(huart, (uint8_t*)"ABS_POS: ");
		myPrintfDec(huart,pRsrc->reg.ABS_POS);
		myPrintfStr(huart, (uint8_t*)"\r\n");
		return 1;
	}
	if(strncmp((const char*)cmdStr,"_get_param(mark)",16)==0 ){
		pRsrc->reg.MARK = cSPIN_Get_Param(pRsrc, cSPIN_MARK);
		myPrintfStr(huart, (uint8_t*)"MARK: ");
		myPrintfDec(huart,pRsrc->reg.MARK);
		myPrintfStr(huart, (uint8_t*)"\r\n");
		return 1;
	}
	if(strncmp((const char*)cmdStr,"_get_param(acc)",15)==0 ){
		pRsrc->reg.ACC = cSPIN_Get_Param(pRsrc, cSPIN_ACC);
		myPrintfStr(huart, (uint8_t*)"ACC: ");
		myPrintfDec(huart,(pRsrc->reg.ACC));
		myPrintfStr(huart, (uint8_t*)"\r\n");
		return 1;
	}
	if(strncmp((const char*)cmdStr,"_get_param(dec)",15)==0 ){
		pRsrc->reg.DEC = cSPIN_Get_Param(pRsrc, cSPIN_DEC);
		myPrintfStr(huart, (uint8_t*)"DEC: ");
		myPrintfDec(huart,(pRsrc->reg.DEC));
		myPrintfStr(huart, (uint8_t*)"\r\n");
		return 1;
	}
	if(strncmp((const char*)cmdStr,"_get_param(max_speed)",21)==0 ){
		pRsrc->reg.MAX_SPEED = cSPIN_Get_Param(pRsrc, cSPIN_MAX_SPEED);
		myPrintfStr(huart, (uint8_t*)"MAX_SPEED: ");
		myPrintfDec(huart,(pRsrc->reg.MAX_SPEED));
		myPrintfStr(huart, (uint8_t*)"\r\n");
		return 1;
	}
	if(strncmp((const char*)cmdStr,"_get_param(min_speed)",21)==0 ){
		pRsrc->reg.MIN_SPEED = cSPIN_Get_Param(pRsrc, cSPIN_MIN_SPEED);
		myPrintfStr(huart, (uint8_t*)"MIN_SPEED: ");
		myPrintfDec(huart,(pRsrc->reg.MIN_SPEED));
		myPrintfStr(huart, (uint8_t*)"\r\n");
		return 1;
	}
	if(strncmp((const char*)cmdStr,"_get_param(fs_spd)",17)==0 ){
		pRsrc->reg.FS_SPD = cSPIN_Get_Param(pRsrc, cSPIN_FS_SPD);
		myPrintfStr(huart, (uint8_t*)"FS_SPD: ");
		myPrintfDec(huart,(pRsrc->reg.FS_SPD));
		myPrintfStr(huart, (uint8_t*)"\r\n");
		return 1;
	}
	if(strncmp((const char*)cmdStr,"_get_param(int_spd)",18)==0 ){
		pRsrc->reg.INT_SPD = cSPIN_Get_Param(pRsrc, cSPIN_INT_SPD);
		myPrintfStr(huart, (uint8_t*)"INT_SPD: ");
		myPrintfDec(huart,(pRsrc->reg.INT_SPD));
		myPrintfStr(huart, (uint8_t*)"\r\n");
		return 1;
	}
	if(strncmp((const char*)cmdStr,"_get_param(config)",18)==0 ){
		pRsrc->reg.CONFIG = cSPIN_Get_Param(pRsrc, cSPIN_CONFIG);
		myPrintfStr(huart, (uint8_t*)"CONFIG: ");
		myPrintfHex(huart,pRsrc->reg.CONFIG);
		myPrintfStr(huart, (uint8_t*)"\r\n");
		return 1;
	}
	if(strncmp((const char*)cmdStr,"_get_param(status)",18)==0 ){
		pRsrc->reg.STATUS = cSPIN_Get_Param(pRsrc, cSPIN_STATUS);
		myPrintfStr(huart, (uint8_t*)"STATUS: ");
		myPrintfBin(huart,pRsrc->reg.STATUS);
		myPrintfStr(huart, (uint8_t*)"\r\n");
		return 1;
	}
	/***********************************************************
	@	_run(fwd/rev,spd) spd in steps/s, 
		This command can be given anytime and is immediately executed.
	************************************************************/
	if(strncmp((const char*)cmdStr,"_run(fwd,",9)==0 ){
		if(firstNumFrStr(cmdStr, &val)){
			cSPIN_Run(pRsrc, FWD, Speed_Steps_to_Par(val));
			if(printEn){
				myPrintfStr(huart, (uint8_t*)"_run(fwd,");
				myPrintfDec(huart,val);
				myPrintfStr(huart, (uint8_t*)")\r\n");
			}
		}
		else	myPrintfStr(huart, (uint8_t*)"A number must be needed\r\n");
		return 1;
	}
	if(strncmp((const char*)cmdStr,"_run(rev,",9)==0 ){
		if(firstNumFrStr(cmdStr, &val)){
			cSPIN_Run(pRsrc, REV, Speed_Steps_to_Par(val));
			if(printEn){
				myPrintfStr(huart, (uint8_t*)"_run(rev,");
				myPrintfDec(huart,val);
				myPrintfStr(huart, (uint8_t*)")\r\n");
			}
		}
		else	myPrintfStr(huart, (uint8_t*)"A number must be needed\r\n");
		return 1;
	}
	/***********************************************************
	@	_Step_Clock(fwd/rev)
		This command can only be given when the motor is stopped.
	************************************************************/
	if(strncmp((const char*)cmdStr,"_step_clock(fwd)",16)==0 ){
		cSPIN_Step_Clock(pRsrc, FWD);
		if(printEn)	myPrintfStr(huart, (uint8_t*)"send pulse to run fwd...\r\n");
		return 1;
	}
	if(strncmp((const char*)cmdStr,"_step_clock(rev)",16)==0 ){
		cSPIN_Step_Clock(pRsrc, REV);
		if(printEn)	myPrintfStr(huart, (uint8_t*)"send pulse to run rev...\r\n");
		return 1;
	}
	/***********************************************************
	@	_move(fwd/rev,nstep), nstep is N*step
		This command can only be given when the motor is stopped.
	************************************************************/
	if(strncmp((const char*)cmdStr,"_move(fwd,",10)==0 ){
		if(firstNumFrStr(cmdStr, &val)){
			cSPIN_Move(pRsrc, FWD, val);
			if(printEn){
				myPrintfStr(huart, (uint8_t*)"_move(fwd,");
				myPrintfDec(huart,val);
				myPrintfStr(huart, (uint8_t*)")\r\n");
			}
		}
		else	myPrintfStr(huart, (uint8_t*)"A number must be needed\r\n");
		return 1;
	}
	if(strncmp((const char*)cmdStr,"_move(rev,",10)==0 ){
		if(firstNumFrStr(cmdStr, &val)){
			cSPIN_Move(pRsrc, REV, val);
			if(printEn){
				myPrintfStr(huart, (uint8_t*)"_move(rev,");
				myPrintfDec(huart,val);
				myPrintfStr(huart, (uint8_t*)")\r\n");
			}
		}
		else	myPrintfStr(huart, (uint8_t*)"A number must be needed\r\n");
		return 1;
	}
	/***********************************************************
	@	_go_to(absPos),  absPos is in N*step
		This command can be given only when the previous motion command as been completed 
	************************************************************/
	if(strncmp((const char*)cmdStr,"_go_to(",7)==0 ){
		if(firstNumFrStr(cmdStr, &val)){
			cSPIN_Go_To(pRsrc, val);
			if(printEn){
				myPrintfStr(huart, (uint8_t*)"_go_to(");
				myPrintfDec(huart,val);
				myPrintfStr(huart, (uint8_t*)")\r\n");
			}
		}
		else	myPrintfStr(huart, (uint8_t*)"A number must be needed\r\n");
		return 1;
	}
	/***********************************************************
	@	_go_to_dir(fwd/rev,absPos),  absPos is in N*step
		This command can be given only when the previous motion command as been completed 
	************************************************************/
	if(strncmp((const char*)cmdStr,"_go_to_dir(fwd,",15)==0 ){
		if(firstNumFrStr(cmdStr, &val)){
			cSPIN_Go_To_Dir(pRsrc, FWD, val);
			if(printEn){
				myPrintfStr(huart, (uint8_t*)"_go_to_dir(fwd,");
				myPrintfDec(huart,val);
				myPrintfStr(huart, (uint8_t*)")\r\n");
			}
		}
		else	myPrintfStr(huart, (uint8_t*)"A number must be needed\r\n");
		return 1;
	}
	if(strncmp((const char*)cmdStr,"_go_to_dir(rev,",15)==0 ){
		if(firstNumFrStr(cmdStr, &val)){
			cSPIN_Go_To_Dir(pRsrc, REV, val);
			if(printEn){
				myPrintfStr(huart, (uint8_t*)"_go_to_dir(rev,");
				myPrintfDec(huart,val);
				myPrintfStr(huart, (uint8_t*)")\r\n");
			}
		}
		else	myPrintfStr(huart, (uint8_t*)"A number must be needed\r\n");
		return 1;
	}
	/***********************************************************
	@	_go_until(act:cpy/rst,direct:fwd/rev,speed) speed in step/s
		This command can be given anytime and is immediately executed.
	************************************************************/
	if(strncmp((const char*)cmdStr,"_go_until(rst,fwd,",18)==0 ){
		if(firstNumFrStr(cmdStr, &val)){
			cSPIN_Go_Until(pRsrc, ACTION_RESET, FWD, Speed_Steps_to_Par(val));
			if(printEn){
				myPrintfStr(huart, (uint8_t*)"_go_until(rst,fwd,");
				myPrintfDec(huart,val);
				myPrintfStr(huart, (uint8_t*)")\r\n");
			}
		}
		else	myPrintfStr(huart, (uint8_t*)"A number must be needed\r\n");
		return 1;
	}
	if(strncmp((const char*)cmdStr,"_go_until(rst,rev,",18)==0 ){
		if(firstNumFrStr(cmdStr, &val)){
			cSPIN_Go_Until(pRsrc, ACTION_RESET, REV, Speed_Steps_to_Par(val));
			if(printEn){
				myPrintfStr(huart, (uint8_t*)"_go_until(rst,rev,");
				myPrintfDec(huart,val);
				myPrintfStr(huart, (uint8_t*)")\r\n");
			}
		}
		else	myPrintfStr(huart, (uint8_t*)"A number must be needed\r\n");
		return 1;
	}
	if(strncmp((const char*)cmdStr,"_go_until(cpy,fwd,",18)==0 ){
		if(firstNumFrStr(cmdStr, &val)){
			cSPIN_Go_Until(pRsrc, ACTION_COPY, FWD, Speed_Steps_to_Par(val));
			if(printEn){
				myPrintfStr(huart, (uint8_t*)"_go_until(cpy,fwd,");
				myPrintfDec(huart,val);
				myPrintfStr(huart, (uint8_t*)")\r\n");
			}
		}
		else	myPrintfStr(huart, (uint8_t*)"A number must be needed\r\n");
		return 1;
	}
	if(strncmp((const char*)cmdStr,"_go_until(cpy,rev,",18)==0 ){
		if(firstNumFrStr(cmdStr, &val)){
			cSPIN_Go_Until(pRsrc, ACTION_COPY, REV, Speed_Steps_to_Par(val));
			if(printEn){
				myPrintfStr(huart, (uint8_t*)"_go_until(cpy,rev,");
				myPrintfDec(huart,val);
				myPrintfStr(huart, (uint8_t*)")\r\n");
			}
		}
		else	myPrintfStr(huart, (uint8_t*)"A number must be needed\r\n");
		return 1;
	}
	/***********************************************************
	@_release_sw(act:cpy/rst,direct:fwd/rev) 
	************************************************************/
	if(strncmp((const char*)cmdStr,"_release_sw(rst,fwd)",20)==0 ){
		cSPIN_Release_SW(pRsrc, ACTION_RESET, FWD);
		if(printEn)	myPrintfStr(huart, (uint8_t*)"_release_sw(rst,fwd)\r\n");
	}
	if(strncmp((const char*)cmdStr,"_release_sw(rst,rev)",20)==0 ){
		cSPIN_Release_SW(pRsrc, ACTION_RESET, REV);
		if(printEn)	myPrintfStr(huart, (uint8_t*)"_release_sw(rst,rev)\r\n");
		return 1;
	}
	if(strncmp((const char*)cmdStr,"_release_sw(cpy,fwd)",20)==0 ){
		cSPIN_Release_SW(pRsrc, ACTION_COPY, FWD);
		myPrintfStr(huart, (uint8_t*)"_release_sw(cpy,fwd)\r\n");
		return 1;
	}
	if(strncmp((const char*)cmdStr,"_release_sw(cpy,rev)",20)==0 ){
		cSPIN_Release_SW(pRsrc, ACTION_COPY, REV);
		if(printEn)	myPrintfStr(huart, (uint8_t*)"_release_sw(cpy,rev)\r\n");
		return 1;
	}
	/***********************************************************
	@	_go_home() 
		This command can be given only when the previous motion command as been completed 
	************************************************************/
	if(strncmp((const char*)cmdStr,"_go_home()",10)==0 ){
		cSPIN_Go_Home(pRsrc);
		if(printEn)	myPrintfStr(huart, (uint8_t*)"_go_home()\r\n");
		return 1;
	}
	/***********************************************************
	@	_go_mark() 
		This command can be given only when the previous motion command as been completed 
	************************************************************/
	if(strncmp((const char*)cmdStr,"_go_mark()",10)==0 ){
		cSPIN_Go_Mark(pRsrc);
		if(printEn)	myPrintfStr(huart, (uint8_t*)"_go_mark()\r\n");
		return 1;
	}
	/***********************************************************
	@ _reset_pos()
	************************************************************/
	if(strncmp((const char*)cmdStr,"_reset_pos()",12)==0 ){
		cSPIN_Reset_Pos(pRsrc);
		if(printEn)	myPrintfStr(huart, (uint8_t*)"_reset_pos()\r\n");
		return 1;
	}
	/***********************************************************
	@ _soft_stop()
	************************************************************/
	if(strncmp((const char*)cmdStr,"_soft_stop()",12)==0 ){
		cSPIN_Soft_Stop(pRsrc);
		if(printEn)	myPrintfStr(huart, (uint8_t*)"_soft_stop()\r\n");
		return 1;
	}
	/***********************************************************
	@ _hard_stop()
	************************************************************/
	if(strncmp((const char*)cmdStr,"_hard_stop()",12)==0 ){
		cSPIN_Hard_Stop(pRsrc);
		if(printEn)	myPrintfStr(huart, (uint8_t*)"_hard_stop()\r\n");
		return 1;
	}
	/***********************************************************
	@ _soft_hiz()
	************************************************************/
	if(strncmp((const char*)cmdStr,"_soft_hiz()",11)==0 ){
		cSPIN_Soft_HiZ(pRsrc);
		if(printEn)	myPrintfStr(huart, (uint8_t*)"_soft_hiz()\r\n");
		return 1;
	}
	/***********************************************************
	@ _hard_hiz()
	************************************************************/
	if(strncmp((const char*)cmdStr,"_hard_hiz()",11)==0 ){
		cSPIN_Hard_HiZ(pRsrc);
		if(printEn)	myPrintfStr(huart, (uint8_t*)"_hard_hiz()\r\n");
		return 1;
	}
	/***********************************************************
	@ _get_status()
	************************************************************/
	if(strncmp((const char*)cmdStr,"_get_status()",13)==0 ){
		myPrintfStr(huart,(uint8_t *)"Status: ");
		myPrintfBin(huart,cSPIN_Get_Status(pRsrc));
		myPrintfStr(huart,(uint8_t *)"\r\n");	
		return 1;
	}
	return 0;
}

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
