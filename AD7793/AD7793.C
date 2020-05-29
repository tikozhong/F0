/**********************************************************
filename: ad7793.c
**********************************************************/

/************************包含头文件***************************************************/
#include "ad7793.h"
#include "publicRsrc.h"

#define MOD_DEFAULT 0X000A
#define CFG_DEFAULT 0X0710
#define EXIT_CONTINUE_READ     (0X58)
#define ENTER_CONTINUE_READ     (0X5C)  

/**********************************************************
 Private function
**********************************************************/
static void ad7793reset(AD7793_Rsrc_T*);
static s8 ad7793startStrm(AD7793_Rsrc_T *pRsrc, u8 ch, u8 gain, u8 pol, u8 fs, u16 times);
static void ad7793stop(AD7793_Rsrc_T*);
static void ad7793polling(AD7793_Rsrc_T* pRsrc, u8 tickUnit);

static u8 ad7793ReadStatusReg(AD7793_Rsrc_T*);
//mode
static u16 ad7793ReadModeReg(AD7793_Rsrc_T*);     //001
static void ad7793WriteModeReg(AD7793_Rsrc_T*, u16);
//config
static u16 ad7793ReadConfigReg(AD7793_Rsrc_T*);     //010
static void ad7793WriteConfigReg(AD7793_Rsrc_T*, u16);
//data 
static u32 ad7793ReadDataReg(AD7793_Rsrc_T*);     //011
//ID
static u8 ad7793ReadIDReg(AD7793_Rsrc_T*);     //100
//IO
static u8 ad7793ReadIOReg(AD7793_Rsrc_T*);     //101
static void ad7793WriteIOReg(AD7793_Rsrc_T*, u8);
//失调寄存器
static u32 ad7793ReadErrorReg(AD7793_Rsrc_T*);     //110
//满量程寄存器
static u32 ad7793ReadFullReg(AD7793_Rsrc_T*);     //111
//lower driver
static void ad7793_writeReg(AD7793_Rsrc_T*, u8 cmd, const u8* dat, u8 len);
static void ad7793_readReg(AD7793_Rsrc_T* pRsrc, u8 cmd, u8* dat, u8 len);
static void ad7793_continueRead(AD7793_Rsrc_T* pRsrc, u8 action);
/**********************************************************
 Public function
**********************************************************/
DEV_STATUS AD7793_Setup(
	AD7793_Dev_T *pDev, 
	const PIN_T CS, 
	const PIN_T SCK, 
	const PIN_T MOSI, 
	const PIN_T MISO,
     u8 clkSel,    //0:ADC clock from internal;  1:adc clock from external
     u8 refSel     //0:ref voltage from internal;  1:ref vol from external
){
     u8 dat[4];
     u8 i, tmp;
     //registe hardware
	AD7793_Rsrc_T* pRsrc = &pDev->rsrc;
	pRsrc->CS = CS;
	pRsrc->SCK = SCK;
	pRsrc->MOSI = MOSI;
	pRsrc->MISO = MISO;

	//cofig IO
	PIN_OP.As_OUTPUT_PP_NOPULL_HIGH(CS);
	PIN_OP.As_OUTPUT_PP_NOPULL_HIGH(SCK);
	PIN_OP.As_OUTPUT_PP_NOPULL_HIGH(MOSI);
	PIN_OP.As_INPUT_NOPULL(MISO);
	
     //register api
     pDev->Reset = ad7793reset;
     pDev->StartStrm = ad7793startStrm;
     pDev->Stop = ad7793stop;
     pDev->Polling = ad7793polling;

     //registe lower driver
     pDev->ReadID = ad7793ReadIDReg;
     pDev->ReadStatus = ad7793ReadStatusReg;
     pDev->ReadMode = ad7793ReadModeReg;
     pDev->WriteMode = ad7793WriteModeReg;
     pDev->ReadConfig = ad7793ReadConfigReg;
     pDev->WriteConfig = ad7793WriteConfigReg;
     pDev->ReadData = ad7793ReadDataReg;

	//hardware initial
     pRsrc->clkCfg = clkSel;
     pRsrc->vefVol = refSel;
     //exit continue read
     HAL_GPIO_WritePin(pRsrc->CS.GPIOx, pRsrc->CS.GPIO_Pin, GPIO_PIN_RESET);
     tmp = 0x58;    //0101 1000
     while(1){
          if(HAL_GPIO_ReadPin(pRsrc->MISO.GPIOx, pRsrc->MISO.GPIO_Pin) == GPIO_PIN_RESET){
               for(i=0;i<8;i++){
                    if(tmp & 0x80) HAL_GPIO_WritePin(pRsrc->MOSI.GPIOx, pRsrc->MOSI.GPIO_Pin, GPIO_PIN_SET);
                    else HAL_GPIO_WritePin(pRsrc->MOSI.GPIOx, pRsrc->MOSI.GPIO_Pin, GPIO_PIN_RESET);
                    HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_SET);
                    tmp <<= 1;
                    HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_RESET);
               }          
               break;
          }
     }
     HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_SET);
     HAL_GPIO_WritePin(pRsrc->CS.GPIOx, pRsrc->CS.GPIO_Pin, GPIO_PIN_SET);
     //select internal clock
