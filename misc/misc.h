/**********************************************************
filename: misc.h
**********************************************************/
#ifndef _MISC_H_
#define _MISC_H_

#define USING_RTOS	0
#define USING_F0	1

#if USING_RTOS
#include "cmsis_os.h"
#endif

#if USING_F0
#include "stm32f0xx_hal.h"
#endif

#if USING_F1
#include "stm32f1xx_hal.h"
#endif

#define NOP __NOP
#define CALLBACK_MSG_LEN	48
#define DEV_NAME_LEN		16


#define	BIT(n)	((u32)1<<n)
//#define BIT00	((u32)1<<0)
//#define BIT01	((u32)1<<1)
//#define BIT02	((u32)1<<2)
//#define BIT03	((u32)1<<3)
//#define BIT04	((u32)1<<4)
//#define BIT05	((u32)1<<5)
//#define BIT06	((u32)1<<6)
//#define BIT07	((u32)1<<7)
//#define BIT08	((u32)1<<8)
//#define BIT09	((u32)1<<9)
//#define BIT10	((u32)1<<10)
//#define BIT11	((u32)1<<11)
//#define BIT12	((u32)1<<12)
//#define BIT13	((u32)1<<13)
//#define BIT14	((u32)1<<14)
//#define BIT15	((u32)1<<15)
//#define BIT16	((u32)1<<16)
//#define BIT17	((u32)1<<17)
//#define BIT18	((u32)1<<18)
//#define BIT19	((u32)1<<19)
//#define BIT20	((u32)1<<20)
//#define BIT21	((u32)1<<21)
//#define BIT22	((u32)1<<22)
//#define BIT23	((u32)1<<23)
//#define BIT24	((u32)1<<24)
//#define BIT25	((u32)1<<25)
//#define BIT26	((u32)1<<26)
//#define BIT27	((u32)1<<27)
//#define BIT28	((u32)1<<28)
//#define BIT29	((u32)1<<29)
//#define BIT30	((u32)1<<30)
//#define BIT31	((u32)1<<31)


#define	BIT_LEN(n)	(0XFFFFFFFF>>(32-n))
//#define	BIT_LEN01	(0XFFFFFFFF>>(32-1))
//#define	BIT_LEN02	(0XFFFFFFFF>>(32-2))
//#define	BIT_LEN03	(0XFFFFFFFF>>(32-3))
//#define	BIT_LEN04	(0XFFFFFFFF>>(32-4))
//#define	BIT_LEN05	(0XFFFFFFFF>>(32-5))
//#define	BIT_LEN06	(0XFFFFFFFF>>(32-6))
//#define	BIT_LEN07	(0XFFFFFFFF>>(32-7))
//#define	BIT_LEN08	(0XFFFFFFFF>>(32-8))
//#define	BIT_LEN09	(0XFFFFFFFF>>(32-9))
//#define	BIT_LEN10	(0XFFFFFFFF>>(32-10))
//#define	BIT_LEN11	(0XFFFFFFFF>>(32-11))
//#define	BIT_LEN12	(0XFFFFFFFF>>(32-12))
//#define	BIT_LEN13	(0XFFFFFFFF>>(32-13))
//#define	BIT_LEN14	(0XFFFFFFFF>>(32-14))
//#define	BIT_LEN15	(0XFFFFFFFF>>(32-15))
//#define	BIT_LEN16	(0XFFFFFFFF>>(32-16))
//#define	BIT_LEN17	(0XFFFFFFFF>>(32-17))
//#define	BIT_LEN18	(0XFFFFFFFF>>(32-18))
//#define	BIT_LEN19	(0XFFFFFFFF>>(32-19))
//#define	BIT_LEN20	(0XFFFFFFFF>>(32-20))
//#define	BIT_LEN21	(0XFFFFFFFF>>(32-21))
//#define	BIT_LEN22	(0XFFFFFFFF>>(32-22))
//#define	BIT_LEN23	(0XFFFFFFFF>>(32-23))
//#define	BIT_LEN24	(0XFFFFFFFF>>(32-24))
//#define	BIT_LEN25	(0XFFFFFFFF>>(32-25))
//#define	BIT_LEN26	(0XFFFFFFFF>>(32-26))
//#define	BIT_LEN27	(0XFFFFFFFF>>(32-27))
//#define	BIT_LEN28	(0XFFFFFFFF>>(32-28))
//#define	BIT_LEN29	(0XFFFFFFFF>>(32-29))
//#define	BIT_LEN30	(0XFFFFFFFF>>(32-30))
//#define	BIT_LEN31	(0XFFFFFFFF>>(32-31))
//#define	BIT_LEN32	(0XFFFFFFFF>>(32-32))

#define MAX_TASK			64
#define MAX_INSTANCE	16
/****************************************************************************
 @ enums
****************************************************************************/
typedef enum{DEV_ERROR = 0, DEV_SUCCESS = !DEV_ERROR}	DEV_STATUS;
typedef enum{FALSE = 0, TRUE = !FALSE}	Bool;
typedef enum{NONE_BLOCKING = 0, BLOCKING,} TRANSFER_BLOCK_T;
/*****************************************************************************
 @ typedefs
****************************************************************************/
typedef uint8_t	 	u8;
typedef uint16_t 	u16;
typedef uint32_t 	u32;
typedef int8_t 		s8;
typedef int16_t 	s16;
typedef int32_t 	s32;

typedef struct {
	GPIO_TypeDef* GPIOx;	// Port Register Base addr.
	uint16_t GPIO_Pin;		// Pin
} PIN_T;

typedef struct {
	s32 mul;
	u16 div;
} FLOAT_T;

typedef int32_t Float16B;

typedef struct {
	u32 integer;
	u16 decimal;
} DOUBLE;

typedef struct {
	u8 offset;
	u16 gainMul;
	u16 gainDiv;
} CAL_T;

typedef struct {
	u16 offset;
	u32 gainMul;
	u32 gainDiv;
}CAL32_T;

typedef struct {
	const char *KeyStr;
	const char *ReplaceBy;
} CMD_DEF_T;

#define RESP_STR_LEN	80
typedef struct {
	char str[RESP_STR_LEN];
	s16 bytes;	//actual bytes
	const char *ROM;
} RESP_T;

s16 answerIt(RESP_T* pkt, const char* FORMAT_ORG, ...);

extern char endStr[4];

/* delay about 1us*/
void miscDelay(u8 us);
//void miscDelayMs(u16 ms);
//void miscDelayMs256(u8 ms);
/* 取数组排序后中间80%样品的平均值 */
u8 FilterU8(u8 *pDat, u8 count);
u16 FilterU16(u16 *pDat, u8 count);
/* 排序 */
void sortU8(u8 *pDat, u8 count, u8 type, u8 needLen);
void sortU16(u16 *pDat, u8 count, u8 type, u8 needLen);

//浮点数逼近
void floatByUint(u32* mul, u32* div, u16 base);

//FLOAT2048 FLOAT2048_MUL_FLOAT2048(FLOAT2048 x, FLOAT2048 y);
DOUBLE double_MUL_U32(DOUBLE x, u32 y);
//FLOAT2048 FLOAT2048_MUL_FLOAT2048(FLOAT2048 x, FLOAT2048 y);
//FLOAT2048 FLOAT2048_MUL_U32(FLOAT2048 x, u32 y);

#endif
