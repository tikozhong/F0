/*************************************************************
 fileName: RM68042.c
 
*************************************************************/
#include "rm68042.h"

/*************************************************************
 Public Var
*************************************************************/
const STM32_PIN_RM68042 LCD_RESET = 			{GPIOD,GPIO_PIN_13};
const STM32_PIN_RM68042 LCD_BACKLIGHT = 	{GPIOA,GPIO_PIN_1};

/*************************************************************
 Private Var
*************************************************************/
static uint8_t _rm68042DimClk;		//dim clock, increase in sysStick, 0..15 avilid
static uint8_t _rm68042Dim;			//brightness
static uint8_t _rm68042DimTo;
static uint8_t _rm68042FadeClk;
/*************************************************************
 private functions
*************************************************************/
/*************************************************************
*************************************************************/
static void _rm68042Delay(uint32_t nCount){
	uint32_t i;
	for(i=0;i<nCount;i++);
}

/*************************************************************
*************************************************************/
static void _rm68042Reset(void){
	HAL_GPIO_WritePin(LCD_RESET.GPIOx, LCD_RESET.GPIO_Pin, GPIO_PIN_RESET);
	_rm68042Delay(10000);					   
	HAL_GPIO_WritePin(LCD_RESET.GPIOx, LCD_RESET.GPIO_Pin, GPIO_PIN_SET); 	 
	_rm68042Delay(10000);	
}

/*************************************************************
*************************************************************/
static void _rm68042WriteReg(uint16_t reg)	{
	*(__IO uint16_t *) (Bank1_LCD_REG) = reg;
	//lcdCommand[0] = reg;
}

/*************************************************************
*************************************************************/
static void _rm68042WriteRam(uint16_t ram)	{
	*(__IO uint16_t *) (Bank1_LCD_RAM) = ram;
}

/*************************************************************
*************************************************************/
static uint16_t _rm68042ReadRam(void)	{
	return *(__IO uint16_t *) (Bank1_LCD_RAM);
}

/*************************************************************
*************************************************************/
int32_t _rm68042SetWindow(uint16_t x0, uint16_t x1, uint16_t y0, uint16_t y1){
	
	uint16_t i;
	int32_t rtn = 0;
	
	while(1){
		if(x0 > X_SIZE)	{	rtn = -1;	break;	}
		if(x1 > X_SIZE)	{	rtn = -2;	break;	}
		if(y0 > Y_SIZE)	{	rtn = -3;	break;	}
		if(y1 > Y_SIZE)	{	rtn = -4;	break;	}
		
		if(x0 > x1){	i = x0;	x0 = x1;	x1 = i;	}
		if(y0 > y1){	i = y0;	y0 = y1;	y1 = i;	}
			
		_rm68042WriteReg(0x2a);  		//Set_column_address 
		_rm68042WriteRam(x0>>8);
		_rm68042WriteRam(x0&0xff);
		_rm68042WriteRam(x1>>8);
		_rm68042WriteRam(x1&0xff);

		_rm68042WriteReg(0x2b); 		//Set_page_address  
		_rm68042WriteRam(y0>>8);
		_rm68042WriteRam(y0&0xff);
		_rm68042WriteRam(y1>>8);
		_rm68042WriteRam(y1&0xff);

		break;
	}
	return rtn;
}