//     if(clkSel == 0)     ad7793WriteModeReg(pRsrc, MOD_DEFAULT|(1<<6));  
//     //select internal refVol
//     if(refSel == 0)     ad7793WriteConfigReg(pRsrc, CFG_DEFAULT|(1<<7));
     
     HAL_GPIO_WritePin(pRsrc->CS.GPIOx, pRsrc->CS.GPIO_Pin, GPIO_PIN_RESET);
	return DEV_SUCCESS;
}

/**********************************************************
 Public function
**********************************************************/
static void ad7793reset(AD7793_Rsrc_T* pRsrc){
     u8 i;
     HAL_GPIO_WritePin(pRsrc->CS.GPIOx, pRsrc->CS.GPIO_Pin, GPIO_PIN_RESET);
     HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_RESET);
     HAL_GPIO_WritePin(pRsrc->MOSI.GPIOx, pRsrc->MOSI.GPIO_Pin, GPIO_PIN_SET);
     for(i=0;i<32;i++){
         HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_SET); 
         HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_RESET); 
     }
     HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_SET);
     HAL_GPIO_WritePin(pRsrc->MOSI.GPIOx, pRsrc->MOSI.GPIO_Pin, GPIO_PIN_SET);
     HAL_GPIO_WritePin(pRsrc->CS.GPIOx, pRsrc->CS.GPIO_Pin, GPIO_PIN_RESET);     
}
static s8 ad7793startStrm(AD7793_Rsrc_T *pRsrc, u8 ch, u8 gain, u8 pol, u8 fs, u16 times){
     u16 tmp;
     
     
     
     //config
     tmp = ad7793ReadConfigReg(pRsrc);
     

     //mode


}

static void ad7793stop(AD7793_Rsrc_T* pRsrc){
     
     
}

static void ad7793polling(AD7793_Rsrc_T* pRsrc, u8 tickUnit){
     
     if(pRsrc->exitContinueRead){
          
          
     }
     
     if(HAL_GPIO_ReadPin(pRsrc->MISO.GPIOx, pRsrc->MISO.GPIO_Pin) == GPIO_PIN_RESET){

          ad7793_continueRead(pRsrc, EXIT_CONTINUE_READ);
     }     
     
     switch (pRsrc->squ){
          case 0:   //
               break;
          case 1:   //exit continue read
               if(HAL_GPIO_ReadPin(pRsrc->MISO.GPIOx, pRsrc->MISO.GPIO_Pin) == GPIO_PIN_RESET){
                    ad7793_continueRead(pRsrc, EXIT_CONTINUE_READ);
               }
               break;
          case 2:
               break;
          case 3:
               break;
          default:
               break;
          
          
          
     }




}

