/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : misc.c
* Author             : Tiko Zhong
* Date First Issued  : 07/03/201
* Description        : 
*                      
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "misc.h"
#include "stdarg.h"
#include "myString.h"
#include "string.h"
/* Private define ------------------------------------------------------------*/
#define DEV_MAX 64
/* Private typedef -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
char endStr[4] = {0};
/* Private function prototypes -----------------------------------------------*/
/*******************************************************************************
* Function Name  : delay
* Description    : delay for n us. COUNT_OF_1US should be tuned in different plat
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#define COUNT_OF_1US 12
void miscDelay(u8 us){
	u8 usX1;
	u8 delayUs = us;
	while(delayUs){
		for(usX1=0; usX1<COUNT_OF_1US; usX1++){}
		delayUs --;
	}
}

//void miscDelayMs(u16 ms){
//	osDelay(ms);
//}

//void miscDelayMs256(u8 ms){
//	osDelay(ms);
//}

/*******************************************************************************
* Function Name  : to get aver of 80% samples in middle scope 
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u8 FilterU8(u8 *pDat, u8 count){
	u8 avg = 0, i,min,max;
	u16 sum = 0, tmpU16;
	
	if(count==0)	return avg;
	if(count==1)	return pDat[0];
	if(count==2)	return ((pDat[0]>>2)+(pDat[1]>>2));
	min = pDat[0];	max = pDat[0];
	for(i=0; i<count; i++)	{
		sum += pDat[i];
		if(pDat[i]>max)	max = pDat[i];
		if(pDat[i]<min)	min = pDat[i];
	}
	if(count < 20){
		sum -= (min+max);
		sum /= (count-2);
		return (u8)sum;
	}

	tmpU16 = count*13;	tmpU16 >>= 7;	
	sortU8(pDat,count,0,tmpU16);
	for(i=0;i<tmpU16;i++)	sum -= pDat[i];
	sortU8(pDat,count,1,tmpU16);
	for(i=0;i<tmpU16;i++)	sum -= pDat[i];
	
	avg = sum/(count-(tmpU16<<1));
	return avg;
}

void sortU8(u8 *pDat, u8 count, u8 type, u8 needLen){
	u8 i,j,val,tmpU8,indx,found;
	
	for(i=0;i<needLen&&i<count-1;i++){
		val = pDat[i];	found = 0;
		for(j=i+1;j<count;j++){
			if(type==0 && pDat[j]<val){	//from 0..1..2..n
				val = pDat[j];
				indx = j;
				found = 1;
			}
			if(type>0 && pDat[j]>val){	//from n..2..1..0
				val = pDat[j];
				indx = j;
				found = 1;
			}
		}
		if(found){
			tmpU8 = pDat[i];	
			pDat[i] = pDat[indx];	
			pDat[indx] = tmpU8;
		}
	}
}

void sortU16(u16 *pDat, u8 count, u8 type, u8 needLen){
	u8 i,j,val,indx,found;
	u16 tmpU16;
	
	for(i=0;i<needLen&&i<count-1;i++){
		val = pDat[i];	found = 0;
		for(j=i+1;j<count;j++){
			if(type==0 && pDat[j]<val){	//from 0..1..2..n
				val = pDat[j];
				indx = j;
				found = 1;
			}
			if(type>0 && pDat[j]>val){	//from n..2..1..0
				val = pDat[j];
				indx = j;
				found = 1;
			}
		}
		if(found){
			tmpU16 = pDat[i];	
			pDat[i] = pDat[indx];	
			pDat[indx] = tmpU16;
		}
	}
}

u16 FilterU16(u16 *pDat, u8 count){
	u8 i;
	u16 avg = 0, min, max;
	u32 sum = 0, tmpU32;
	
	if(count==0)	return avg;
	if(count==1)	return pDat[0];
	if(count==2)	return ((pDat[0]>>2)+(pDat[1]>>2));
	min = pDat[0];	max = pDat[0];
	for(i=0; i<count; i++)	{
		sum += pDat[i];
		if(pDat[i]>max)	max = pDat[i];
		if(pDat[i]<min)	min = pDat[i];
	}
	if(count < 20){
		sum -= (min+max);
		sum /= (count-2);
		return (u8)sum;
	}

	tmpU32 = count*13;	tmpU32 >>= 7;	
	sortU16(pDat,count,0,tmpU32);
	for(i=0;i<tmpU32;i++)	sum -= pDat[i];
	sortU16(pDat,count,1,tmpU32);
	for(i=0;i<tmpU32;i++)	sum -= pDat[i];
	
	avg = sum/(count-(tmpU32<<1));
	return avg;
}

void floatByUint(u32* mul, u32* div, u16 base){
	u16 tmp,tmp1;
	u32 sub;
	if(*div < base)	return;
	tmp = (*mul)/(*div);
	tmp1 = (*mul)%(*div);
	*mul = tmp*base;
	sub = tmp1*base/(*div);
	*mul += sub;
	*div = base;
}

DOUBLE double_MUL_U32(DOUBLE x, u32 y){
     DOUBLE rtn = {0};
     u32 tmp = x.decimal*y;
     rtn.integer = x.integer*y + (tmp>>16);
     rtn.decimal = tmp&0x0000ffff;
     return rtn;
}

s16 answerIt(RESP_T* pkt, const char* FORMAT_ORG, ...){
	va_list ap;
	//take string
	va_start(ap, FORMAT_ORG);
	pkt->bytes = vsnprintf(pkt->str, RESP_STR_LEN, FORMAT_ORG, ap);
	va_end(ap);
	return pkt->bytes;
}


/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
