/**********************************************************
filename: fbusb16x1.c
**********************************************************/

/************************包含头文件***************************************************/
#include "fbusb16x1.h"
#include "publicRsrc.h"

#define MBI5024COLOR_OFF 0
#define MBI5024COLOR_RED 1
#define MBI5024COLOR_BLE 2
#define MBI5024COLOR_GRN 3

const u16 COLOR[4]={0,2,4,1};

static const u32 USB_CFG[16] = {
	0xfffffff2,	//0	1111 1111 1111 1111 1111 1111 1111 0010
	0xfffffff9,	//1	1111 1111 1111 1111 1111 1111 1111 1001
	0xffffff2f,	//2	1111 1111 1111 1111 1111 1111 0010 1111
	0xffffff9f,	//3	1111 1111 1111 1111 1111 1111 1001 1111
	0xfffff2ff,	//4	1111 1111 1111 1111 1111 0010 1111 1111
	0xfffff9ff,	//5	1111 1111 1111 1111 1111 1001 1111 1111
	0xffff2fff,	//6	1111 1111 1111 1111 0010 1111 1111 1111
	0xffff9fff,	//7	1111 1111 1111 1111 1001 1111 1111 1111
	0xfff2ffff,	//8	1111 1111 1111 0010 1111 1111 1111 1111
	0xfff9ffff,	//9	1111 1111 1111 1001 1111 1111 1111 1111
	0xff2fffff,	//a	1111 1111 0010 1111 1111 1111 1111 1111
	0xff9fffff,	//b	1111 1111 1001 1111 1111 1111 1111 1111
	0xf2ffffff,	//c	1111 0010 1111 1111 1111 1111 1111 1111
	0xf9ffffff,	//d	1111 1001 1111 1111 1111 1111 1111 1111
	0x2fffffff,	//e	0010 1111 1111 1111 1111 1111 1111 1111
	0x9fffffff,	//f	1001 1111 1111 1111 1111 1111 1111 1111
};

static const u32 USB_CFG_PWRON[16] = {
	0xccccccc0,	//0	1100 1100 1100 1100 1100 1100 1100 0000
	0xccccccc8,	//1	1100 1100 1100 1100 1100 1100 1100 1000
	0xcccccc0c,	//2	1100 1100 1100 1100 1100 1100 0000 1100
	0xcccccc8c,	//3	1100 1100 1100 1100 1100 1100 1000 1100
	0xccccc0cc,	//4	1100 1100 1100 1100 1100 0000 1100 1100
	0xccccc8cc,	//5	1100 1100 1100 1100 1100 1000 1100 1100
	0xcccc0ccc,	//6	1100 1100 1100 1100 0000 1100 1100 1100
	0xcccc8ccc,	//7	1100 1100 1100 1100 1000 1100 1100 1100
	0xccc0cccc,	//8	1100 1100 1100 0000 1100 1100 1100 1100
	0xccc8cccc,	//9	1100 1100 1100 1000 1100 1100 1100 1100
	0xcc0ccccc,	//a	1100 1100 0000 1100 1100 1100 1100 1100
	0xcc8ccccc,	//b	1100 1100 1000 1100 1100 1100 1100 1100
	0xc0cccccc,	//c	1100 0000 1100 1100 1100 1100 1100 1100
	0xc8cccccc,	//d	1100 1000 1100 1100 1100 1100 1100 1100
	0x0ccccccc,	//e	0000 1100 1100 1100 1100 1100 1100 1100
	0x8ccccccc,	//f	1000 1100 1100 1100 1100 1100 1100 1100
};