/**********************************************************
 ad7793_writeReg//000
**********************************************************/
static u8 ad7793ReadStatusReg(AD7793_Rsrc_T* pRsrc){
     u8 tmp;
     //01xxx000 = 01 000 000
     ad7793_readReg(pRsrc, 0x40, &tmp, 1);
     return tmp;
}
/**********************************************************
 ad7793_writeReg//001
**********************************************************/
static u16 ad7793ReadModeReg(AD7793_Rsrc_T* pRsrc){
     u16 tmp;
     //01xxx000 = 01 001 000
     ad7793_readReg(pRsrc, 0x48, (u8*)&tmp, 2);
     return tmp; 
}
static void ad7793WriteModeReg(AD7793_Rsrc_T* pRsrc, u16 reg){
     u16 tmp = reg;
     //00xxx000 = 00 001 000
     ad7793_writeReg(pRsrc, 0x08|0x04, (u8*)&tmp, 2);
}
/**********************************************************
 ad7793_writeReg//010
**********************************************************/
static u16 ad7793ReadConfigReg(AD7793_Rsrc_T* pRsrc){
     u16 tmp;
     //01xxx000 = 01 010 000
     ad7793_readReg(pRsrc, 0x50|0x04, (u8*)&tmp, 2);
     return tmp;
}       
static void ad7793WriteConfigReg(AD7793_Rsrc_T* pRsrc, u16 reg){
     u16 tmp = reg;
     //00xxx000 = 00 010 000
     ad7793_writeReg(pRsrc, 0x10|0x04, (u8*)&tmp, 2);
}
/**********************************************************
 ad7793_writeReg//011
**********************************************************/
static u32 ad7793ReadDataReg(AD7793_Rsrc_T* pRsrc){
     u32 tmp;
     //01xxx000 = 01 011 000
     ad7793_readReg(pRsrc, 0x58, (u8*)&tmp, 3);
     return tmp;   
}   
/**********************************************************
 ad7793ReadIDReg//100
**********************************************************/
static u8 ad7793ReadIDReg(AD7793_Rsrc_T* pRsrc){
     u8 tmp;
     //01xxx000 = 01 100 000 = 0110 0000
     ad7793_readReg(pRsrc, 0x60, &tmp, 1);
     return tmp;
}    
/**********************************************************
 ad7793ReadIOReg//101
**********************************************************/
static u8 ad7793ReadIOReg(AD7793_Rsrc_T* pRsrc){
     u8 tmp;
     //01xxx000 = 01 101 000 = 0110 1000
     ad7793_readReg(pRsrc, 0x68, &tmp, 1);
     return tmp;
}    
static void ad7793WriteIOReg(AD7793_Rsrc_T* pRsrc, u8 reg){
}
/**********************************************************
 ad7793ReadErrorReg//110
**********************************************************/
static u32 ad7793ReadErrorReg(AD7793_Rsrc_T* pRsrc){
     u32 tmp;
     //01xxx000 = 01 110 000 = 0111 0000
     ad7793_readReg(pRsrc, 0x70, (u8*)&tmp, 4);
     return tmp;
}    
/**********************************************************
 ad7793ReadFullReg//111
**********************************************************/
static u32 ad7793ReadFullReg(AD7793_Rsrc_T* pRsrc){
     u32 tmp;
     //01xxx000 = 01 111 000 = 0111 1000
     ad7793_readReg(pRsrc, 0x78, (u8*)&tmp, 4);
     return tmp;
}   


/**********************************************************
 ad7793_exitContinueRead
**********************************************************/
static void ad7793_continueRead(AD7793_Rsrc_T* pRsrc, u8 action){
     u8 i,tmp;
     //0101 1000
     tmp = action;
     HAL_GPIO_WritePin(pRsrc->CS.GPIOx, pRsrc->CS.GPIO_Pin, GPIO_PIN_RESET);      
     HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_RESET);
     for(i=0;i<8;i++){
          if(tmp & 0x80) HAL_GPIO_WritePin(pRsrc->MOSI.GPIOx, pRsrc->MOSI.GPIO_Pin, GPIO_PIN_SET);
          else HAL_GPIO_WritePin(pRsrc->MOSI.GPIOx, pRsrc->MOSI.GPIO_Pin, GPIO_PIN_RESET);
          HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_SET);
          tmp <<= 1;
          HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_RESET);
     }  
     HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_SET);
     HAL_GPIO_WritePin(pRsrc->MOSI.GPIOx, pRsrc->MOSI.GPIO_Pin, GPIO_PIN_SET);       
}

