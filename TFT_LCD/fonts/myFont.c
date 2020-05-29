/**********************************************
filename:	myfont.c
**********************************************/
#include "myfont.h"
extern int32_t LcdSetPixel(uint16_t x, uint16_t y, uint16_t color);

static void printChar(uint16_t x, uint16_t y, uint16_t color, FONT_T fontType, uint8_t symbol);
/********************
* 
********************/
static void printChar(uint16_t x, uint16_t y, uint16_t color, FONT_T fontType, uint8_t symbol){
	uint8_t tmp,i,j,k,bytesPerLine,charHeigh;
	
	bytesPerLine = fontType.charInfo[symbol-CHAR_BASE].BytesPerLine;
	charHeigh = fontType.charHeight;
	
	for(i=0; i<charHeigh; i++){
		for(j=0; j<bytesPerLine; j++){
			tmp = fontType.charInfo[symbol-CHAR_BASE].pData[i*bytesPerLine+j];
			for(k=0;k<8;k++){
				if(tmp&0x80)	LcdSetPixel(x+j*8+k, y+i, color);
				tmp <<= 1;
			}
		}
	}	
}

uint16_t printString(uint16_t x, uint16_t y, uint16_t color, FONT_T fontType, uint8_t* string){
	uint8_t i;
	uint16_t xPos;
	
	for(i=0,xPos=x;1;i++){
		if(string[i]==0)	break;
		printChar(xPos,y,color,fontType,string[i]);
		xPos += fontType.charInfo[string[i]-CHAR_BASE].XSize;
	}
	return xPos;
}

uint16_t printNum(uint16_t x, uint16_t y, uint16_t color, FONT_T fontType, uint32_t num){
	uint8_t s,zeroEn=0;
	uint16_t xPos;
	uint32_t tmp0,tmp1,base;

	tmp0 = num;
	base = 1000000000;
	xPos = x;
	while(1){
		tmp1 = tmp0/base;
		tmp0 %= base;
		if(tmp1){
			s = (uint8_t)tmp1+48;
			printChar(xPos,y,color,fontType,s);
			xPos += fontType.charInfo[s-CHAR_BASE].XSize;
			zeroEn = 1;
		}
		else{
			if(zeroEn){
				s = '0';
				printChar(xPos,y,color,fontType,s);
				xPos += fontType.charInfo[s-CHAR_BASE].XSize;
			}
		}
		base /= 10;
		if(base == 1){
			s = (uint8_t)tmp0+48;
			printChar(xPos,y,color,fontType,s);
			break;
		}
	}
	return xPos;
}


