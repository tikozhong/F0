/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : fbusb16x1_cmd.c
* Author             : Tiko Zhong
* Date First Issued  : 07/16/2018
* Description        : This file provides a set of functions needed to manage the
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "mbi5024_cmd.h"
#include "responseX.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
const u8 MBI5240_LED_HELP[] = {
	"\nFBUSB16X1 Task Commands:"
	"\n sys.help(\"fbusb16x1_t\")"
	"\nFBUSB16X1 Task Commands:"
	"\n %dev.setColor(brdIndx, ledIndx, color)\t\\\\color:0..3, 0:off\t1:red\t2:green\t3:blue"
	"\r\n"
};

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/*******************************************************************************
* Function Name  : ISL29125Cmd
* Description    : 
* Input          : 
								: 
* Output         : None
* Return         : None 
*******************************************************************************/
u8 mbi5024Cmd(MBI5024_Dev_T *pDev, PAKET_T *packetIn, PAKET_T *packetOut){
//	FBUSB16X1_Rsrc_T* pRsrc;
	u32 tmp0,tmp1,tmp2;
	packetOut->option0 = 2;	//default, packet will as cpp style
	char str[16] = {0};

	//match devName
	if(isSameStr((const u8*)packetIn->addr[0], (const u8*)pDev->rsrc.name)){
		//%devName.help()
		if(isSameStr(packetIn->addr[1], (const u8*)"help") && packetIsMatch(packetIn, "%s%s")){
			RESPONSE(packetOut, RESPONSE_OK, (char*)pDev->rsrc.name, "help", NULL);
			strcat(str,RESPONSE_OK);
			strcat(str,pDev->rsrc.name);
			packetAddTo(packetOut, "%s%s%p", str, "help", MBI5240_LED_HELP);
			return 1;
		}
		//%devName.setled(0,ledport, ledIndx, color)
		else if(isSameStr(packetIn->addr[1], (const u8*)"setled") && packetIsMatch(packetIn, "%s%s%u%u%u%u")){
			tmp0 = *(u32*)packetIn->addr[3];
			tmp1 = *(u32*)packetIn->addr[4];
			tmp2 = *(u32*)packetIn->addr[5];
			pDev->SetLED(&pDev->rsrc, tmp0, tmp1, tmp2);
			strcat(str,RESPONSE_OK);
			strcat(str,pDev->rsrc.name);
			packetAddTo(packetOut, "%s%s%u%u%u%u", str, "setledx", 0, tmp0, tmp1, tmp2);
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
