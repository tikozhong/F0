/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : uartFwd_cmd.c
* Author             : Tiko Zhong
* Date First Issued  : 07/16/2018
* Description        : This file provides a set of functions needed to manage the
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "uartFwd_cmd.h"
#include "responseX.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
const u8 UARTFWD_LED_HELP[] = {
	"\nUartFwd Task Commands:"
	"\n sys.help(\"uartfwd_t\")"
	"\nUartFwd Task Commands:"
	"\n %dev.forward(brdIndx, ledIndx, color)\t\\\\color:0..3, 0:off\t1:red\t2:green\t3:blue"
	"\r\n"
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/*******************************************************************************
* Function Name  : 
* Description    : 
* Input          : 
								: 
* Output         : None
* Return         : None 
*******************************************************************************/
u8 uartFwdCmd(UARTFWD_Dev_T *pDev, PAKET_T *packetIn, PAKET_T *packetOut){
	packetOut->option0 = 2;	//default, packet will as cpp style
	char str[16] = {0};

	//match devName
	if(isSameStr((const u8*)packetIn->addr[0], (const u8*)pDev->rsrc.name)){
		//%devName.help()
		if(isSameStr(packetIn->addr[1], (const u8*)"help") && packetIsMatch(packetIn, "%s%s")){
			RESPONSE(packetOut, RESPONSE_OK, (char*)pDev->rsrc.name, "help", NULL);
			strcat(str,RESPONSE_OK);
			strcat(str,pDev->rsrc.name);
			packetAddTo(packetOut, "%s%s%p", str, "help", UARTFWD_LED_HELP);
			return 1;
		}
		//%devName.forward(uartIndx,CMD,END,timeout)
		else if(isSameStr(packetIn->addr[1], (const u8*)"forward") && packetIsMatch(packetIn, "%s%s")){
			if(packetIsMatch(packetIn, "%s%s%s%u")){
				pDev->Forward(&pDev->rsrc, 0xffff, (char*)packetIn->addr[2], *(u32*)packetIn->addr[3]);
				RESPONSE(packetOut, RESPONSE_OK, (char*)pDev->rsrc.name, "forward", NULL);
			}
			else if(packetIsMatch(packetIn, "%s%s%u%s%u")){
				pDev->Forward(&pDev->rsrc, *(u32*)packetIn->addr[2], (char*)packetIn->addr[3], *(u32*)packetIn->addr[4]);
				RESPONSE(packetOut, RESPONSE_OK, (char*)pDev->rsrc.name, "forward", NULL);
			}
			else{
				RESPONSE(packetOut, RESPONSE_ERR, (char*)pDev->rsrc.name, "forward", NULL);
			}
			return 1;
		}
		else{
			strcat(str,RESPONSE_ERR);
			strcat(str,pDev->rsrc.name);
			packetAddTo(packetOut, "%s%s%s", str, packetIn->addr[1], "unknownFunc");
			return 0;
		}
	}
	else{
		strcat(str,RESPONSE_ERR);
		strcat(str, (char*)packetIn->addr[0]);
		packetAddTo(packetOut, "%s%s%s%s", str, packetIn->addr[1], "unknownDev", "unknownFunc");
		return 0;
	}
}

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