/*************************************************************
Public FunctionsS

*************************************************************/
/*************************************************************
*************************************************************/
void rm68042Initial(u16 background){
	uint32_t i;
	uint16_t x,y;
	
	//fade to full brightness
	_rm68042DimClk = 0;		//dim clock, increase in sysStick, 0..15 avilid
	_rm68042Dim = 0;			//brightness
	_rm68042DimTo = 15;
	_rm68042FadeClk = 0;

	_rm68042Reset();

	_rm68042WriteReg(0x0011);	//Exit sleep mode
	_rm68042Delay(20);
	
	_rm68042WriteReg(0x00D0);	//Power_Setting
	_rm68042WriteRam(0x0007); 
	_rm68042WriteRam(0x0041); 
	_rm68042WriteRam(0x001C); 

	_rm68042WriteReg(0x00D1); 	//VCOM Control
	_rm68042WriteRam(0x0000); 
	_rm68042WriteRam(0x0036); 	
	_rm68042WriteRam(0x001B); 	
	
	_rm68042WriteReg(0x00D2);	//Power_Setting for Normal Mode
	_rm68042WriteRam(0x0001); 
	_rm68042WriteRam(0x0011); 

	_rm68042WriteReg(0x00C0);  //Panel Driving Setting
	_rm68042WriteRam(0x0010);  
	_rm68042WriteRam(0x003B);  
	_rm68042WriteRam(0x0000);   
	_rm68042WriteRam(0x0012);  
	_rm68042WriteRam(0x0001);  

	_rm68042WriteReg(0x00C5);	//Frame Rate and Inversion Control   
	_rm68042WriteRam(0x0003);

	_rm68042WriteReg(0x00C8);	//Gamma Setting
	_rm68042WriteRam(0x0000);
	_rm68042WriteRam(0x0057);
	_rm68042WriteRam(0x0033);
	_rm68042WriteRam(0x0000);
	_rm68042WriteRam(0x0000);
	_rm68042WriteRam(0x0000);
	_rm68042WriteRam(0x0044);
	_rm68042WriteRam(0x0002);
	_rm68042WriteRam(0x0077);
	_rm68042WriteRam(0x0000);
	_rm68042WriteRam(0x0000);
	_rm68042WriteRam(0x0000);

	_rm68042WriteReg(0x003a); 	//Set_pixel_format: 16bit/pixel
	_rm68042WriteRam(0x0055); 	  
	_rm68042Delay(20);  

	_rm68042WriteReg(0x0029); 	// Set_display_on
	_rm68042Delay(10);	

	x = 480;						
	y = 320;
	_rm68042WriteReg(0x0036); 	//Set_address_mode	
	_rm68042WriteRam(0x00a2); 	//

	_rm68042WriteReg(0x2a);  		//Set_column_address 
	_rm68042WriteRam(0x00);
	_rm68042WriteRam(0x00);
	_rm68042WriteRam(x>>8);
	_rm68042WriteRam(x&0xff);

	_rm68042WriteReg(0x2b); 		//Set_page_address  
	_rm68042WriteRam(0x00);
	_rm68042WriteRam(0x00);
	_rm68042WriteRam(y>>8);
	_rm68042WriteRam(y&0xff);
	
	_rm68042WriteReg(0x2c);
	for (i=0; i<480*320; i++)		_rm68042WriteRam(background);
}

/*************************************************************
 Deinitial
*************************************************************/
void rm68042DeInitial(void){
}

/*************************************************************
 Read driver ID
*************************************************************/
uint16_t rm68042ReadID(void){
	uint16_t rtn;
	return rtn;
}
void rm68042InitSPI(void){
}

/*************************************************************
 display on
*************************************************************/
void rm68042DispOn(void){
	_rm68042WriteReg(0x0029);
	_rm68042Delay(1);	
}

/*************************************************************
 display off
*************************************************************/
void rm68042DispOff(void){
	_rm68042WriteReg(0x0028);
	_rm68042Delay(1);	
}

/*************************************************************
 Print a dot with specify color
*************************************************************/
int32_t rm68042SetPixel(uint16_t x, uint16_t y, uint16_t color){
	int32_t rtn=0;

	while(1){
		if(x > X_SIZE)	{rtn = -1;	break;}
		if(y > Y_SIZE)	{rtn = -2;	break;}
		
		_rm68042WriteReg(0x2a);  		//Set_column_address 
		_rm68042WriteRam(x>>8);
		_rm68042WriteRam(x&0xff);
		_rm68042WriteRam(x>>8);
		_rm68042WriteRam(x&0xff);

		_rm68042WriteReg(0x2b); 		//Set_page_address  
		_rm68042WriteRam(y>>8);
		_rm68042WriteRam(y&0xff);
		_rm68042WriteRam(y>>8);
		_rm68042WriteRam(y&0xff);
		
		_rm68042WriteReg(0x2c);			
		_rm68042WriteRam(color);	
		break;
	}
	return rtn;
}

/*************************************************************
 get a dot's color
*************************************************************/
int32_t rm68042GetPixel(uint16_t x, uint16_t y){
	int32_t rtn = 0;
	
	while(1){
		if(x > X_SIZE)	{rtn = -1;	break;}
		if(y > Y_SIZE)	{rtn = -2;	break;}
		
		_rm68042WriteReg(0x2a);  		//Set_column_address 
		_rm68042WriteRam(x>>8);
		_rm68042WriteRam(x&0xff);
		_rm68042WriteRam(x>>8);
		_rm68042WriteRam(x&0xff);

		_rm68042WriteReg(0x2b); 		//Set_page_address  
		_rm68042WriteRam(y>>8);
		_rm68042WriteRam(y&0xff);
		_rm68042WriteRam(y>>8);
		_rm68042WriteRam(y&0xff);
		
		_rm68042WriteReg(0x2e);	
		rtn = _rm68042ReadRam();	
		break;
	}
	return rtn;
}

/*************************************************************
 fill rectangle with color
*************************************************************/
int32_t rm68042FillRectColor(uint16_t x0,uint16_t x1,uint16_t y0,uint16_t y1,uint16_t color){
	uint16_t i,j;
	int32_t rtn = 0;
	
	while(1){
		if(_rm68042SetWindow(x0,x1,y0,y1)<0)	{	rtn = -1;	break;}
		_rm68042WriteReg(0x2c);			
		for (i=x0; i<=x1; i++)
			for(j=y0;j<=y1;j++){
				_rm68042WriteRam(color);
			}	
		break;
	}
	return rtn;
}

