/**********************************************
filename:	page01.h
**********************************************/
#ifndef _PAGE_01_H
#define _PAGE_01_H

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
	buttonDev_t btnMod;	
	u16 bColor;
	u16 actColor;
}page01Rsrc_t;

typedef struct {
	page01Rsrc_t rsrc;
	void (*Redraw)(page01Rsrc_t* pRsrc);
	void (*ResponseEvent)(page01Rsrc_t* pRsrc, GUI_EVENT_T evnt, u16 x, u16 y);
}page01Dev_t;

void page01Setup(page01Dev_t* pDev,	rm68140Dev_T *lcd, fontDev_T *fontDev, u16 bColor, u16 actColor, u16 txtColor, const FONT_T* font);
#endif
