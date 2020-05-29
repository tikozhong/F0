/**********************************************
filename:	myGui.c
**********************************************/

#include "string.h"
#include "myuart_os.h"
#include "page00.h"

/* using external variables ------------------------------------*/
/* global variables --------------------------------------------*/
/* private variables -------------------------------------------*/

static void page00Redraw(page00Rsrc_t* pRsrc);
static void page00ResponseEvent(page00Rsrc_t* pRsrc, GUI_EVENT_T evnt, u16 x, u16 y);

/* private Functions -------------------------------------------*/
void page00Setup(page00Dev_t* pDev,	rm68140Dev_T *lcd, fontDev_T *fontDev, u16 bColor, u16 actColor, u16 txtColor, const FONT_T* font){
	//seutp all keys
	pDev->rsrc.bColor = bColor;
	pDev->rsrc.actColor = actColor;
	pDev->rsrc.lcd = lcd;
	pDev->rsrc.font = fontDev;
	buttonSetup(&pDev->rsrc.keys[0], lcd, fontDev, 280, 250, 58, 58, bColor, actColor, txtColor, font, "0");
	buttonSetup(&pDev->rsrc.keys[1], lcd, fontDev, 280, 55, 58, 58, bColor, actColor, txtColor, font, "1");
	buttonSetup(&pDev->rsrc.keys[2], lcd, fontDev, 345, 55, 58, 58, bColor, actColor, txtColor, font, "2");
	buttonSetup(&pDev->rsrc.keys[3], lcd, fontDev, 410, 55, 58, 58, bColor, actColor, txtColor, font, "3");
	buttonSetup(&pDev->rsrc.keys[4], lcd, fontDev, 280, 120, 58, 58, bColor, actColor, txtColor, font, "4");
	buttonSetup(&pDev->rsrc.keys[5], lcd, fontDev, 345, 120, 58, 58, bColor, actColor, txtColor, font, "5");
	buttonSetup(&pDev->rsrc.keys[6], lcd, fontDev, 410, 120, 58, 58, bColor, actColor, txtColor, font, "6");
	buttonSetup(&pDev->rsrc.keys[7], lcd, fontDev, 280, 185, 58, 58, bColor, actColor, txtColor, font, "7");
	buttonSetup(&pDev->rsrc.keys[8], lcd, fontDev, 345, 185, 58, 58, bColor, actColor, txtColor, font, "8");
	buttonSetup(&pDev->rsrc.keys[9], lcd, fontDev, 410, 185, 58, 58, bColor, actColor, txtColor, font, "9");
	buttonSetup(&pDev->rsrc.keys[10], lcd, fontDev, 345, 250, 123, 58, bColor, actColor, txtColor, font, "ENTER");

	pDev->Redraw = page00Redraw;
	pDev->ResponseEvent = page00ResponseEvent;
	page00Redraw(&pDev->rsrc);
}

static void page00Redraw(page00Rsrc_t* pRsrc){
	u8 i;
	u16 tmp;
	//draw all buttons
	pRsrc->lcd->FullScreen(&pRsrc->lcd->rsrc, pRsrc->bColor);
	for(i=0;i<11;i++)		pRsrc->keys[i].Redraw(&pRsrc->keys[i].rsrc);
	//draw line
	pRsrc->lcd->DrawHLine(&pRsrc->lcd->rsrc, 280, 45, 185, pRsrc->actColor);
	pRsrc->lcd->DrawHLine(&pRsrc->lcd->rsrc, 280, 46, 185, pRsrc->actColor);
	
	tmp = 45;
	pRsrc->lcd->DrawHLine(&pRsrc->lcd->rsrc, 30, tmp++, 200, pRsrc->actColor); 
	pRsrc->lcd->DrawHLine(&pRsrc->lcd->rsrc, 30, tmp, 200, pRsrc->actColor);		tmp += 52;
	
	pRsrc->lcd->DrawHLine(&pRsrc->lcd->rsrc, 30, tmp++, 200, pRsrc->actColor);
	pRsrc->lcd->DrawHLine(&pRsrc->lcd->rsrc, 30, tmp, 200, pRsrc->actColor);		tmp += 51;	
	
	pRsrc->lcd->DrawHLine(&pRsrc->lcd->rsrc, 30, tmp++, 200, pRsrc->actColor);
	pRsrc->lcd->DrawHLine(&pRsrc->lcd->rsrc, 30, tmp, 200, pRsrc->actColor);		tmp += 52;	
	
	pRsrc->lcd->DrawHLine(&pRsrc->lcd->rsrc, 30, tmp++, 200, pRsrc->actColor);
	pRsrc->lcd->DrawHLine(&pRsrc->lcd->rsrc, 30, tmp, 200, pRsrc->actColor);		tmp += 51;	

	pRsrc->lcd->DrawHLine(&pRsrc->lcd->rsrc, 30, tmp++, 200, pRsrc->actColor);
	pRsrc->lcd->DrawHLine(&pRsrc->lcd->rsrc, 30, tmp, 200, pRsrc->actColor);		tmp += 51;	
	
	pRsrc->lcd->DrawHLine(&pRsrc->lcd->rsrc, 30, tmp++, 200, pRsrc->actColor);
	pRsrc->lcd->DrawHLine(&pRsrc->lcd->rsrc, 30, tmp, 200, pRsrc->actColor);	
	
	pRsrc->font->printX(&pRsrc->font->rsrc, 30, 18, 0xFFFF, &FONT_GEORGIA28, "USER LIST:");
}

static void page00ResponseEvent(page00Rsrc_t* pRsrc, GUI_EVENT_T evnt, u16 x, u16 y){
	u8 i;
	for(i=0;i<11;i++)		
		pRsrc->keys[i].ResponseEvent(&pRsrc->keys[i].rsrc,evnt, x, y);
}
