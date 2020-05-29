/******************** (C) COPYRIGHT 2015 INCUBECN *****************************
* File Name          : response.h
* Author             : Tiko Zhong
* Date First Issued  : 20170221
* Description        : This file provides a set of functions needed to manage the
*                      communication using HAL_UARTxxx
********************************************************************************
* History:
* 12/01/2015: V0.0
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _RESPONSE_X_H
#define _RESPONSE_X_H

/* Includes ------------------------------------------------------------------*/
#include "misc.h"
#include "packet.h"

extern const char OK[];
extern const char ERR[];
extern const char MSG[];

//extern const char UNK[];
/*
%d    signed int, in decimal
%u    unsigned int, in decimal
%f    float, in decimal
%s    string
%x    signed/unsigned int, in hex
%%    '%'

%md    signed int, in decimal; fill '0' if no enough
%mu    unsigned int, in decimal
%m.nf  float, in decimal
%ms    string
%mx    signed/unsigned int, in hex
*/

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
s8 RESPONSE_X(const char* PREFIX, char* rtnStr, u8 rtnStrSz, const char* DEVNAME, const char* FUNC_NAME, const char* FORMAT_ORG, ...);
s8 RESPONSE(PAKET_T *packet, const char* PREFIX, const char* DEVNAME, const char* FUNC_NAME, const char* FORMAT, ...);
s8 RESPONSE_ORG(PAKET_T *packet, const char* PREFIX, const PAKET_T *PACKET);

#endif /* _CMD_H */

/******************* (C) COPYRIGHT 2015 INCUBECN *****END OF FILE****/
