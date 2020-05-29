/*
 * File:   ADS1220.c
 * Author: David W
 *
 * Created on November 6, 2016, 2:11 PM
 */

#include "ADS1220.h"
#include "publicRsrc.h"
#include "myuart.h"


static void ads1220Reset     (ads1220Rsrc_t *pRsrc);
static void ads1220PowerDown (ads1220Rsrc_t *pRsrc);
static void ads1220Start     (ads1220Rsrc_t *pRsrc);
static u8 ads1220ReadData   (ads1220Rsrc_t *pRsrc, u32* dat);
static void ads1220Polling   (ads1220Rsrc_t *pRsrc, u8 tick);

static u8 ads1220ReadReg     (ads1220Rsrc_t *pRsrc, u8 addr);
static void ads1220WriteReg  (ads1220Rsrc_t *pRsrc, u8 addr, u8 reg);

static void ads1220Command(ads1220Rsrc_t *pRsrc, u8 cmd);
//lower driver
//static void ads1220WriteReg(ads1220Rsrc_t *pRsrc, u8 regAddr, u8 reg);
//static u8 ads1220ReadReg(ads1220Rsrc_t *pRsrc, u8 regAddr);

void ADS1220_Setup(
	ads1220Dev_t* pDev,	//device
	const PIN_T CS, 		//chip select pin
	const PIN_T SCK, 		//serial clok pin
	const PIN_T MOSI, 	//host out, device in
	const PIN_T MISO		//host in, device out
){
     pDev->rsrc.CS = CS;
     pDev->rsrc.SCK = SCK;
     pDev->rsrc.MOSI = MOSI;
     pDev->rsrc.MISO = MISO;
     
     //op
     pDev->ReadReg = ads1220ReadReg;
     pDev->WriteReg = ads1220WriteReg;
     pDev->Reset = ads1220Reset;
     pDev->PowerDown = ads1220PowerDown;
     pDev->Start = ads1220Start;
     pDev->Polling = ads1220Polling;
     
     PIN_OP.As_OUTPUT_PP_NOPULL_HIGH(CS);
     PIN_OP.As_OUTPUT_PP_NOPULL_LOW(SCK);
     PIN_OP.As_OUTPUT_PP_NOPULL_HIGH(MOSI);
     PIN_OP.As_INPUT_NOPULL(MISO);
}

static void ads1220Polling   (ads1220Rsrc_t *pRsrc, u8 tick){
     u32 dat;
     pRsrc->tick += tick;

     if(pRsrc->tick > 50){     //per 30ms a run
          switch(pRsrc->squ){
               case 0:   //
                    break;
               case 1:
                    if(ads1220ReadData(pRsrc, &dat)){
                         pRsrc->squ = 0;
                         print("+msg@adcVal(%u,\t%u\tuv)\n", dat, dat*61/1000);
                    }
                    break;
          }
     }
}

static void ads1220WriteReg(ads1220Rsrc_t *pRsrc, u8 regAddr, u8 regVal){
     u8 tmp = 0,i;
     tmp = 0x40 | ((regAddr&0x03)<<2);

     HAL_GPIO_WritePin(pRsrc->CS.GPIOx, pRsrc->CS.GPIO_Pin, GPIO_PIN_RESET);
     HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_RESET);

     for(i=0x80;i>0;i>>=1){
          if(tmp & i) HAL_GPIO_WritePin(pRsrc->MOSI.GPIOx, pRsrc->MOSI.GPIO_Pin, GPIO_PIN_SET);
          else        HAL_GPIO_WritePin(pRsrc->MOSI.GPIOx, pRsrc->MOSI.GPIO_Pin, GPIO_PIN_RESET);
          HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_SET);
          HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_RESET);
     }
     tmp = regVal;
     for(i=0x80;i>0;i>>=1){
          if(tmp & i) HAL_GPIO_WritePin(pRsrc->MOSI.GPIOx, pRsrc->MOSI.GPIO_Pin, GPIO_PIN_SET);
          else        HAL_GPIO_WritePin(pRsrc->MOSI.GPIOx, pRsrc->MOSI.GPIO_Pin, GPIO_PIN_RESET);
          HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_SET);
          HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_RESET);
     }

     HAL_GPIO_WritePin(pRsrc->CS.GPIOx, pRsrc->CS.GPIO_Pin, GPIO_PIN_SET);
}