/*************************************************************
 fill rectangle with image
*************************************************************/
int32_t rm68042FillRectImage(uint16_t x0,uint16_t x1,uint16_t y0,uint16_t y1,const uint16_t* pImg,uint8_t flip){
	uint16_t i,j;
	int32_t rtn=0;
	
	uint16_t xx0,xx1,yy0,yy1;
	
	if(x0 > x1)	{	xx0=x1;	xx1=x0;	}
	if(x0 < x1)	{	xx0=x0;	xx1=x1;	}
	if(y0 > y1)	{	yy0=y1;	yy1=y0;	}
	if(y0 < y1)	{	yy0=y0;	yy1=y1;	}
	
/*	
	while(1){
		if(_rm68042SetWindow(x0,x1,y0,y1)<0)	{rtn = -1;	break;}
		_rm68042WriteReg(0x2c);	
		for (i=x0,k=0; i<=x1; i++)
			for(j=y0;j<=y1;j++){
				_rm68042WriteRam(pImg[k]);
				k++;
			}		
		break;
	}
*/
	while(1){
		if(_rm68042SetWindow(xx0,xx1,yy0,yy1)<0)	{rtn = -1;	break;}
		_rm68042WriteReg(0x2c);	
		for (i=0; i<=(yy1-yy0); i++)
			for(j=0;j<=(xx1-xx0);j++){
				switch (flip){
					case 1:	_rm68042WriteRam(pImg[i*(xx1-xx0+1)+(xx1-xx0)-j]);	break;
					case 2:	_rm68042WriteRam(pImg[((yy1-yy0)-i)*(xx1-xx0+1)+j]);	break;
					case 3:	_rm68042WriteRam(pImg[((yy1-yy0)-i)*(xx1-xx0+1)+(xx1-xx0)-j]);	break;
					default:_rm68042WriteRam(pImg[i*(xx1-xx0+1)+j]);	break;			
				}
			}
		break;
	}
	return rtn;
}

/*************************************************************

*************************************************************/
int32_t rm68042DrawRect(uint8_t dotSize, uint16_t x0, uint16_t x1, uint16_t y0, uint16_t y1, uint16_t color){
	int32_t rtn=0;
	uint8_t dot;

	while(1){
		if(dotSize <= 0){	rtn = -1;	break;}
		dot = dotSize-1;
		//draw x0,x1
		if(y0 >= y1){
			if(rm68042FillRectColor(x0,x1,(y0-dot),y0,color)<0)	{rtn = -2;	break;}
			if(rm68042FillRectColor(x0,x1,y1,(y1+dot),color)<0)	{rtn = -3;	break;}
		}
		else{
			if(rm68042FillRectColor(x0,x1,y0,y0+dot,color)<0)	{rtn = -4;	break;}
			if(rm68042FillRectColor(x0,x1,y1-dot,y1,color)<0)	{rtn = -5;	break;}
		}
		//draw y0,y1
		if(x0 >= x1){
			if(rm68042FillRectColor((x0-dot),x0,y0,y1,color)<0)	{rtn = -6;	break;}
			if(rm68042FillRectColor(x1,(x1+dot),y0,y1,color)<0)	{rtn = -7;	break;}
		}
		else{
			if(rm68042FillRectColor(x0,(x0+dot),y0,y1,color)<0)	{rtn = -8;	break;}
			if(rm68042FillRectColor((x1-dot),x1,y0,y1,color)<0)	{rtn = -9;	break;}
		}
		break;
	}
	return rtn;
}

/*************************************************************
 dim the backlight's brightness, dimClk=0..15
*************************************************************/
void rm68042SetBackLight(uint8_t dim){
	_rm68042DimTo = dim;
}

/*************************************************************

*************************************************************/
void rm68042Tick(void){
	_rm68042DimClk++;
	_rm68042DimClk &= 0x0f;	
	if(_rm68042Dim > _rm68042DimClk )
		HAL_GPIO_WritePin(LCD_BACKLIGHT.GPIOx, LCD_BACKLIGHT.GPIO_Pin, GPIO_PIN_SET);		//turn on backlight
	else
		HAL_GPIO_WritePin(LCD_BACKLIGHT.GPIOx, LCD_BACKLIGHT.GPIO_Pin, GPIO_PIN_RESET);	//turn off backlight			

	_rm68042FadeClk++;
	if(_rm68042FadeClk >= 20){
		_rm68042FadeClk = 0;
		if(_rm68042Dim > _rm68042DimTo)	_rm68042Dim--;
		if(_rm68042Dim < _rm68042DimTo)	_rm68042Dim++;	
	}
} 

