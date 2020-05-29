/*************************************************************
 fileName: RM68042.c
 
*************************************************************/
#include "rm68042.h"
#include "publicRsrc.h"
//#include "myuart_os.h"

//commands list:
//00h  //nop 
#define CMD_SOFT_RESET	0x01  //soft_reset 
#define	get_display_ID 	0x04
#define	get_DSI_err 	0x05
#define	read_display_status 	0x09
#define	get_power_mode 	0x0A
#define	get_address_mode 	0x0B
#define	get_pixel_format 	0x0C
#define	get_display_mode 	0x0D
#define	get_signal_mode 	0x0E
#define	get_diagnostic_result 	0x0F
#define	enter_sleep_mode 	0x10
#define CMD_exit_sleep_mode 0x11  
#define	enter_partial_mode 	0x12
#define	enter_normal_mode 	0x13
#define	exit_invert_mode 	0x20
#define	enter_invert_mode 	0x21
#define	set_display_off 	0x28
#define	set_display_on 	0x29
#define	set_column_address 	0x2A
#define	set_page_address 	0x2B
#define	write_memory_start 	0x2C
#define	read_memory_start 	0x2E
#define	set_partial_area 	0x30
#define	set_scroll_area 	0x33
#define	set_tear_off 	0x34
#define	set_tear_on 	0x35
#define	set_address_mode 	0x36
#define	exit_idle_mode 	0x38
#define	enter_idle_mode 	0x39
#define	set_pixel_format 	0x3A
#define	write_memory _continue 	0x3C
#define	read_memory _continue 	0x3E
#define	set_tear_scanline 	0x44
#define	get_scanline 	0x45
#define	set_display_brightness 	0x51
#define	get_display_brightness 	0x52
#define	set_control_display	0x53
#define	get_control_display 	0x54
#define	set_cabc_mode 	0x55
#define	get_cabc_mode 	0x56
#define	set_cabc_min_brightness 	0x5E
#define	get_cabc_min_brightness 	0x5F
#define	read_first_checksum 	0xAA
#define	read_continue_checksum 	0xAF
#define	read_ID1 	0xDA
#define	read_ID2 	0xDB
#define	read_ID3 	0xDC
#define	Interface_mode_control 	0xB0
#define	frame_rate_control_nomal 	0xB1
#define	frame_rate_control_idle 	0xB2
#define	frame_rate_control_partial 	0xB3
#define	display_inversion_control 	0xB4
#define	blanking_porch_control	0xB5
#define	display_function_control 	0xB6
#define	entry_mode_set 	0xB7
#define	device_code_read 	0xBF
#define CMD_power_control_1 0xC0  //power_control_1 
#define CMD_power_control_2 0xC1  //power_control_2 
#define CMD_power_control_3 0xC2  //power_control_3 
#define	power_control_4	0xC4
#define	vcom_control_1 	0xC5
#define	nv_memory_write 	0xD0
#define	nv_memory_protection_key 	0xD1
#define	nv_memory_status_read 	0xD2
#define	read_ID4 	0xD3
#define	Gamma_setting 	0xE0

/*************************************************************
 Public Var
*************************************************************/
//LCD IO functions 
static void rm68140Delay(u32 nCount);
static void rm68140IO(rm68140Rsrc_T* pRsrc, u8 reg, u8 *pDat, u8 nBytes);
static void rm68140Read(rm68140Rsrc_T* pRsrc, u8 reg, u8 *pDat, u8 nBytes);
static void rm68140Write(rm68140Rsrc_T* pRsrc, u8 reg, u8 *pDat, u8 nBytes);

//Exported_Functions
static void rm68140Reset(rm68140Rsrc_T*, u16 xSize, u16 ySize);
static u16 rm68140_ReadID(rm68140Rsrc_T*);

//static void rm68140_WriteReg(rm68140Rsrc_T*, uint8_t LCD_Reg, u16 LCD_RegValue);
//u16 rm68140_ReadReg(uint8_t LCD_Reg);

