/**********************************************************
filename: mailOp.h
**********************************************************/
#ifndef _MAIL_OP_H_
#define _MAIL_OP_H_
#include "misc.h"
#include "packet.h"

#define SUB_PRNT_ONLY 0XFF
#define SUB_FRIEN_ADD	0XFE
#define SUB_FILE_START	0XFD
/****************************************************************************
 @ enums
****************************************************************************/

/*****************************************************************************
 @ typedefs
****************************************************************************/
typedef struct{
	osMessageQId productor;
	PAKET_T Content;
	u32 traceIndex;		//also can be sub
} Mail_T;

osStatus newSend(osMailQId sndr, osMailQId rcvr, u32 sub, const char* FORMAT, ...);	//APPLY NEW PACKET FUNCTION
Mail_T* newMail(osMailQId sndr, osMailQId rcvr, u32 sub);

#endif
