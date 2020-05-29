/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : mcp42xxx_CMD.c
* Author             : Tiko Zhong
* Date First Issued  : 12/01/2015
* Description        : This file provides a set of functions needed to manage the
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "mcp42xxx_CMD.h"
#include "string.h"
#include "stdio.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
//extern UART_DEV myUart;
const char MCP42XXX_HELP[] = {
	"MCP42XXX Task Commands:"
	"\n sys.help(\"ad9834_t\")"
	"\n sys.listdev(\"ad9834_t\")"
	"\nAD9834 device Commands:"
	"\n dev.help()"
	"\n dev.sine(\"frq\")\t//set a fixed sine wave"
	"\n dev.triangle(\"frq\")\t//set a fixed triangle wave"
	"\n %dev.ScanSine(startFrq, endFrq, StepFrq, StepTime)\t//scane a sine wave"
	"\n %dev.ScanTriangle(startFrq, endFrq, StepFrq, StepTime)\t//scane a triangle wave"
	"\n dev.powerDown()\t//set output to 0Volt"
	"\n where:"
	"\n frq: in microHZ, StepTime: in ms, >=4mS"
	"\r\n"
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/*******************************************************************************
* Function Name  : AD9834Cmd
*******************************************************************************/
#define FUN_LEN 64
u8 MCP42XXX_Cmd(MCP42XXX_Dev_T *pDev, const char* CMD, void (*print)(const char* FORMAT_ORG, ...)){
	MCP42XXX_Rsrc_T* pRsrc = &pDev->rsrc;
	s16 nameLen = strlen(pRsrc->name);
	s32 num[4];
	const char* LINE = NULL;
	if(strncmp(CMD, pRsrc->name, nameLen)==0){
		LINE = &CMD[nameLen];
		if(strncmp(LINE, ".help", strlen("help"))==0){
			print(MCP42XXX_HELP);
			return 1;
		}	
		//dev.SetPosA(ohm)
		else if(sscanf(LINE, ".setposa %d ", &num[0])== 1){
			pDev->SetPosA(pRsrc, num[0]);
			print("+ok@%s.setPosA(%d)\r\n", pRsrc->name, num[0]);
			return 1;
		}
		//dev.SetPosB(ohm)
		else if(sscanf(LINE, ".setposb %d ", &num[0])== 1){
			pDev->SetPosB(pRsrc, num[0]);
			print("+ok@%s.setPosB(%d)\r\n", pRsrc->name, num[0]);
			return 1;
		}	
		//dev.IncPosA(ohm)
		else if(sscanf(LINE, ".stepposa %d ", &num[0])== 1){
			pDev->StepPosA(pRsrc, num[0]);
			print("+ok@%s.setPosA(%d)\r\n", pRsrc->name, pRsrc->posA);
			return 1;
		}
		//dev.IncPosB(ohm)
		else if(sscanf(LINE, ".stepposb %d ", &num[0])== 1){
			pDev->StepPosB(pRsrc, num[0]);
			print("+ok@%s.setPosB(%d)\r\n", pRsrc->name, pRsrc->posB);
			return 1;
		}

		
		
//		//dev.triangle(mHz), in 10E-3 Hz
//		else if(sscanf(LINE, ".triangle %d ", &num[0])== 1){
//			pDev->TriangleWave(pRsrc, num[0]);
//			print("+ok@%s.triangle(%d)\r\n", pRsrc->name, num[0]);
//			return 1;
//		}
//		//dev.scansine(star_mHz,end_mHz, step_mHz, stepTime), in 10E-3 Hz
//		else if(sscanf(LINE, ".scansine %d %d %d %d ", &num[0], &num[1],&num[2],&num[3])== 4){
//			pDev->ScanSineWave(pRsrc, num[0], num[1], num[2], num[3]);
//			print("+ok@%s.scansine(%d,%d,%d,%d)\r\n", pRsrc->name, num[0], num[1], num[2], num[3]);
//			return 1;
//		}
//		else if(strncmp(LINE, ".powerdown  ", strlen(".powerdown  "))==0){
//			pDev->Powerdown(pRsrc);
//			print("+ok@%s.powerdown()\r\n", pRsrc->name);
//			return 1;
//		}
	}
	return 0;
}

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
