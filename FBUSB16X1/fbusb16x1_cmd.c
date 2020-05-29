/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : fbusb16x1_cmd.c
* Author             : Tiko Zhong
* Date First Issued  : 07/16/2018
* Description        : This file provides a set of functions needed to manage the
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "fbusb16x1_cmd.h"
#include "responseX.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
const u8 FBUSB16X1_HELP[] = {
	"\nFBUSB16X1 Task Commands:"
	"\n sys.help(\"fbusb16x1_t\")"
	"\nFBUSB16X1 Task Commands:"
	"\n %dev.setUSB(brdIndx, usbIndx)\t\t\\\\brdIndx:0..15\tusbIndx:0..15"
	"\n %dev.setLED(brdIndx, ledIndx, color)\t\\\\color:0..3, 0:off\t1:red\t2:green\t3:blue"
	"\n %dev.getUSB(brdIndx)"
	"\n %dev.getLED(brdIndx,ledIndx)\t\\\\brdIndx:0..15\tledIndx:0..15"
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
//#define FUN_LEN 64
#define READ_MAX 64

u8 fbusb16x1Cmd(FBUSB16X1_Dev_T *pDev, PAKET_T *packetIn, PAKET_T *packetOut){
//	FBUSB16X1_Rsrc_T* pRsrc;
	u32 tmp0,tmp1,tmp2,tmp3;
	packetOut->option0 = 2;	//default, packet will as cpp style
	char str[16] = {0};

	//match devName
	if(isSameStr((const u8*)packetIn->addr[0], (const u8*)pDev->rsrc.name)){
		//%devName.help()
		if(isSameStr(packetIn->addr[1], (const u8*)"help") && packetIsMatch(packetIn, "%s%s")){
			RESPONSE(packetOut, RESPONSE_OK, (char*)pDev->rsrc.name, "help", NULL);


			strcat(str,RESPONSE_OK);
			strcat(str,pDev->rsrc.name);
			packetAddTo(packetOut, "%s%s%p", str, "help", FBUSB16X1_HELP);
			return 1;
		}
		//%devName.setUSB(brdIndx, usbIndx)
		else if(isSameStr(packetIn->addr[1], (const u8*)"setusb") && packetIsMatch(packetIn, "%s%s%u%u")){
			tmp0 = *(u32*)packetIn->addr[2];
			tmp1 = *(u32*)packetIn->addr[3];
			if(tmp0<16 && (tmp1<16 || tmp1==0xff)){
				pDev->SetUSB(&pDev->rsrc, tmp0, tmp1, 1);		//1:when swith to another usb, left its power on.
				strcat(str,RESPONSE_OK);
				strcat(str,pDev->rsrc.name);
				packetAddTo(packetOut, "%s%s%u%u", str, "setusb", tmp0, tmp1);
			}
			else{
				strcat(str,RESPONSE_ERR);
				strcat(str,pDev->rsrc.name);
				packetAddTo(packetOut, "%s%s%s", str, "setusb", "Overfloat");
			}
			return 1;
		}
		//%devName.getUSB(brdIndx)
		else if(isSameStr(packetIn->addr[1], (const u8*)"getusb") && packetIsMatch(packetIn, "%s%s%u")){
			tmp0 = *(u32*)packetIn->addr[2];
			tmp1 = pDev->GetUSB(&pDev->rsrc, tmp0);
			strcat(str,RESPONSE_OK);
			strcat(str,pDev->rsrc.name);
			packetAddTo(packetOut, "%s%s%u%u", str, "getusb", tmp0, tmp1);			
			return 1;			
		}
		//%devName.setLED(brdIndx, ledIndx, color)
		else if(isSameStr(packetIn->addr[1], (const u8*)"setled") && packetIsMatch(packetIn, "%s%s%u%u%u")){
			tmp0 = *(u32*)packetIn->addr[2];
			tmp1 = *(u32*)packetIn->addr[3];
			tmp3 = *(u32*)packetIn->addr[4];
			tmp2 = tmp1%3;
			tmp1 /= 3;
			if(tmp0<16 && tmp1<8 && tmp2<3){
				pDev->SetLED(&pDev->rsrc, tmp0, tmp1, tmp2, tmp3);
				strcat(str,RESPONSE_OK);
				strcat(str,pDev->rsrc.name);
				packetAddTo(packetOut, "%s%s%u%u%u", str, "setled", tmp0, *(u32*)packetIn->addr[3], tmp3);
			}
			else{
				strcat(str,RESPONSE_ERR);
				strcat(str,pDev->rsrc.name);
				packetAddTo(packetOut, "%s%s%s", str, "setled", "Overfloat");
			}
			return 1;
		}	
		//%devName.setLED(brdIndx, portIndx, ledIndx, color)
		else if(isSameStr(packetIn->addr[1], (const u8*)"setled") && packetIsMatch(packetIn, "%s%s%u%u%u%u")){
			tmp0 = *(u32*)packetIn->addr[2];
			tmp1 = *(u32*)packetIn->addr[3];
			tmp2 = *(u32*)packetIn->addr[4];
			tmp3 = *(u32*)packetIn->addr[5];
			if(tmp0<16 && tmp1<8 && tmp2<3){
				pDev->SetLED(&pDev->rsrc, tmp0, tmp1, tmp2, tmp3);
				strcat(str,RESPONSE_OK);
				strcat(str,pDev->rsrc.name);
				packetAddTo(packetOut, "%s%s%u%u%u%u", str, "setled", tmp0, tmp1, tmp2, tmp3);
			}
			else{
				strcat(str,RESPONSE_ERR);
				strcat(str,pDev->rsrc.name);
				packetAddTo(packetOut, "%s%s%s", str, "setled", "Overfloat");
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
