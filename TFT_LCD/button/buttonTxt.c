/**********************************************
filename:	buttonTxt.c
**********************************************/
#include "buttonTxt.h"
#include "string.h"
//#include "myuart_os.h"

/* using external variables ------------------------------------*/
/* global variables --------------------------------------------*/
/* private variables -------------------------------------------*/
static void buttonRedraw(buttonRsrc_t* pRsrc);
static void buttonResponseEvent(buttonRsrc_t* pRsrc, GUI_EVENT_T evnt, u16 x, u16 y);

/* private Functions -------------------------------------------*/
void buttonSetup(buttonDev_t* pDev, rm68140Dev_T *lcd, fontDev_T *fontDev, u16 x, u16 y, u16 width, u16 heigh, u16 bColor, u16 actColor, u16 txtColor, const FONT_T* font, const char* TEXT){
	pDev->rsrc.lcd = lcd;
	pDev->rsrc.fontDev = fontDev;
	pDev->rsrc.locX = x;
	pDev->rsrc.locY = y;
	pDev->rsrc.width = width;
	pDev->rsrc.heigh = heigh;
	pDev->rsrc.bColor = bColor;
	pDev->rsrc.activeColor = actColor;
	pDev->rsrc.textColor = txtColor;
	pDev->rsrc.font = font;
	memset(pDev->rsrc.text,0,16);
	strcpy(pDev->rsrc.text, TEXT);
	
	pDev->Redraw = buttonRedraw;
	pDev->ResponseEvent = buttonResponseEvent;
}

static void buttonRedraw(buttonRsrc_t* pRsrc){
	rm68140Dev_T* lcd = pRsrc->lcd;
	u16 x0,x1,y0,y1;

	x0 = pRsrc->locX;
	x1 = x0 + pRsrc->width;
	y0 = pRsrc->locY;
	y1 = y0 + 2;
	lcd->DrawBlock(&lcd->rsrc, x0, x1, y0, y1, pRsrc->activeColor);

	x0 = pRsrc->locX;
	x1 = x0 + pRsrc->width;
	y1 = pRsrc->locY + pRsrc->heigh;	
	y0 = y1 - 2;
	lcd->DrawBlock(&lcd->rsrc, x0, x1, y0, y1, pRsrc->activeColor);
	
	x0 = pRsrc->locX;
	x1 = x0 + 2;
	y0 = pRsrc->locY;
	y1 = y0 + pRsrc->heigh;	
	lcd->DrawBlock(&lcd->rsrc, x0, x1, y0, y1, pRsrc->activeColor);
		
	x1 = pRsrc->locX+pRsrc->width;
	x0 = x1 - 2;
	y0 = pRsrc->locY;
	y1 = y0 + pRsrc->heigh;	
	lcd->DrawBlock(&lcd->rsrc, x0, x1, y0, y1, pRsrc->activeColor);
	
	pRsrc->fontDev->printX(&pRsrc->fontDev->rsrc, pRsrc->locX+22, pRsrc->locY+13,pRsrc->textColor, pRsrc->font, pRsrc->text);	
}

static void buttonResponseEvent(buttonRsrc_t* pRsrc, GUI_EVENT_T evnt, u16 x, u16 y){
}