static void rm68140_DisplayOn(rm68140Rsrc_T*);
static void rm68140_DisplayOff(rm68140Rsrc_T*);
//static void rm68140_SetCursor(rm68140Rsrc_T*, u16 Xpos, u16 Ypos);
static s8 rm68140WritePixel(rm68140Rsrc_T*, u16 Xpos, u16 Ypos, u16 RGB_Code);
static s8 rm68140DrawHLine(rm68140Rsrc_T*, u16 Xpos, u16 Ypos, u16 Length, u16 RGB_Code);
static s8 rm68140DrawVLine(rm68140Rsrc_T*, u16 Xpos, u16 Ypos, u16 Length, u16 RGB_Code);
static s8 rm68140FillColor(rm68140Rsrc_T *pRsrc, u16 x0, u16 x1, u16 y0, u16 y1, u16 color);
static s8 rm68140DrawImage(rm68140Rsrc_T *pRsrc, u16 x, u16 y, const myImg_t* img);
static s8 rm68140FullScreen	(rm68140Rsrc_T *pRsrc, u16 color);
static s8 rm68140DrawPartern	(rm68140Rsrc_T *pRsrc, u16 x, u16 y);

static s8 rm68140FillRectImage(rm68140Rsrc_T *pRsrc, u16 x0, u16 x1, u16 y0, u16 y1, const myImg_t* img);
static s8 rm68140SetWindow(rm68140Rsrc_T *pRsrc, u16 x0, u16 x1, u16 y0, u16 y1);

/*************************************************************
 Setup a LCD Device
*************************************************************/
s8 Rm68140Setup(
	rm68140Dev_T *pDev,
	PIN_T RS, 		//Resister select pin
	PIN_T CS, 		//SPI select pin
	SPI_HandleTypeDef *SPI_HANDLE,
	u16 xLen, u16 yLen
){
	pDev->rsrc.SPI_HANDLE = SPI_HANDLE;
	pDev->rsrc.REG_SEL = RS;
	pDev->rsrc.SPI_CS = CS;
	//register op
	pDev->Reset = rm68140Reset;
	pDev->ReadID = rm68140_ReadID;
	pDev->DisplayOn = rm68140_DisplayOn;
	pDev->DisplayOff = rm68140_DisplayOff;

	pDev->WritePixel = rm68140WritePixel;
	pDev->DrawHLine = rm68140DrawHLine;
	pDev->DrawVLine = rm68140DrawVLine;
	pDev->DrawImage = rm68140DrawImage;
	pDev->DrawBlock = rm68140FillColor;
	pDev->DrawPartern = rm68140DrawPartern;
	pDev->FullScreen = rm68140FullScreen;
	
  /* SPI1 parameter re-configuration*/
  SPI_HANDLE->Init.Mode = SPI_MODE_MASTER;
  SPI_HANDLE->Init.Direction = SPI_DIRECTION_2LINES;
  SPI_HANDLE->Init.DataSize = SPI_DATASIZE_8BIT;
  SPI_HANDLE->Init.CLKPolarity = SPI_POLARITY_LOW;
  SPI_HANDLE->Init.CLKPhase = SPI_PHASE_1EDGE;
  SPI_HANDLE->Init.NSS = SPI_NSS_SOFT;
  SPI_HANDLE->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  SPI_HANDLE->Init.FirstBit = SPI_FIRSTBIT_MSB;
  SPI_HANDLE->Init.TIMode = SPI_TIMODE_DISABLE;
  SPI_HANDLE->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  SPI_HANDLE->Init.CRCPolynomial = 10;
		
  if (HAL_SPI_Init(SPI_HANDLE) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
		return -1;
  }

	PIN_OP.As_OUTPUT_PP_NOPULL_HIGH(RS);
	PIN_OP.As_OUTPUT_PP_NOPULL_HIGH(CS);
	
	rm68140Reset(&pDev->rsrc, xLen, yLen);
	
	return 0;
}

/*************************************************************
*************************************************************/
static void rm68140Delay(u32 nCount){
	volatile int i;
	for(i=0;i<720;i++)
    for(; nCount != 0; nCount--);
}

