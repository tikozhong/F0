/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   ADS1220.H
 * Author: DAVID WIEBE
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef ADS1220_H
#define	ADS1220_H

#include <misc.h>

//////////////////////////////////////////////////////////////////////////////
/***********************Program Constants************************************/

#define ADS1220_RESET       0x06    //Reset ADS1220 registry to default
#define ADS1220_WRITE_REG   0x43    //Write 4 bytes starting @ address 0
#define ADS1220_START       0x08    //Start taking temperature readings
#define ADS1220_READ        0x10    //Read data on command
//3 Wire RTD Registry Settings
#define ADS1220_3W_REG_0    0x60    //0110-Input Multiplex, XXX - User Set Gain 
                                    //0-PGA Enabled
#define ADS1220_3W_REG_1    0x00    //XXXXX- User Set SPS, 0-Single Sample, 
                                    //0-Int Temp Disabled, 0-Burnout sensor off
#define ADS1220_3W_REG_2_FLTR   0x55//01-Ext Ref inputs, 01-50Hz&60Hz Filter, 
                                    //0-Low side power switch, 101-0.5mA IDAC
#define ADS1220_3W_REG_2_NOFLTR 0x45//01-Ext Ref inputs, 00-Filter Disabled, 
                                    //0-Low side power switch, 101-0.5mA IDAC
#define ADS1220_3W_REG_3    0x70    //011-IDAC1 to AIN2, 100-IDAC2 to AIN3, 
                                    //0-DRDY pin used for data ready, 
                                    //0-Always write zero for chip
//ADS1220 Internal Offset Calibration Registry Settings
#define ADS1220_ICOFFSET_REG_0  0xE0    //1110-Short Multiplex for calibration, 
                                        //XXX- User Set Gain, 0-PGA Enabled  
//#define ADS1220_ICOFFSET_REG_1  0X04    //XXXXX- User Set SPS Settings
                                        //1-Continuous Sample, 0-Int Temp 
                                        //Disabled, 0-Burnout sensor off
//#define ADS1220_ICOFFSET_REG_2  0X50    //01-Ext Ref inputs, 01-50Hz&60Hz Filter
                                        //0-Low side power swtch, 000- IDACs Off
//#define ADS1220_ICOFFSET_REG_3  0X0   //000-IDAC1 Disabled, 000-IDAC2 Disabled, 
                                        //0-DRDY pin used for data ready, 
                                        //0-Always write zero for chip
//MUX                                     
#define MUX_AIN0_AIN1 0x00   //0000 : AINP = AIN0, AINN = AIN1 (default)
#define MUX_AIN0_AIN2 0x01   //0001 : AINP = AIN0, AINN = AIN2
#define MUX_AIN0_AIN3 0x02   //0010 : AINP = AIN0, AINN = AIN3
#define MUX_AIN1_AIN2 0x03   //0011 : AINP = AIN1, AINN = AIN2
#define MUX_AIN1_AIN3 0x04   //0100 : AINP = AIN1, AINN = AIN3
#define MUX_AIN2_AIN3 0x05   //0101 : AINP = AIN2, AINN = AIN3 
#define MUX_AIN1_AIN0 0x06   //0110 : AINP = AIN1, AINN = AIN0
#define MUX_AIN3_AIN2 0x07   //0111 : AINP = AIN3, AINN = AIN2
#define MUX_AIN0_AVSS 0x08   //1000 : AINP = AIN0, AINN = AVSS
#define MUX_AIN1_AVSS 0x09   //1001 : AINP = AIN1, AINN = AVSS
#define MUX_AIN2_AVSS 0x0a   //1010 : AINP = AIN2, AINN = AVSS
#define MUX_AIN3_AVSS 0x0b   //1011 : AINP = AIN3, AINN = AVSS
#define MUX_REF_P_N_4 0x0c   //1100 : (V(REFPx) - V(REFNx)) / 4 monitor (PGA bypassed)
#define MUX_VDD_VSS_4 0x0d   //1101 : (AVDD - AVSS) / 4 monitor (PGA bypassed)
#define MUX_VDD_VSS_2 0x0e   //1110 : AINP and AINN shorted to (AVDD + AVSS) / 2
#define MUX_RESERVED  0x0f   //1111 : Reserved                                     
                                        
//Available Gain Options
#define ADS1220_GAIN_1      0x00
#define ADS1220_GAIN_2      0x02
#define ADS1220_GAIN_4      0x04
#define ADS1220_GAIN_8      0x06
#define ADS1220_GAIN_16     0x08
#define ADS1220_GAIN_32     0x0A
#define ADS1220_GAIN_64     0x0C
#define ADS1220_GAIN_128    0x0E

