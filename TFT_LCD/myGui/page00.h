/**********************************************
filename:	page00.h
**********************************************/
#ifndef _PAGE_00_H
#define _PAGE_00_H

#include "guiType.h"
#include "buttontxt.h"
#include "fontDev.h"
#include "rm68042.h"
/*****************************************************************************
 @ public defines
****************************************************************************/


/*****************************************************************************
 @ typedefs
****************************************************************************/
typedef struct {
	rm68140Dev_T *lcd;
	fontDev_T *font;
	buttonDev_t keys[11];	
	u16 bColor;
	u16 actColor;
}page00Rsrc_t;

typedef struct {
	page00Rsrc_t rsrc;
	void (*Redraw)(page00Rsrc_t* pRsrc);
	void (*ResponseEvent)(page00Rsrc_t* pRsrc, GUI_EVENT_T evnt, u16 x, u16 y);
}page00Dev_t;

void page00Setup(page00Dev_t* pDev,	rm68140Dev_T *lcd, fontDev_T *fontDev, u16 bColor, u16 actColor, u16 txtColor, const FONT_T* font);
#endif
