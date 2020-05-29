/*************************************************************
 fileName: RM68042.h
*************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _RM68042_H
#define _RM68042_H

#ifdef _STM32F0
	#include "stm32f0xx_hal.h"
#endif	

#ifdef _STM32F1
	#include "stm32f1xx_hal.h"
#endif	
#include <stdio.h>

/*************************************************************
*	[usr]config DBI, tunr on only one DBI
*************************************************************/
#define	DBI_USING_SPI			0
#define DBI_USING_M16BIT	1

/*************************************************************
 * [usr]register base
 * ѡ��BANK1-BORSRAM1 ���� TFT����ַ��ΧΪ0X60000000~0X63FFFFFF
 * FSMC_A16 ��LCD��DC(�Ĵ���/����ѡ��)��
 * 16 bit => FSMC[24:0]��ӦHADDR[25:1]
 * �Ĵ�������ַ = 0X60000000
 * RAM����ַ = 0X60020000 = 0X60000000+2^16*2 = 0X60000000 + 0X20000 = 0X60020000
*************************************************************/
#define Bank1_LCD_REG    ((u32)0x60000000)	   	//Disp Reg ADDR
#define Bank1_LCD_RAM    ((u32)0x60020000)    	//Disp Data ADDR

#define X_SIZE	480
#define Y_SIZE	320

/*****************************************************************************
 @ typedefs
****************************************************************************/
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

typedef struct {
	GPIO_TypeDef* GPIOx;	// Port Register Base addr.
	u16 GPIO_Pin;		// Pin
} STM32_PIN_RM68042;

/*************************************************************
 * public var
 *
*************************************************************/


/*************************************************************
 public functions
*************************************************************/
void rm68042Initial(u16 background);
void rm68042DeInitial(void);
u16 rm68042ReadID(void);
void rm68042InitSPI(void);
void rm68042DispOn(void);
void rm68042DispOff(void);
int32_t rm68042SetPixel(u16 x, u16 y, u16 color);
int32_t rm68042GetPixel(u16 x, u16 y);
int32_t rm68042FillRectColor(u16 x0,u16 x1,u16 y0,u16 y1,u16 color);
int32_t rm68042FillRectImage(u16 x0,u16 x1,u16 y0,u16 y1,const uint16_t* pImg,uint8_t flip);
int32_t rm68042DrawRect(u8 dotSize, u16 x0, u16 x1, u16 y0, u16 y1, u16 color);
void rm68042SetBackLight(u8 dim);
void rm68042Tick(void);

//δ��֤��ʵ��
//��Ļ��ʼʱ��ʾ��ʽ��ע�⣺��IDelayʱ��ʾ��һ��������������ˢ�µ�
//��ʱ�����ֶ���ˢ�½��������  LCD_WR_REG(0x0007,0x0173);������ʾ
//��Immediatelyʱû�б�ע�͵��ǲ���Ҫ�˹���

#endif /* _RM68042_H */

/*****************************END OF FILE**************************/