//Available Samples Per Second Options
#define ADS1220_SPS_20      0x0
#define ADS1220_SPS_40      0x10
#define ADS1220_SPS_90      0x30
#define ADS1220_SPS_180     0x50
#define ADS1220_SPS_350     0x70
#define ADS1220_SPS_660     0x90
#define ADS1220_SPS_1200    0xB0
#define ADS1220_SPS_2000    0xD0

//Value used for Errors
#define ADS1220_ERROR       0x7F

//Constants used to calculate resistance and temperature from Bytes read
#define ADS1220_rZero       100             //PT100 resistance at 0C
#define ADS1220_FSR         (((uint32_t)1<<23)-1) //Full scale for signed 24 bit
#define ADS1220_VREF        2.49            //2.49 Based on Board Design and selected Vref

//#include <stdint.h>
//#include <stdbool.h>
//#include <math.h>

#include <stdint.h>
#include "misc.h"

///////////////////////////////////////////////////////////////////////////////
/*******************ADS1220 Chip Command Functions****************************/

void performReset( void );
//functions called by i2c interrupt;  they set gainRegisteryValue and 
//gainRegistryValue based on user inputs.  Need to call initializeThreeWireRTD() 
//after to update chip registry
void setGain( void );       
void setSPS ( void );
//performs internal calibration of ADS1220; called automatically by 
//initializeThreeWireRTD())
void performInternalOffset( void ); 
//calls performInternalOffset and Writes settings to register for 3wire RTD
//make sure to call setGain and setSPS before initializing RTD
void initializeThreeWireRTD( void ); 
void readRTDBytes(u8 rtd); //reads temperatures from ADS1220
void readRTDResistance(u8 rtd);
void readRTDTemperatureC(uint8_t rtd);
void readRTDTemperatureF(uint8_t rtd);

///////////////////////////////////////////////////////////////////////////////
/***************************Global Variables**********************************/

//Program written for 8-bit PIC (PIC16F18345)
//union floatNBytes4
//{
//    float f[4];
//    uint8_t b[4][4];
//}rtdTemperatureC, rtdTemperatureF;

//union floatNBytes5
//{
//    float f[5];
//    uint8_t b[5][4];
//}rtdResistance;

//union parameters
//{
//    float f;
//    uint8_t b[4];
//}a_parameter, b_parameter;

//uint8_t rtdBytes [5][3] = {0}; //includes icOffset value
//uint8_t gainRegisterValue;
//uint8_t spsRegisterValue;
//uint8_t gainValue;
//uint8_t spsValue;
//uint32_t offset32 = 0;
//uint8_t garbage; //use to record values returned use when transmitting SPI info
//u8 waitForRTDInitialization = 1;


typedef struct
{
	/* config	*/
	PIN_T CS;
	PIN_T SCK; 
	PIN_T MISO; 
	PIN_T MOSI;
	//reg copy

	u8 tick,squ;
	u8 exitContinueRead;
	void (*printMsg)(s32 adcVal);	
	void (*delay)(u8 ms);
//	tAD779X_RDYState RDYState;
	
	u8 curCh;
} ads1220Rsrc_t;

typedef struct
{
	ads1220Rsrc_t rsrc;
	//
	u8 (*ReadReg)     (ads1220Rsrc_t *pRsrc, u8 addr);
	void (*WriteReg)  (ads1220Rsrc_t *pRsrc, u8 addr, u8 reg);
	void (*Reset)     (ads1220Rsrc_t *pRsrc);
	void (*PowerDown) (ads1220Rsrc_t *pRsrc);
	void (*Start)     (ads1220Rsrc_t *pRsrc);
	u32 (*ReadData)   (ads1220Rsrc_t *pRsrc);
	void (*Polling)	  (ads1220Rsrc_t *pRsrc, u8 tick);
	

	//api
	u32 (*adcCode)(ads1220Rsrc_t *pRsrc);
	u32 (*adcVolt)(ads1220Rsrc_t *pRsrc);
} ads1220Dev_t;

void ADS1220_Setup(
	ads1220Dev_t* pDev,	//device
	const PIN_T CS, 		//chip select pin
	const PIN_T SCK, 		//serial clok pin
	const PIN_T MOSI, 	//host out, device in
	const PIN_T MISO		//host in, device out
);
#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* ADS1220_H */

