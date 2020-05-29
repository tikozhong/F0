/*************************************************************
 fileName: myBmp.h
*************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _MYBMP_H
#define _MYBMP_H

#include "misc.h"

typedef struct {
	const uint8_t xsize;
	const uint8_t ysize;
	const uint16_t* pImg;
} myImg_t;

extern myImg_t F_PARTERN;
extern myImg_t IMG_LOGIN;
#endif
