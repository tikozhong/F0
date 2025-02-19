/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                           www.segger.com                           *
**********************************************************************
*                                                                    *
* C-file generated by                                                *
*                                                                    *
*        Bitmap Converter for emWin V5.22.                           *
*        Compiled Jul  4 2013, 12:18:24                              *
*        (c) 1998 - 2013 Segger Microcontroller GmbH && Co. KG       *
*                                                                    *
**********************************************************************
*                                                                    *
* Source file: PARTERN                                               *
* Dimensions:  50 * 85                                               *
* NumColors:   16bpp: 65536                                          *
*                                                                    *
**********************************************************************
*/

#include "myBmp.h"

#define XSIZE 50
#define YSIZE 83

#define IMG_NAME F_PARTERN

static const uint16_t IMG_RSRC[] = {
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 
        0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 
        0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 
        0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 
        0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 
        0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 
        0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 
        0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 
        0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 
        0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFB36, 0xF800, 0xF800, 0xF800, 0xF800, 0xF800, 0xF800, 0xFFF6, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0x033F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x601F, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x97FF, 0x07E7, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 
        0xDFE7, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFB36, 0xF800, 0xF800, 0xF800, 0xF800, 0xF800, 0xF800, 0xF800, 0xFFF6, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0x033F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x601F, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x07F6, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 
        0xB7E0, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFB36, 0xF800, 0xF9C0, 0xFFFB, 0xFFFF, 0xFC9B, 0xF800, 0xF800, 0xFED2, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0x033F, 0x001F, 0xB33F, 0xFFFF, 0xFFFF, 0x3C9F, 0x001F, 0x001F, 0xFDBF, 0xFFFF, 0xFFFF, 0x3FFB, 0x07E0, 0x07E0, 0xB7E0, 0xFFFF, 0xFFFF, 0xFFFF, 0x97FF, 
        0xB7E7, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFB36, 0xF800, 0xF9C0, 0xFFFB, 0xFFFF, 0xFC9B, 0xF800, 0xF9C0, 0xFFFB, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0x033F, 0x001F, 0xB33F, 0xFFFF, 0xFFFF, 0x96DF, 0x001F, 0x001F, 0xFDBF, 0xFFFF, 0xB7FF, 0x07EC, 0x07E0, 0xB7E0, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFB36, 0xF800, 0xF800, 0xF800, 0xF800, 0xF800, 0xFB20, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0x033F, 0x001F, 0xB33F, 0xFFFF, 0xFFFF, 0x033F, 0x001F, 0x601F, 0xFFFF, 0xFFFF, 0x67FF, 0x07E0, 0x3FE0, 0xFFF2, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFB36, 0xF800, 0xF800, 0xF800, 0xF800, 0xF800, 0xF800, 0xF9C0, 0xFFFB, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0x033F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0x381F, 0xFEDF, 0xFFFF, 0xFFFF, 0x3FFB, 0x07E0, 0x3FE0, 0xFFF2, 0xFFFF, 0x67FF, 0x07E0, 0x07E0, 0x07E0, 
        0x3FE0, 0xFFF2, 0xFFFF, 0xFFFF, 0xFFFF, 0xFB36, 0xF800, 0xF9C0, 0xFFFB, 0xFFFF, 0xFEDF, 0xF807, 0xF800, 0xFC87, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0x033F, 0x001F, 0x001F, 0x001F, 0x001F, 0x001F, 0xFDBF, 0xFFFF, 0xFFFF, 0xFFFF, 0x67FF, 0x07E0, 0x3FE0, 0xFFF2, 0xFFFF, 0x67FF, 0x07E0, 0x07E0, 0x07E0, 
        0x3FE0, 0xFFF2, 0xFFFF, 0xFFFF, 0xFFFF, 0xFB36, 0xF800, 0xF9C0, 0xFFFB, 0xFFFF, 0xFFFF, 0xF9D2, 0xF800, 0xFB20, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0x033F, 0x001F, 0x91DF, 0xFFFF, 0x3C9F, 0x001F, 0x001F, 0xFDBF, 0xFFFF, 0xFFFF, 0x97FF, 0x07E7, 0x07E0, 0xDFE7, 0xFFFF, 0xFFFF, 0xFFFF, 0x97FF, 0x07E7, 
        0x3FE0, 0xFFF2, 0xFFFF, 0xFFFF, 0xFFFF, 0xFB36, 0xF800, 0xF9C0, 0xFFFB, 0xFFFF, 0xFDBF, 0xF800, 0xF800, 0xFC87, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0x033F, 0x001F, 0x91DF, 0xFFFF, 0xDFFF, 0x01DF, 0x001F, 0x601F, 0xFFFF, 0xFFFF, 0xDFFF, 0x07F2, 0x07E0, 0x07E0, 0xDFE7, 0xFFFF, 0xFFFF, 0x97FF, 0x07E7, 
        0x3FE0, 0xFFF2, 0xFFFF, 0xFFFF, 0xFFFF, 0xFB36, 0xF800, 0xF800, 0xF800, 0xF800, 0xF800, 0xF800, 0xF800, 0xFED2, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0x033F, 0x001F, 0x91DF, 0xFFFF, 0xFFFF, 0x96DF, 0x001F, 0x001F, 0xB33F, 0xFFFF, 0xFFFF, 0xB7FF, 0x07EC, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 
        0x3FE0, 0xFFF2, 0xFFFF, 0xFFFF, 0xFFFF, 0xFB36, 0xF800, 0xF800, 0xF800, 0xF800, 0xF800, 0xF800, 0xFFF6, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0x033F, 0x001F, 0x91DF, 0xFFFF, 0xFFFF, 0xFFFF, 0x3C9F, 0x001F, 0x001F, 0xDC9F, 0xFFFF, 0xFFFF, 0xFFFF, 0x3FFB, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 0x07E0, 
        0xFFEC, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 
        0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 
        0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 
        0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 
        0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 
        0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 
        0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 
        0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 
        0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xF800, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xF800, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xF800, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xF800, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xF800, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xF800, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xF800, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xF800, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xF800, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xF800, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xF800, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xF800, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xF800, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xF800, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xF800, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xF800, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xF800, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xF800, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xF800, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xF800, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xF800, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xF800, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xF800, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xF800, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xF800, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xF800, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xF800, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xF800, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xF800, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xF800, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xF800, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xF800, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xF800, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xF800, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xFFFF, 0xF800, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xFFFF, 0xF800, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xED00, 0xFFFF, 0xF800, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 
        0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF
};

myImg_t IMG_NAME =	{XSIZE,YSIZE,IMG_RSRC};

//GUI_CONST_STORAGE GUI_BITMAP bmPARTERN = {
//  50, // xSize
//  83, // ySize
//  100, // BytesPerLine
//  16, // BitsPerPixel
//  (unsigned char *)_acPARTERN,  // Pointer to picture data
//  NULL,  // Pointer to palette
//  GUI_DRAW_BMP565
//};

/*************************** End of file ****************************/