/**********************************************************
 Private function
**********************************************************/
static void setUSB(FBUSB16X1_Rsrc_T*, u8 brdIndx, u8 usbIndx, u8 pwr_on);
static void setLED(FBUSB16X1_Rsrc_T*, u8 brdIndx, u8 portIndx, u8 ledIndx, u8 color);
static u8 getUSB(FBUSB16X1_Rsrc_T*, u8 brdIndx);
static u8 getLED(FBUSB16X1_Rsrc_T*, u8 brdIndx, u8 ledIndx);
static void polling(FBUSB16X1_Rsrc_T*);
static void selBoard(FBUSB16X1_Rsrc_T*, u8 brdIndx);
static void makeLedLck(FBUSB16X1_Rsrc_T*, u8 lckBuf);
/**********************************************************
 Public function
**********************************************************/
DEV_STATUS FBUSB16X1_Setup(FBUSB16X1_Dev_T *pDev, DEV_PORT* port){
	FBUSB16X1_Rsrc_T* pRsrc = &pDev->rsrc;
	u8 buf[5];
	//initial resource
	memset(pRsrc, 0, sizeof(FBUSB16X1_Rsrc_T));
	//config
	pRsrc->ADDR[0] = port->PC0;
	pRsrc->ADDR[1] = port->PC1;
	pRsrc->ADDR[2] = port->PC2;
	pRsrc->ADDR[3] = port->PC3;
	pRsrc->HC595LCK1 = port->PB1;
	pRsrc->HC595LCK2 = port->PA2;
	pRsrc->HC595SCK = port->PB0;
	pRsrc->HC595DAT = port->PA0;
	pRsrc->LED_SCK = port->PB2;
	pRsrc->LED_DAT = port->PB3;
	pRsrc->RUNNING_LED = port->PA1;
	PIN_OP.As_OUTPUT_PP_NOPULL_HIGH(pRsrc->ADDR[0]);
	PIN_OP.As_OUTPUT_PP_NOPULL_HIGH(pRsrc->ADDR[1]);
	PIN_OP.As_OUTPUT_PP_NOPULL_HIGH(pRsrc->ADDR[2]);
	PIN_OP.As_OUTPUT_PP_NOPULL_HIGH(pRsrc->ADDR[3]);
	PIN_OP.As_OUTPUT_PP_NOPULL_LOW(pRsrc->HC595LCK1);
	PIN_OP.As_OUTPUT_PP_NOPULL_LOW(pRsrc->HC595LCK2);
	PIN_OP.As_OUTPUT_PP_NOPULL_LOW(pRsrc->HC595SCK);
	PIN_OP.As_OUTPUT_PP_NOPULL_LOW(pRsrc->HC595DAT);
	PIN_OP.As_OUTPUT_PP_NOPULL_LOW(pRsrc->LED_SCK);
	PIN_OP.As_OUTPUT_PP_NOPULL_LOW(pRsrc->LED_DAT);
	PIN_OP.As_OUTPUT_OD_NOPULL_HIGH(pRsrc->RUNNING_LED);//As_OUTPUT_OD_NOPULL_HIGH
	hc595DevSetup(&pRsrc->hc595, pRsrc->HC595LCK1, pRsrc->HC595SCK, pRsrc->HC595DAT, MSB_FIRST);
	//ops
	pDev->SetUSB = setUSB;
	pDev->SetLED = setLED;
	pDev->GetUSB = getUSB;
	pDev->GetLED = getLED;
	pDev->Polling = polling;
	
	//first switch off al
	buf[0] = 0xff;
	buf[1] = 0xff;
	buf[2] = 0xff;
	buf[3] = 0xff;
	buf[4] = 0;
	pRsrc->hc595.ShftBuf(&pRsrc->hc595.rsrc, buf, 5);
	makeLedLck(pRsrc,0);
	return DEV_SUCCESS;
}

/**********************************************************
 Public function
**********************************************************/
static void setUSB(FBUSB16X1_Rsrc_T* pRsrc, u8 brdIndx, u8 usbIndx, u8 pwr_on){
	u8 buf[4];
	u32 usbCfg;

	if(brdIndx<16 && (usbIndx<16 || usbIndx==0xff))	selBoard(pRsrc,brdIndx);	//select brd	
	else{ return;	}

	if(pwr_on){
		if(usbIndx == 0xff)	usbCfg = 0xcccccccc;
		else	usbCfg = USB_CFG_PWRON[usbIndx];		//when swith to another usb, left its power on.
		//first switch off al
		buf[0] = 0xcc;
		buf[1] = 0xcc;
		buf[2] = 0xcc;
		buf[3] = 0xcc;
		pRsrc->hc595.ShftBuf(&pRsrc->hc595.rsrc, buf, 4);
	}
	else{
		if(usbIndx == 0xff)	usbCfg = 0xffffffff;
		else	usbCfg = USB_CFG[usbIndx];
		//first switch off al
		buf[0] = 0xff;
		buf[1] = 0xff;
		buf[2] = 0xff;
		buf[3] = 0xff;
		pRsrc->hc595.ShftBuf(&pRsrc->hc595.rsrc, buf, 4);
	}
	//turn on one
	pRsrc->UsbCfg[brdIndx] = usbCfg;
	buf[0] = usbCfg & 0xff;	usbCfg >>= 8;
	buf[1] = usbCfg & 0xff;	usbCfg >>= 8;
	buf[2] = usbCfg & 0xff;	usbCfg >>= 8;
	buf[3] = usbCfg & 0xff;	
	pRsrc->hc595.ShftBuf(&pRsrc->hc595.rsrc, buf, 4);
	PIN_OP.Write(pRsrc->ADDR[0], GPIO_PIN_SET);
	PIN_OP.Write(pRsrc->ADDR[1], GPIO_PIN_SET);
	PIN_OP.Write(pRsrc->ADDR[2], GPIO_PIN_SET);
	PIN_OP.Write(pRsrc->ADDR[3], GPIO_PIN_SET);
}