static u8 ads1220ReadReg(ads1220Rsrc_t *pRsrc, u8 regAddr){
     u8 tmp = 0,i;
     tmp = 0x20 | ((regAddr&0x03)<<2);

     HAL_GPIO_WritePin(pRsrc->CS.GPIOx,pRsrc->CS.GPIO_Pin,GPIO_PIN_RESET);
     HAL_GPIO_WritePin(pRsrc->SCK.GPIOx,pRsrc->SCK.GPIO_Pin,GPIO_PIN_RESET);

     for(i=0x80;i>0;i>>=1){
          if(tmp & i) HAL_GPIO_WritePin(pRsrc->MOSI.GPIOx, pRsrc->MOSI.GPIO_Pin, GPIO_PIN_SET);
          else           HAL_GPIO_WritePin(pRsrc->MOSI.GPIOx, pRsrc->MOSI.GPIO_Pin, GPIO_PIN_RESET);
          HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_SET);
          HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_RESET);
     }
     HAL_GPIO_WritePin(pRsrc->MOSI.GPIOx, pRsrc->MOSI.GPIO_Pin, GPIO_PIN_RESET);
     tmp = 0;
     for(i=0x80;i>0;i>>=1){
          HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_SET);
          if(HAL_GPIO_ReadPin(pRsrc->MISO.GPIOx, pRsrc->MISO.GPIO_Pin) == GPIO_PIN_SET)  tmp |= i;
          HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_RESET);
     }

     HAL_GPIO_WritePin(pRsrc->CS.GPIOx, pRsrc->CS.GPIO_Pin, GPIO_PIN_SET);
     return tmp;
}

static void ads1220Command(ads1220Rsrc_t *pRsrc, u8 cmd){
     u8 i;
     
     HAL_GPIO_WritePin(pRsrc->CS.GPIOx,pRsrc->CS.GPIO_Pin,GPIO_PIN_RESET);
     HAL_GPIO_WritePin(pRsrc->SCK.GPIOx,pRsrc->SCK.GPIO_Pin,GPIO_PIN_RESET);

     for(i=0x80;i>0;i>>=1){
          if(cmd & i) HAL_GPIO_WritePin(pRsrc->MOSI.GPIOx, pRsrc->MOSI.GPIO_Pin, GPIO_PIN_SET);
          else           HAL_GPIO_WritePin(pRsrc->MOSI.GPIOx, pRsrc->MOSI.GPIO_Pin, GPIO_PIN_RESET);
          HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_SET);
          HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_RESET);
     }

     HAL_GPIO_WritePin(pRsrc->CS.GPIOx, pRsrc->CS.GPIO_Pin, GPIO_PIN_SET);
}

static void ads1220Reset(ads1220Rsrc_t *pRsrc){
     ads1220Command(pRsrc, 0x06);
}

static void ads1220PowerDown(ads1220Rsrc_t *pRsrc){
     ads1220Command(pRsrc, 0x02);
}

static void ads1220Start(ads1220Rsrc_t *pRsrc){
     ads1220Command(pRsrc, 0x08);
     pRsrc->squ = 1;
}

static u8 ads1220ReadData(ads1220Rsrc_t *pRsrc, u32* dat){
     u8 i;

     HAL_GPIO_WritePin(pRsrc->CS.GPIOx,pRsrc->CS.GPIO_Pin,GPIO_PIN_RESET);
     HAL_GPIO_WritePin(pRsrc->SCK.GPIOx,pRsrc->SCK.GPIO_Pin,GPIO_PIN_RESET);

     if(HAL_GPIO_ReadPin(pRsrc->MISO.GPIOx, pRsrc->MISO.GPIO_Pin) == GPIO_PIN_SET)   return 0;

     *dat = 0;
     for(i=0;i<24;i++){
          *dat<<=1;
          HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_SET);
          if(HAL_GPIO_ReadPin(pRsrc->MISO.GPIOx, pRsrc->MISO.GPIO_Pin) == GPIO_PIN_SET)  *dat |= 0x01;
          HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_RESET);
     }

     HAL_GPIO_WritePin(pRsrc->CS.GPIOx, pRsrc->CS.GPIO_Pin, GPIO_PIN_SET);
     return 1;
}