/*************************************************************

*************************************************************/
static void rm68140IO(rm68140Rsrc_T* pRsrc, u8 cmd, u8 *pDat, u8 nBytes){
	//spi enable
	HAL_GPIO_WritePin(pRsrc->SPI_CS.GPIOx, pRsrc->SPI_CS.GPIO_Pin, GPIO_PIN_RESET);
	//Send command stage
	HAL_GPIO_WritePin(pRsrc->REG_SEL.GPIOx, pRsrc->REG_SEL.GPIO_Pin, GPIO_PIN_RESET);
	//transmit command
	while (HAL_SPI_GetState(pRsrc->SPI_HANDLE) != HAL_SPI_STATE_READY){ }
	HAL_SPI_Transmit(pRsrc->SPI_HANDLE, &cmd, 1, HAL_MAX_DELAY);

	if(nBytes){
		//Send data stage
		HAL_GPIO_WritePin(pRsrc->REG_SEL.GPIOx, pRsrc->REG_SEL.GPIO_Pin, GPIO_PIN_SET);
		while (HAL_SPI_GetState(pRsrc->SPI_HANDLE) != HAL_SPI_STATE_READY){}
		HAL_SPI_TransmitReceive(pRsrc->SPI_HANDLE, pDat, pDat, nBytes, 10000);
	}
	//spi disable
	HAL_GPIO_WritePin(pRsrc->SPI_CS.GPIOx, pRsrc->SPI_CS.GPIO_Pin, GPIO_PIN_SET);
}

static void rm68140Write(rm68140Rsrc_T* pRsrc, u8 cmd, u8 *pDat, u8 nBytes){
	//spi enable
	HAL_GPIO_WritePin(pRsrc->SPI_CS.GPIOx, pRsrc->SPI_CS.GPIO_Pin, GPIO_PIN_RESET);
	//Send command stage
	HAL_GPIO_WritePin(pRsrc->REG_SEL.GPIOx, pRsrc->REG_SEL.GPIO_Pin, GPIO_PIN_RESET);
	//transmit command
	while (HAL_SPI_GetState(pRsrc->SPI_HANDLE) != HAL_SPI_STATE_READY){ }
	HAL_SPI_Transmit(pRsrc->SPI_HANDLE, &cmd, 1, HAL_MAX_DELAY);

	if(nBytes){
		//Send data stage
		HAL_GPIO_WritePin(pRsrc->REG_SEL.GPIOx, pRsrc->REG_SEL.GPIO_Pin, GPIO_PIN_SET);
		while (HAL_SPI_GetState(pRsrc->SPI_HANDLE) != HAL_SPI_STATE_READY){}
		HAL_SPI_Transmit(pRsrc->SPI_HANDLE, pDat, nBytes, 10000);
	}
	//spi disable
	HAL_GPIO_WritePin(pRsrc->SPI_CS.GPIOx, pRsrc->SPI_CS.GPIO_Pin, GPIO_PIN_SET);
}

static void rm68140Read(rm68140Rsrc_T* pRsrc, u8 cmd, u8 *pDat, u8 nBytes){
	//spi enable
	HAL_GPIO_WritePin(pRsrc->SPI_CS.GPIOx, pRsrc->SPI_CS.GPIO_Pin, GPIO_PIN_RESET);
	//Send command stage
	HAL_GPIO_WritePin(pRsrc->REG_SEL.GPIOx, pRsrc->REG_SEL.GPIO_Pin, GPIO_PIN_RESET);
	//transmit command
	while (HAL_SPI_GetState(pRsrc->SPI_HANDLE) != HAL_SPI_STATE_READY){ }
	HAL_SPI_Transmit(pRsrc->SPI_HANDLE, &cmd, 1, HAL_MAX_DELAY);

	if(nBytes){
		//Send data stage
		HAL_GPIO_WritePin(pRsrc->REG_SEL.GPIOx, pRsrc->REG_SEL.GPIO_Pin, GPIO_PIN_SET);
		while (HAL_SPI_GetState(pRsrc->SPI_HANDLE) != HAL_SPI_STATE_READY){}
		HAL_SPI_Receive(pRsrc->SPI_HANDLE, pDat, nBytes, 10000);
	}
	//spi disable
	HAL_GPIO_WritePin(pRsrc->SPI_CS.GPIOx, pRsrc->SPI_CS.GPIO_Pin, GPIO_PIN_SET);
}

