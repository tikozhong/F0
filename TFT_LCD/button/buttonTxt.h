/**********************************************
filename:	buttonTxt.h
**********************************************/
#ifndef _BUTTON_TXT_H
#define _BUTTON_TXT_H

#include "guiType.h"
#include "fontDev.h"
#include "rm68042.h"

/*****************************************************************************
 @ public defines
****************************************************************************/
/*****************************************************************************
 @ typedefs
****************************************************************************/
typedef struct {
	//driver layer
	rm68140Dev_T *lcd;
	fontDev_T *fontDev;
	//feature
	u16 locX,locY;
	u16 width,heigh;
	u16 bColor;	//back groud color
	u16 activeColor;
	const FONT_T* font;
	u16 textColor;
	char text[16];
}buttonRsrc_t;

typedef struct {
	buttonRsrc_t rsrc;
	void (*Redraw)(buttonRsrc_t* pRsrc);
	void (*ResponseEvent)(buttonRsrc_t* pRsrc, GUI_EVENT_T evnt, u16 x, u16 y);
}buttonDev_t;

void buttonSetup(buttonDev_t* pDev, rm68140Dev_T *lcd, fontDev_T *fontDev, u16 x, u16 y, u16 width, u16 heigh, u16 bColor, u16 actColor, u16 txtColor, const FONT_T* font, const char* TEXT);

#endif
