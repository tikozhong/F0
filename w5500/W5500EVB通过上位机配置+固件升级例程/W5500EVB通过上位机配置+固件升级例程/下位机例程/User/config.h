#ifndef _CONFIG_H_
#define _CONFIG_H_

#include "stm32f10x.h"
#include "Types.h"
#include "device.h"

#define __GNUC__
#define FW_VER_HIGH   1
#define FW_VER_LOW    0

#define SOCK_HTTP 		0
#define SOCK_CONFIG		1
#define REMOTE_CLIENT_PORT	1460	/* from client to server */
#pragma pack(1)
typedef struct _CONFIG_MSG
{
	uint8 op[4];
	uint8 mac[6];
	uint8 sw_ver[2];
	uint8 lip[4];
  uint8 sub[4];
  uint8 gw[4];
  uint8 dns[4];
	uint8 dhcp;
	uint8 debug;
	uint8 crc_flag[6];
}CONFIG_MSG;
#pragma pack()
extern CONFIG_MSG  ConfigMsg, RecvMsg;
#define CONFIG_MSG_LEN      sizeof(CONFIG_MSG)-4 // the 4 bytes OP will not save to EEPROM

//#define 
#define ON											1
#define OFF											0
#define HIGH										1
#define LOW											0

#define TX_RX_MAX_BUF_SIZE	2048

#endif