/*************************************************************
*************************************************************/
static void rm68140Reset(rm68140Rsrc_T* pRsrc, u16 xSize, u16 ySize){
	u8 cmd,ram[16];
	//soft_reset
	cmd = CMD_SOFT_RESET;
	rm68140Write(pRsrc, cmd, NULL, 0);
	rm68140Delay(100);

	//exit_sleep_mode 
	cmd = CMD_exit_sleep_mode;
	rm68140Write(pRsrc,cmd, NULL, 0);
	rm68140Delay(20);

//************* Start Initial Sequence **********//
	ram[0] = (0x0A); // P-Gamma level
	ram[1] = (0x0A); // N-Gamma level
	rm68140Write(pRsrc, 0xC0, ram, 2);

	ram[0] = (0x41);
	ram[1] = (0x07); // VCI1 = 2.5V
	rm68140Write(pRsrc, 0xC1, ram, 2);

	ram[0] = (0x33);
	rm68140Write(pRsrc, 0xC2, ram, 1);
	
	ram[0] = (0x00);
	ram[1] = (0x42); // VCM Setting
	ram[2] = (0x80); // VCM Register Enable
	rm68140Write(pRsrc, 0xC5, ram, 3);
	
	ram[0] = (0xB0); // Frame Rate Setting
	ram[1] = (0x11);
	rm68140Write(pRsrc, 0xB1, ram, 2);
	
	ram[0] = (0x00); // Frame Rate Setting
	rm68140Write(pRsrc, 0xB4, ram, 1);

	ram[0] = (0x00);
	ram[1] = (0x02);
	ram[2] = (0x3B);
	rm68140Write(pRsrc, 0xB6, ram, 3);

	ram[0] = (0x07);
	rm68140Write(pRsrc, 0xB7, ram, 1);

	ram[0] = (0x36);
	ram[1] = (0xA5);
	ram[2] = (0xD3);
	rm68140Write(pRsrc, 0xF0, ram, 3);

	ram[0] = (0x80);
	rm68140Write(pRsrc, 0xE5, ram, 1);

	ram[0] = (0x01);
	rm68140Write(pRsrc, 0xE5, ram, 1);

	ram[0] = (0x00);
	rm68140Write(pRsrc, 0XB3, ram, 1);

	ram[0] = (0x00);
	rm68140Write(pRsrc, 0xE5, ram, 1);

	ram[0] = (0x36);
	ram[1] = (0xA5);
	ram[2] = (0x53);
	rm68140Write(pRsrc, 0xF0, ram, 3);

	ram[0] = (0x00);
	ram[1] = (0x35);
	ram[2] = (0x33);
	ram[3] = (0x00);
	ram[4] = (0x00);
	ram[5] = (0x00);
	ram[6] = (0x00);
	ram[7] = (0x35);
	ram[8] = (0x33);
	ram[9] = (0x00);
	ram[10] = (0x00);
	ram[11] = (0x00);
	rm68140Write(pRsrc, 0xE0, ram, 12);

	ram[0] = (0x08);
	rm68140Write(pRsrc, 0x36, ram, 1);

	ram[0] = (0x00);
	rm68140Write(pRsrc, 0xEE, ram, 1);
	//16Bit
	ram[0] = (0x55);	
	rm68140Write(pRsrc, 0x3A, ram, 1);
	
	rm68140Write(pRsrc, 0x11, NULL, 0);	//Sleep Out 
	rm68140Write(pRsrc, 0x13, NULL, 0);	//Normal Display Mode On 
	rm68140Write(pRsrc, 0x20, NULL, 0);	//Display Inversion Off
	rm68140Write(pRsrc, set_display_on, NULL, 0);
	rm68140Delay(10);
	
	pRsrc->xLen = xSize;
	pRsrc->yLen = ySize;
	
	if(pRsrc->xLen > pRsrc->yLen) 	ram[0] = (0x20); //ºáÆÁ
	else ram[0] = (0x40); //ºáÆÁ
	rm68140Write(pRsrc, 0x36, ram, 1);

//	if(pRsrc->xLen > pRsrc->yLen){
//	//horizontal test
//		rm68140DrawImage(pRsrc, 0, 50, &F_PARTERN);
//		rm68140DrawImage(pRsrc, 50, 50, &F_PARTERN);
//		rm68140DrawImage(pRsrc, 100, 50, &F_PARTERN);
//		rm68140DrawImage(pRsrc, 150, 50, &F_PARTERN);
//		rm68140DrawImage(pRsrc, 200, 50, &F_PARTERN);
//		rm68140DrawImage(pRsrc, 250, 50, &F_PARTERN);
//		rm68140DrawImage(pRsrc, 300, 50, &F_PARTERN);
//		rm68140DrawImage(pRsrc, 350, 50, &F_PARTERN);
//		rm68140DrawImage(pRsrc, 400, 50, &F_PARTERN);
//		rm68140DrawImage(pRsrc, 450, 280, &F_PARTERN);
//	}
//	else{
//		//vertical test
//		rm68140DrawImage(pRsrc, 0, 50, &F_PARTERN);
//		rm68140DrawImage(pRsrc, 50, 50, &F_PARTERN);
//		rm68140DrawImage(pRsrc, 100, 50, &F_PARTERN);
//		rm68140DrawImage(pRsrc, 150, 50, &F_PARTERN);
//		rm68140DrawImage(pRsrc, 200, 50, &F_PARTERN);
//		rm68140DrawImage(pRsrc, 250, 420, &F_PARTERN);
//		rm68140DrawImage(pRsrc, 300, 420, &F_PARTERN);
//		rm68140DrawImage(pRsrc, 350, 420, &F_PARTERN);
//		rm68140DrawImage(pRsrc, 400, 420, &F_PARTERN);
//		rm68140DrawImage(pRsrc, 450, 420, &F_PARTERN);
//	}

}

