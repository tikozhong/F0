/**********************************************************
filename: w5500x.c
**********************************************************/

/************************包含头文件***************************************************/
#include "w5500x.h"
#include "w5500.h"
#include "publicRsrc.h"

#include <string.h>
/**********************************************************
 Private function
**********************************************************/
//api
static void setIp	(W5500_Rsrc_T*, u8 ip0, u8 ip1, u8 ip2, u8 ip3);
static void setIpMask	(W5500_Rsrc_T*, u8 msk0, u8 msk1, u8 msk2, u8 msk3);
static void setGateWay	(W5500_Rsrc_T*, u8 gw0, u8 gw1, u8 gw2, u8 gw3);
static void getNetInfo	(W5500_Rsrc_T*);

//lower level driver
static void w5500x_takeSpiRsrc(void);
static void w5500x_freeSpiRsrc(void);
static void w5500x_spiEnable(void);
static void w5500x_spiDisable(void);
static u8 w5500x_spi_readbyte(void);
static void 	w5500x_spi_writebyte(u8 wb);
static void 	w5500x_spi_readburst(u8* pBuf, u16 len);
static void 	w5500x_spi_writeburst(u8* pBuf, u16 len);

//socket buf config
static u8 scktTxSzCfg[8] = {2,2,2,2,2,2,2,2};	//each socket has 2k bytes tx buffer
static u8 scktRxSzCfg[8] = {2,2,2,2,2,2,2,2};	//each socket has 2k bytes rx buffer
//wiz_NetInfo gWIZNETINFO = { .mac = {0x00, 0x08, 0xdc,0x00, 0xab, 0xcd},
//                            .ip = {192, 168, 1, 123},
//                            .sn = {255,255,255,0},
//                            .gw = {192, 168, 1, 1},
//                            .dns = {0,0,0,0},
//                            .dhcp = NETINFO_STATIC };
//public share
W5500_Dev_T* pW5500Dev = NULL;
W5500_Rsrc_T* pRsrc = NULL;
/**********************************************************
 Public function
**********************************************************/
DEV_STATUS W5500_Setup(
	W5500_Dev_T *pDev, 
	SPI_HandleTypeDef* SPI,
	const PIN_T CS, 
	const PIN_T IRQ
){
	u32 error;
	pW5500Dev = pDev;
	pRsrc = &pDev->rsrc;
	memset(pRsrc,0,sizeof(W5500_Rsrc_T));
	pRsrc->WIZCHIP = &WIZCHIP;	//define in wizchip_conf.c, just copy first
	//critical control
	WIZCHIP.CRIS._enter = w5500x_takeSpiRsrc;
	WIZCHIP.CRIS._exit = w5500x_freeSpiRsrc;
	//chip select
	WIZCHIP.CS._select = w5500x_spiEnable;
	WIZCHIP.CS._deselect = w5500x_spiDisable;
	//spi read and write
	WIZCHIP.IF.SPI._read_byte = w5500x_spi_readbyte;
	WIZCHIP.IF.SPI._read_burst = w5500x_spi_readburst;
	WIZCHIP.IF.SPI._write_byte = w5500x_spi_writebyte;
	WIZCHIP.IF.SPI._write_burst = w5500x_spi_writeburst;
	
	//COFNIG IO
	pRsrc->IRQ = IRQ;	
	pRsrc->CS = CS;
	pRsrc->SPI = SPI;

	PIN_OP.As_OUTPUT_PP_NOPULL_HIGH(CS);
	PIN_OP.As_INPUT_PULLUP(IRQ);
	
	//api
	pDev->GetNetInfo = getNetInfo;
	pDev->SetIp = setIp;
	pDev->SetIpMask = setIpMask;
	pDev->SetGateWay = setGateWay;
	
	//config socket buf len according to scktBufCfg
	if(wizchip_init(scktTxSzCfg, scktRxSzCfg) <0)		pRsrc->error |= 1U<<0;
	
	//config net info
	//wiz_NetInfo gWIZNETINFO = { .mac = {0x00, 0x08, 0xdc,0x00, 0xab, 0xcd},
	//                            .ip = {192, 168, 1, 123},
	//                            .sn = {255,255,255,0},
	//                            .gw = {192, 168, 1, 1},
	//                            .dns = {0,0,0,0},
	//                            .dhcp = NETINFO_STATIC };
	pRsrc->netInfo.ip[0] = 192;
	pRsrc->netInfo.ip[1] = 168;
	pRsrc->netInfo.ip[2] = 1;
	pRsrc->netInfo.ip[3] = 123;
	pRsrc->netInfo.sn[0] = 255;
	pRsrc->netInfo.sn[1] = 255;
	pRsrc->netInfo.sn[2] = 255;
	pRsrc->netInfo.sn[3] = 0;
	pRsrc->netInfo.gw[0] = 192;
	pRsrc->netInfo.gw[1] = 168;
	pRsrc->netInfo.gw[2] = 1;
	pRsrc->netInfo.gw[3] = 1;
	pRsrc->netInfo.mac[0] = 0xaa;
	pRsrc->netInfo.mac[1] = 0xbb;
	pRsrc->netInfo.mac[2] = 0xcc;
	pRsrc->netInfo.mac[3] = 0xdd;
	pRsrc->netInfo.mac[4] = 0xee;
	pRsrc->netInfo.mac[5] = 0x12;
	wizchip_setnetinfo(&pRsrc->netInfo);	//update net info from chip
	
	//config retry, 200mS
	pRsrc->timeOut.time_100us = 2000;	//200ms
	pRsrc->timeOut.retry_cnt = 8;
	wizchip_settimeout(&pRsrc->timeOut);

	//interrupt config
//	void wizchip_setinterruptmask(intr_kind intr);
	
	//
//	socket(SOCK_TCPS, Sn_MR_TCP, 5000, 0x00);
	return DEV_SUCCESS;
}