static void setLED(FBUSB16X1_Rsrc_T* pRsrc, u8 brdIndx, u8 portIndx, u8 ledIndx, u8 cIndx){
	u8 i;
	u16 tmp16b;
	
	if(brdIndx>=16 || portIndx>=8 || ledIndx>=4 || cIndx>=4)	return;
	selBoard(pRsrc,brdIndx);	//select brd

	//before shift 
	pRsrc->LedPortColor[brdIndx][portIndx] &= (0x03<<ledIndx*2)^0xff;
	pRsrc->LedPortColor[brdIndx][portIndx] |= (cIndx<<ledIndx*2);
	i = pRsrc->LedPortColor[brdIndx][portIndx];	
	tmp16b = 0xfe00;
	//1st led
	tmp16b |= COLOR[i&0x03];
	//2nd led
	i >>= 2;
	tmp16b |= COLOR[i&0x03]<<3;
	//3rd led
	i >>= 2;
	tmp16b |= COLOR[i&0x03]<<6;
	//shift led data
	for(i=0;i<16;i++){
		if(tmp16b&0x8000)	HAL_GPIO_WritePin(pRsrc->LED_DAT.GPIOx, pRsrc->LED_DAT.GPIO_Pin, GPIO_PIN_SET);
		else							HAL_GPIO_WritePin(pRsrc->LED_DAT.GPIOx, pRsrc->LED_DAT.GPIO_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(pRsrc->LED_SCK.GPIOx, pRsrc->LED_SCK.GPIO_Pin, GPIO_PIN_SET);	
		HAL_GPIO_WritePin(pRsrc->LED_SCK.GPIOx, pRsrc->LED_SCK.GPIO_Pin, GPIO_PIN_RESET);
		tmp16b <<= 1;
	}
	makeLedLck(pRsrc, 1<<portIndx);		/* LCK SEND A PULSE */
	PIN_OP.Write(pRsrc->ADDR[0], GPIO_PIN_SET);
	PIN_OP.Write(pRsrc->ADDR[1], GPIO_PIN_SET);
	PIN_OP.Write(pRsrc->ADDR[2], GPIO_PIN_SET);
	PIN_OP.Write(pRsrc->ADDR[3], GPIO_PIN_SET);
}

static u8 getUSB(FBUSB16X1_Rsrc_T* pRsrc, u8 brdIndx){
	u8 i;
	if(brdIndx>=16 )	return 0xff;
	for(i=0;i<16;i++)	if(USB_CFG[i] == pRsrc->UsbCfg[brdIndx])	return i;
	return 0xff;
}

static u8 getLED(FBUSB16X1_Rsrc_T* pRsrc, u8 brdIndx, u8 ledIndx){
	return 0;
}

void polling(FBUSB16X1_Rsrc_T* pRsrc){
	if(++pRsrc->tick > 32){
		pRsrc->tick = 0;
		PIN_OP.Toggle(pRsrc->RUNNING_LED);
	}
}

static void selBoard(FBUSB16X1_Rsrc_T* pRsrc, u8 brdIndx){
	u8 i,j;
	j = brdIndx;
	for(i=0;i<4;i++){
		if(j & 0x01)	PIN_OP.Write(pRsrc->ADDR[i], GPIO_PIN_SET);
		else 					PIN_OP.Write(pRsrc->ADDR[i], GPIO_PIN_RESET);
		j>>=1;
	}
}

static void makeLedLck(FBUSB16X1_Rsrc_T* pRsrc, u8 lckBuf){
	u8 i,j;
	/* LCK SEND A PULSE */
	j = lckBuf;
	for(i=0;i<8;i++){
		if(j&0x80)	HAL_GPIO_WritePin(pRsrc->HC595DAT.GPIOx, pRsrc->HC595DAT.GPIO_Pin, GPIO_PIN_SET);
		else						HAL_GPIO_WritePin(pRsrc->HC595DAT.GPIOx, pRsrc->HC595DAT.GPIO_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(pRsrc->HC595SCK.GPIOx, pRsrc->HC595SCK.GPIO_Pin, GPIO_PIN_SET);	
		HAL_GPIO_WritePin(pRsrc->HC595SCK.GPIOx, pRsrc->HC595SCK.GPIO_Pin, GPIO_PIN_RESET);
		j <<= 1;
	}
	HAL_GPIO_WritePin(pRsrc->HC595LCK2.GPIOx, pRsrc->HC595LCK2.GPIO_Pin, GPIO_PIN_SET);	NOP();NOP();
	HAL_GPIO_WritePin(pRsrc->HC595LCK2.GPIOx, pRsrc->HC595LCK2.GPIO_Pin, GPIO_PIN_RESET);
	
	if(lckBuf){
		for(i=0;i<8;i++){
			HAL_GPIO_WritePin(pRsrc->HC595DAT.GPIOx, pRsrc->HC595DAT.GPIO_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(pRsrc->HC595SCK.GPIOx, pRsrc->HC595SCK.GPIO_Pin, GPIO_PIN_SET);	
			HAL_GPIO_WritePin(pRsrc->HC595SCK.GPIOx, pRsrc->HC595SCK.GPIO_Pin, GPIO_PIN_RESET);
			j <<= 1;
		}
		HAL_GPIO_WritePin(pRsrc->HC595LCK2.GPIOx, pRsrc->HC595LCK2.GPIO_Pin, GPIO_PIN_SET);	NOP();NOP();
		HAL_GPIO_WritePin(pRsrc->HC595LCK2.GPIOx, pRsrc->HC595LCK2.GPIO_Pin, GPIO_PIN_RESET);		
	}

}

/**********************************************************
 == THE END ==
**********************************************************/