/**********************************************************
 ad7793_writeReg
**********************************************************/
static void	ad7793_writeReg(AD7793_Rsrc_T* pRsrc,  u8 cmd, const u8* dat, u8 len){
     u8 i,j,tmp=cmd;
     HAL_GPIO_WritePin(pRsrc->CS.GPIOx, pRsrc->CS.GPIO_Pin, GPIO_PIN_RESET);
     HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_RESET);
     for(i=0;i<8;i++){
          if(tmp & 0x80) HAL_GPIO_WritePin(pRsrc->MOSI.GPIOx, pRsrc->MOSI.GPIO_Pin, GPIO_PIN_SET);
          else HAL_GPIO_WritePin(pRsrc->MOSI.GPIOx, pRsrc->MOSI.GPIO_Pin, GPIO_PIN_RESET);
          HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_SET);
          tmp <<= 1;
          HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_RESET);
     }     
     for(i=len-1;1;i--){
          tmp = dat[i];
          for(j=0;j<8;j++){
               HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_RESET);
               if(tmp & 0x80) HAL_GPIO_WritePin(pRsrc->MOSI.GPIOx, pRsrc->MOSI.GPIO_Pin, GPIO_PIN_SET);
               else HAL_GPIO_WritePin(pRsrc->MOSI.GPIOx, pRsrc->MOSI.GPIO_Pin, GPIO_PIN_RESET);
               HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_SET);
               tmp <<= 1;
               HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_RESET);
          }
          if(i==0)  break;
     }
     HAL_GPIO_WritePin(pRsrc->CS.GPIOx, pRsrc->CS.GPIO_Pin, GPIO_PIN_SET);
     HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_SET);
     HAL_GPIO_WritePin(pRsrc->MOSI.GPIOx, pRsrc->MOSI.GPIO_Pin, GPIO_PIN_SET);  
}
/**********************************************************
 ad7793_readReg
**********************************************************/
static void ad7793_readReg(AD7793_Rsrc_T* pRsrc, u8 cmd, u8* dat, u8 len){
     u8 i,j,tmp = cmd;
     HAL_GPIO_WritePin(pRsrc->CS.GPIOx, pRsrc->CS.GPIO_Pin, GPIO_PIN_RESET);
     HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_RESET);
     for(i=0;i<8;i++){
          if(tmp & 0x80) HAL_GPIO_WritePin(pRsrc->MOSI.GPIOx, pRsrc->MOSI.GPIO_Pin, GPIO_PIN_SET);
          else HAL_GPIO_WritePin(pRsrc->MOSI.GPIOx, pRsrc->MOSI.GPIO_Pin, GPIO_PIN_RESET);
          HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_SET);
          tmp <<= 1;
          HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_RESET);
     }
     HAL_GPIO_WritePin(pRsrc->MOSI.GPIOx, pRsrc->MOSI.GPIO_Pin, GPIO_PIN_RESET);
     for(i=len-1;1;i--){
          for(int j=0; j<8; j++){
               dat[i] <<=1;
               HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_SET);
               if(HAL_GPIO_ReadPin(pRsrc->MISO.GPIOx, pRsrc->MISO.GPIO_Pin) == GPIO_PIN_SET)     dat[i] |= 0x01;  
               HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_RESET);
          }
          if(i == 0)     break;
     }
     HAL_GPIO_WritePin(pRsrc->CS.GPIOx, pRsrc->CS.GPIO_Pin, GPIO_PIN_SET);
     HAL_GPIO_WritePin(pRsrc->SCK.GPIOx, pRsrc->SCK.GPIO_Pin, GPIO_PIN_SET);
     HAL_GPIO_WritePin(pRsrc->MOSI.GPIOx, pRsrc->MOSI.GPIO_Pin, GPIO_PIN_SET);  
}

/**********************************************************
 == THE END ==
**********************************************************/