/*************************************************************
*************************************************************/
static u16 rm68140_ReadID(rm68140Rsrc_T* pRsrc){
	u8 cmd,ram[3];
	cmd = 0x04;
	rm68140IO(pRsrc, cmd, ram, 3);
	return (((u16)ram[1]<<8) | ram[2]);
}

/*************************************************************
*************************************************************/
static void rm68140_DisplayOn(rm68140Rsrc_T* pRsrc){
	u8 cmd = 0x29;
	rm68140Write(pRsrc, cmd, NULL, 0);
}

/*************************************************************
*************************************************************/
static void rm68140_DisplayOff(rm68140Rsrc_T* pRsrc){
	u8 cmd = 0x28;
	rm68140Write(pRsrc, cmd, NULL, 0);
}

/*************************************************************
*************************************************************/
static s8 rm68140WritePixel(rm68140Rsrc_T* pRsrc, u16 x, u16 y, u16 RGB_Code){
	u8 cmd,ram[2];
	
	if(pRsrc->xLen > pRsrc->yLen)	{	if(rm68140SetWindow(pRsrc, x, x, y, y)<0)	return -1;	}
	else													{	if(rm68140SetWindow(pRsrc, (pRsrc->xLen-1-x), (pRsrc->xLen-1-x), y, y)<0)	return -1;	}

	
//	rm68140SetWindow(pRsrc, Xpos, Xpos, Ypos, Ypos);
	cmd = write_memory_start;
	ram[0] = RGB_Code>>8;
	ram[1] = RGB_Code;
	rm68140Write(pRsrc, cmd, &ram[0], 2);
	return 0;
}

/*************************************************************
*************************************************************/
static s8 rm68140DrawHLine(rm68140Rsrc_T* pRsrc, u16 Xpos, u16 Ypos, u16 Length, u16 RGB_Code){
	return (rm68140FillColor(pRsrc, Xpos, Xpos+Length-1, Ypos, Ypos, RGB_Code));
}

static s8 rm68140DrawVLine(rm68140Rsrc_T* pRsrc, u16 Xpos, u16 Ypos, u16 Length, u16 RGB_Code){
	return (rm68140FillColor(pRsrc, Xpos, Xpos, Ypos, Ypos+Length-1, RGB_Code));
}

///*************************************************************
//Public FunctionsS

///*************************************************************
// Read driver ID
//*************************************************************/
//u16 rm68140ReadID(rm68140Rsrc_T *pRsrc){
//	u16 rtn;
//	return rtn;
//}
//void rm68140InitSPI(rm68140Rsrc_T *pRsrc){
//}

///*************************************************************
// display on
//*************************************************************/
//void rm68140DispOn(rm68140Rsrc_T *pRsrc){
//	rm68140WriteReg(pRsrc, 0x29);	
//	rm68140Delay(1);	
//}

///*************************************************************
// display off
//*************************************************************/
//void rm68140DispOff(rm68140Rsrc_T *pRsrc){
//	rm68140WriteReg(pRsrc, 0x28);	
//	rm68140Delay(1);	
//}

