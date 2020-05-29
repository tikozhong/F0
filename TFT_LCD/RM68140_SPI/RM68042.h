/*************************************************************
 fileName: RM68042.h
*************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _RM68042_H
#define _RM68042_H

#include "misc.h"
#include "myBmp.h"

/*************************************************************
*	[usr]config DBI, tunr on only one DBI
*************************************************************/
#define	DBI_USING_SPI			1
#define DBI_USING_M16BIT	0

#define White          0xFFFF
#define Black          0x0000
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0

#define WHITE						0xFFFF
#define BLACK						0x0000	  
#define BLUE						0x001F  
#define BRED						0XF81F
#define GRED						0XFFE0
#define GBLUE						0X07FF
#define RED							0xF800
#define MAGENTA						0xF81F
#define GREEN						0x07E0
#define CYAN						0x7FFF
#define YELLOW						0xFFE0
#define BROWN						0XBC40 //×ØÉ«
#define BRRED						0XFC07 //×ØºìÉ«
#define GRAY						0X8430 //»ÒÉ«


/****************************************************************************
 @ typedefs
****************************************************************************/
typedef struct {
	PIN_T REG_SEL;
	PIN_T SPI_CS;
	SPI_HandleTypeDef *SPI_HANDLE;
	u16 xLen,yLen;
}rm68140Rsrc_T;

typedef struct {
	rm68140Rsrc_T rsrc;
	//ops
	void (*Reset) 			(rm68140Rsrc_T*, u16 xSize, u16 ySize);
	u16 (*ReadID) 			(rm68140Rsrc_T*);
	void (*DisplayOn)	 	(rm68140Rsrc_T*);
	void (*DisplayOff) 	(rm68140Rsrc_T*);
	void (*SetCursor) 	(rm68140Rsrc_T*, u16 Xpos, u16 Ypos);
	s8 (*WritePixel)		(rm68140Rsrc_T*, u16 Xpos, u16 Ypos, u16 color);
	s8 (*DrawHLine) (rm68140Rsrc_T*, u16 Xpos, u16 Ypos, u16 Length, u16 color);
	s8 (*DrawVLine) (rm68140Rsrc_T*, u16 Xpos, u16 Ypos, u16 Length, u16 color);
	s8 (*DrawImage) (rm68140Rsrc_T *pRsrc, u16 x, u16 y, const myImg_t* img);
	s8 (*DrawBlock) (rm68140Rsrc_T *pRsrc, u16 x0, u16 x1, u16 y0, u16 y1, u16 color);
	
	s8 (*FullScreen)	(rm68140Rsrc_T *pRsrc, u16 color);
	s8 (*DrawPartern)	(rm68140Rsrc_T *pRsrc, u16 x, u16 y);
	//	u16 (*GetLcdPixelWidth) (rm68140Rsrc_T*);
	//	u16 (*GetLcdPixelHeight) (rm68140Rsrc_T*);
	//u16 (*ReadPixel) 		(rm68140Rsrc_T*, u16 Xpos, u16 Ypos);
	//void (*SetDisplayWindow) (rm68140Rsrc_T*, u16 Xpos, u16 Ypos, u16 Width, u16 Height);
}rm68140Dev_T;

/*************************************************************
 public functions
*************************************************************/
s8 Rm68140Setup(
	rm68140Dev_T *pDev,
	PIN_T RS, 		//Resister select pin
	PIN_T CS, 		//SPI select pin
	SPI_HandleTypeDef *SPI_HANDLE,
	u16 xLen, u16 yLen
);

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif /* _RM68042_H */

/*****************************END OF FILE**************************/