/**********************************************************
 Public function
**********************************************************/
//static void MX_SPI1_Init(void)
//{

//  /* SPI1 parameter configuration*/
//  hspi1.Instance = SPI1;
//  hspi1.Init.Mode = SPI_MODE_MASTER;
//  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
//  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
//  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
//  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
//  hspi1.Init.NSS = SPI_NSS_SOFT;
//  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
//  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
//  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
//  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
//  hspi1.Init.CRCPolynomial = 10;
//  if (HAL_SPI_Init(&hspi1) != HAL_OK)
//  {
//    _Error_Handler(__FILE__, __LINE__);
//  }

//}
static void w5500x_takeSpiRsrc(void){
}

static void w5500x_freeSpiRsrc(void){
}

static void w5500x_spiEnable(void){
	HAL_GPIO_WritePin(pW5500Dev->rsrc.CS.GPIOx, pW5500Dev->rsrc.CS.GPIO_Pin, GPIO_PIN_RESET);
}
static void w5500x_spiDisable(void){
	HAL_GPIO_WritePin(pW5500Dev->rsrc.CS.GPIOx, pW5500Dev->rsrc.CS.GPIO_Pin, GPIO_PIN_SET);
}
static u8 w5500x_spi_readbyte(void){
	u8 ret = 0;
	while(HAL_SPI_GetState(pRsrc->SPI) != HAL_SPI_STATE_READY);	
	HAL_SPI_Receive_IT(pRsrc->SPI, &ret, 1);
	return ret;
}
static void 	w5500x_spi_writebyte(u8 wb){
	while(HAL_SPI_GetState(pRsrc->SPI) != HAL_SPI_STATE_READY);	
	HAL_SPI_Transmit(pRsrc->SPI, &wb, 1, 10);
}

static void 	w5500x_spi_readburst(u8* pBuf, u16 len){
	while(HAL_SPI_GetState(pRsrc->SPI) != HAL_SPI_STATE_READY);	
	HAL_SPI_Receive_IT(pRsrc->SPI, pBuf, len);
}

static void 	w5500x_spi_writeburst(u8* pBuf, u16 len){
	while(HAL_SPI_GetState(pRsrc->SPI) != HAL_SPI_STATE_READY);	
	HAL_SPI_Transmit_IT(pRsrc->SPI, pBuf, len);
}

static void setIp	(W5500_Rsrc_T* rsrc, u8 ip0, u8 ip1, u8 ip2, u8 ip3){
	rsrc->netInfo.ip[0] = ip0;
	rsrc->netInfo.ip[1] = ip1;
	rsrc->netInfo.ip[2] = ip2;
	rsrc->netInfo.ip[3] = ip3;
	wizchip_setnetinfo(&rsrc->netInfo);
}

static void setIpMask	(W5500_Rsrc_T* rsrc, u8 msk0, u8 msk1, u8 msk2, u8 msk3){
	rsrc->netInfo.sn[0] = msk0;
	rsrc->netInfo.sn[1] = msk1;
	rsrc->netInfo.sn[2] = msk2;
	rsrc->netInfo.sn[3] = msk3;
	wizchip_setnetinfo(&rsrc->netInfo);
}

static void setGateWay	(W5500_Rsrc_T* rsrc, u8 gw0, u8 gw1, u8 gw2, u8 gw3){
	rsrc->netInfo.gw[0] = gw0;
	rsrc->netInfo.gw[1] = gw1;
	rsrc->netInfo.gw[2] = gw2;
	rsrc->netInfo.gw[3] = gw3;
	wizchip_setnetinfo(&rsrc->netInfo);
}

static void setMac	(W5500_Rsrc_T* rsrc, u8 mac0, u8 mac1, u8 mac2, u8 mac3, u8 mac4, u8 mac5){
	rsrc->netInfo.mac[0] = mac0;
	rsrc->netInfo.mac[1] = mac1;
	rsrc->netInfo.mac[2] = mac2;
	rsrc->netInfo.mac[3] = mac3;
	rsrc->netInfo.mac[4] = mac4;
	rsrc->netInfo.mac[5] = mac5;
	wizchip_setnetinfo(&rsrc->netInfo);
}

static void getNetInfo	(W5500_Rsrc_T* rsrc){
	memset(&rsrc->netInfo,0,sizeof(wiz_NetInfo));
	wizchip_getnetinfo(&rsrc->netInfo);
}


/**********************************************************
 == THE END ==
**********************************************************/
