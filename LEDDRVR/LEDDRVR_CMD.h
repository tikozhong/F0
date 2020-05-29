/**********************************************************
filename: leddrvr_cmd.h
**********************************************************/
#ifndef _LEDDRVR_CMD_H_
#define _LEDDRVR_CMD_H_

#include "publicRsrc.h"
#include "leddrvr.h"
/*****************************************************************************
 @ typedefs
****************************************************************************/
extern const u8 LEDDRVR_HELP[];

s8 ledDrvrCmd(LedDrvrDev_t *dev, PAKET_T* packetIn, PAKET_T* packetRtn);

#endif
