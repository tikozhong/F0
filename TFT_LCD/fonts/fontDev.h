/**********************************************
filename:	fontDev.h
**********************************************/
#ifndef _FONT_DEV_H
#define _FONT_DEV_H

#include "rm68042.h"
/*****************************************************************************
 @ public defines
****************************************************************************/

/*****************************************************************************
 @ typedefs
****************************************************************************/
typedef unsigned          char uint8_t;
typedef unsigned short    int uint16_t;
typedef unsigned          int uint32_t;
typedef 				          int int32_t;

typedef struct {
  unsigned char XSize;
  unsigned char XDist;
  unsigned char BytesPerLine;
  const unsigned char* pData;
} GUI_CHARINFO;

typedef struct {
	const GUI_CHARINFO* charInfo;
	const unsigned char charHeight;
} FONT_T;

typedef struct {
	rm68140Dev_T *lcd;
	const FONT_T* font;
	uint16_t color;
}fontRsrc_T;

typedef struct {
	fontRsrc_T rsrc;
	u16 (*print)(fontRsrc_T* pRsrc, uint16_t x, uint16_t y, const char* string);
	u16 (*printX)(fontRsrc_T* pRsrc, uint16_t x, uint16_t y, uint16_t color, const FONT_T* font, const char* string);
}fontDev_T;


extern const FONT_T FONT_GEORGIA14;
extern const FONT_T FONT_GEORGIA16;
extern const FONT_T FONT_GEORGIA18;
extern const FONT_T FONT_GEORGIA20;
extern const FONT_T FONT_GEORGIA22;
extern const FONT_T FONT_GEORGIA24;
extern const FONT_T FONT_GEORGIA26;
extern const FONT_T FONT_GEORGIA28;
extern const FONT_T FONT_CAMBRIA14;
extern const FONT_T FONT_CAMBRIA18;
extern const FONT_T FONT_GEORGIA14;
extern const FONT_T FONT_GEORGIA16;
extern const FONT_T FONT_GEORGIA18;
extern const FONT_T FONT_GEORGIA20;
extern const FONT_T FONT_GEORGIA22;
extern const FONT_T FONT_GEORGIA24;
extern const FONT_T FONT_GEORGIA26;
extern const FONT_T FONT_GEORGIA28;
extern const FONT_T FONT_CONSOLAS38;



s8 fontDevSetup(fontDev_T* pDev, rm68140Dev_T *pLCD, u16 color, const FONT_T* font);

#endif