///*************************************************************
// Print a dot with specify color
//*************************************************************/
//int32_t rm68140SetPixel(rm68140Rsrc_T *pRsrc, u16 x, u16 y, u16 color){
//	int32_t rtn=0;
//	u8 tmp[4];

//	if(x > X_SIZE)	return RM68140_ERR;
//	if(y > Y_SIZE)	return RM68140_ERR;
//	//Set_column_address 
//	rm68140WriteReg(pRsrc, 0x2a);  		
//	tmp[0] = x>>8;	tmp[1] = x&0xff;	tmp[2] = x>>8;	tmp[3] = x&0xff;	
//	rm68140WriteRam(pRsrc, tmp, 4);	
//	//Set_page_address  
//	rm68140WriteReg(pRsrc, 0x2b);  		
//	tmp[0] = y>>8;	tmp[1] = y&0xff;	tmp[2] = y>>8;	tmp[3] = y&0xff;	
//	rm68140WriteRam(pRsrc, tmp, 4);	
//	//Set_page_address  
//	rm68140WriteReg(pRsrc, 0x2c);  		
//	tmp[0] = color;	
//	rm68140WriteRam(pRsrc, tmp, 1);	

//	return rtn;
//}

///*************************************************************
// get a dot's color
//*************************************************************/
//int32_t rm68140GetPixel(rm68140Rsrc_T *pRsrc, u16 x, u16 y){
//	int32_t rtn = 0;
//	u8 tmp[4];

//	if(x > X_SIZE)	return RM68140_ERR;
//	if(y > Y_SIZE)	return RM68140_ERR;
//	//Set_column_address 
//	rm68140WriteReg(pRsrc, 0x2a);  		
//	tmp[0] = x>>8;	tmp[1] = x&0xff;	tmp[2] = x>>8;	tmp[3] = x&0xff;	
//	rm68140WriteRam(pRsrc, tmp, 4);	
//	//Set_page_address  
//	rm68140WriteReg(pRsrc, 0x2b);  		
//	tmp[0] = y>>8;	tmp[1] = y&0xff;	tmp[2] = y>>8;	tmp[3] = y&0xff;	
//	rm68140WriteRam(pRsrc, tmp, 4);	

//	rm68140WriteReg(pRsrc, 0x2e);  		
//	tmp[0] = y>>8;	tmp[1] = y&0xff;	tmp[2] = y>>8;	tmp[3] = y&0xff;	
//	rm68140WriteRam(pRsrc, tmp, 4);	
//	
//	rm68140ReadRam(pRsrc, tmp, 4);	

//	return tmp[0];
//}

/*************************************************************
 fill rectangle with color
*************************************************************/
static s8 rm68140FillColor(rm68140Rsrc_T *pRsrc, u16 x0, u16 x1, u16 y0, u16 y1, u16 color){
	u16 i,j;
	u8 tmp[2];
	
	if(pRsrc->xLen > pRsrc->yLen)	{	if(rm68140SetWindow(pRsrc, x0, x1, y0, y1)<0)	return -1;	}
	else													{	if(rm68140SetWindow(pRsrc, (pRsrc->xLen-1-x1), (pRsrc->xLen-1-x0), y0, y1)<0)	return -1;	}

	//spi enable
	HAL_GPIO_WritePin(pRsrc->SPI_CS.GPIOx, pRsrc->SPI_CS.GPIO_Pin, GPIO_PIN_RESET);
	//Send command stage
	HAL_GPIO_WritePin(pRsrc->REG_SEL.GPIOx, pRsrc->REG_SEL.GPIO_Pin, GPIO_PIN_RESET);
	tmp[0] = write_memory_start;
	//transmit command
	while (HAL_SPI_GetState(pRsrc->SPI_HANDLE) != HAL_SPI_STATE_READY){ }
	HAL_SPI_Transmit(pRsrc->SPI_HANDLE, &tmp[0], 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(pRsrc->REG_SEL.GPIOx, pRsrc->REG_SEL.GPIO_Pin, GPIO_PIN_SET);

	for(i=0;i<(y1-y0+1);i++){
		for(j=0;j<(x1-x0+1);j++){
			tmp[0] = color>>8;
			tmp[1] = color;
			while (HAL_SPI_GetState(pRsrc->SPI_HANDLE) != HAL_SPI_STATE_READY){ }
			HAL_SPI_Transmit(pRsrc->SPI_HANDLE, &tmp[0], 2, HAL_MAX_DELAY);
		}
	}	
	return 0;
}


/*************************************************************
 Print a image
*************************************************************/
static s8 rm68140DrawImage(rm68140Rsrc_T *pRsrc, u16 x, u16 y, const myImg_t* img){
	u16 x0,x1,y0,y1;
	x0 = x;	
	x1 = x+img->xsize-1;
	y0 = y;
	y1 = y+img->ysize-1;
	if(x1 >= pRsrc->xLen)	x1 = pRsrc->xLen-1;
	if(y1 >= pRsrc->yLen)	y1 = pRsrc->yLen-1;
	return(rm68140FillRectImage(pRsrc, x0, x1, y0, y1, img));
}

/*************************************************************
 fill rectangle with image
*************************************************************/
static s8 rm68140FillRectImage(rm68140Rsrc_T *pRsrc, u16 x0, u16 x1, u16 y0, u16 y1, const myImg_t* img){
	u16 i,j;
	u8 tmp[2];
	
	if(pRsrc->xLen > pRsrc->yLen)	{	if(rm68140SetWindow(pRsrc, (x0), (x1), y0, y1)<0)	return -1;	}
	else													{	if(rm68140SetWindow(pRsrc, (pRsrc->xLen-1-x1), (pRsrc->xLen-1-x0), y0, y1)<0)	return -1;	}

	//spi enable
	HAL_GPIO_WritePin(pRsrc->SPI_CS.GPIOx, pRsrc->SPI_CS.GPIO_Pin, GPIO_PIN_RESET);
	//Send command stage
	HAL_GPIO_WritePin(pRsrc->REG_SEL.GPIOx, pRsrc->REG_SEL.GPIO_Pin, GPIO_PIN_RESET);
	tmp[0] = write_memory_start;
	//transmit command
	while (HAL_SPI_GetState(pRsrc->SPI_HANDLE) != HAL_SPI_STATE_READY){ }
	HAL_SPI_Transmit(pRsrc->SPI_HANDLE, &tmp[0], 1, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(pRsrc->REG_SEL.GPIOx, pRsrc->REG_SEL.GPIO_Pin, GPIO_PIN_SET);

	for(i=0;i<(y1-y0+1);i++){
		for(j=0;j<(x1-x0+1);j++){
			tmp[0] = img->pImg[i*(img->xsize)+j]>>8;
			tmp[1] = img->pImg[i*(img->xsize)+j];
			while (HAL_SPI_GetState(pRsrc->SPI_HANDLE) != HAL_SPI_STATE_READY){ }
			HAL_SPI_Transmit(pRsrc->SPI_HANDLE, &tmp[0], 2, HAL_MAX_DELAY);
		}
	}	

	return 0;
}

/*************************************************************
*************************************************************/
static s8 rm68140SetWindow(rm68140Rsrc_T *pRsrc, u16 x0, u16 x1, u16 y0, u16 y1){
	u8 tmp[4];
	
	if(x0>x1 || y0>y1)	return -1;
	if(x0 > pRsrc->xLen)	{	return -1;	}
	if(x1 > pRsrc->xLen)	{	return -2;	}
	if(y0 > pRsrc->yLen)	{	return -3;	}
	if(y1 > pRsrc->yLen)	{	return -4;	}
		
	tmp[0] = x0>>8;
	tmp[1] = x0&0xff;
	tmp[2] = x1>>8;
	tmp[3] = x1&0xff;
	rm68140Write(pRsrc, set_column_address, tmp, 4);
	
	tmp[0] = y0>>8;
	tmp[1] = y0&0xff;
	tmp[2] = y1>>8;
	tmp[3] = y1&0xff;
	rm68140Write(pRsrc, set_page_address, tmp, 4);

	return 0;
}

static s8 rm68140FullScreen	(rm68140Rsrc_T *pRsrc, u16 color){
	return(rm68140FillColor(pRsrc, 0, pRsrc->xLen-1, 0, pRsrc->yLen-1, color));
}

static s8 rm68140DrawPartern	(rm68140Rsrc_T *pRsrc, u16 x, u16 y){
	return(rm68140DrawImage(pRsrc, x, y, &F_PARTERN));
}

/*************************************************************

*************************************************************/
//int32_t rm68140DrawRect(uint8_t dotSize, u16 x0, u16 x1, u16 y0, u16 y1, u16 color){
//	int32_t rtn=0;
//	uint8_t dot;

//	while(1){
//		if(dotSize <= 0){	rtn = -1;	break;}
//		dot = dotSize-1;
//		//draw x0,x1
//		if(y0 >= y1){
//			if(rm68140FillRectColor(x0,x1,(y0-dot),y0,color)<0)	{rtn = -2;	break;}
//			if(rm68140FillRectColor(x0,x1,y1,(y1+dot),color)<0)	{rtn = -3;	break;}
//		}
//		else{
//			if(rm68140FillRectColor(x0,x1,y0,y0+dot,color)<0)	{rtn = -4;	break;}
//			if(rm68140FillRectColor(x0,x1,y1-dot,y1,color)<0)	{rtn = -5;	break;}
//		}
//		//draw y0,y1
//		if(x0 >= x1){
//			if(rm68140FillRectColor((x0-dot),x0,y0,y1,color)<0)	{rtn = -6;	break;}
//			if(rm68140FillRectColor(x1,(x1+dot),y0,y1,color)<0)	{rtn = -7;	break;}
//		}
//		else{
//			if(rm68140FillRectColor(x0,(x0+dot),y0,y1,color)<0)	{rtn = -8;	break;}
//			if(rm68140FillRectColor((x1-dot),x1,y0,y1,color)<0)	{rtn = -9;	break;}
//		}
//		break;
//	}
//	return rtn;
//}


//static void BlockWrite(rm68140Rsrc_T* pRsrc, unsigned int Xstart,unsigned int Xend,unsigned int Ystart,unsigned int Yend) 
//{
//	u8 tmp[4];
//	//HX8357-C
//   tmp[0] = (Xstart>>8);
//   tmp[1] = (Xstart);
//   tmp[2] = (Xend>>8);
//   tmp[3] = (Xend);
//	 rm68140Write(pRsrc, 0x2A, tmp, 4);
//  
//   tmp[0] = (Ystart>>8);
//   tmp[1] = (Ystart);
//   tmp[2] = (Yend>>8);
//	 tmp[3] = (Yend);
//	 rm68140Write(pRsrc, 0x2B, tmp, 4);
//}

///*************************************************************
//*************************************************************/
//void rm68140WriteReg(rm68140Rsrc_T *pRsrc, u8 reg)	{
//	//Register Select: reg
//	HAL_GPIO_WritePin(pRsrc->REG_SEL.GPIOx, pRsrc->REG_SEL.GPIO_Pin, GPIO_PIN_RESET);
//	//transmit data
//	while (HAL_SPI_GetState(pRsrc->SPI_HANDLE) != HAL_SPI_STATE_READY){ }
//	HAL_SPI_Transmit(pRsrc->SPI_HANDLE, &reg, 1, HAL_MAX_DELAY);
//}

///*************************************************************
//*************************************************************/
//void rm68140WriteRam(rm68140Rsrc_T *pRsrc, u8 *pDat, u8 nBytes){
//	//Register Select: reg
//	HAL_GPIO_WritePin(pRsrc->REG_SEL.GPIOx, pRsrc->REG_SEL.GPIO_Pin, GPIO_PIN_RESET);
//	//transmit data
//	while (HAL_SPI_GetState(pRsrc->SPI_HANDLE) != HAL_SPI_STATE_READY){ }
//	HAL_SPI_Transmit(pRsrc->SPI_HANDLE, pDat, nBytes, HAL_MAX_DELAY);
//}

///*************************************************************
//*************************************************************/
//void rm68140ReadRam(rm68140Rsrc_T *pRsrc, u8 *pDat, u8 nBytes)	{
//	//Register Select: reg
//	HAL_GPIO_WritePin(pRsrc->REG_SEL.GPIOx, pRsrc->REG_SEL.GPIO_Pin, GPIO_PIN_RESET);
//	//transmit data
//	while (HAL_SPI_GetState(pRsrc->SPI_HANDLE) != HAL_SPI_STATE_READY){ }
//	HAL_SPI_Receive(pRsrc->SPI_HANDLE, pDat, nBytes, HAL_MAX_DELAY);
//}
