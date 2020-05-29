#include "config.h"
#include "bsp_systick.h"
#include "bsp_gpio.h"
#include "bsp_usart.h"
#include "device.h"

#include "socket.h"
#include "w5500.h"
#include "24c16.h"
#include <stdio.h> 
#include <string.h>

CONFIG_MSG  ConfigMsg, RecvMsg;
uint8 reboot_flag; //defined in main.c
uint8 BUFPUB[2048];
uint8 txsize[MAX_SOCK_NUM] = {2,2,2,2,2,2,2,2};
uint8 rxsize[MAX_SOCK_NUM] = {2,2,2,2,2,2,2,2};
uint8_t ip_define=0;
uint8_t ip_eeprom=0;

uint8 DEFAULT_MAC[6] = {0x00, 0x08, 0xdc, 0x11, 0x12, 0x13};
uint8 DEFAULT_LIP[4] = {192, 168, 1, 88};
uint8 DEFAULT_SN[4] = {255, 255, 255, 0};
uint8 DEFAULT_GW[4] = {192, 168, 1, 1};
uint8 DEFAULT_DNS[4] = {8,8,8,8};
uint8 DEFAULT_RIP[4]={192,168,1,100};
uint8 DEFAULT_flag[6]="WIZNET";
void Reset_W5500(void)
{
  GPIO_ResetBits(GPIOC, WIZ_RESET);
  delay_us(2);  
  GPIO_SetBits(GPIOC, WIZ_RESET);
  delay_ms(1600);
}
void reboot(void)
{
  pFunction Jump_To_Application;
  uint32 JumpAddress;
  JumpAddress = *(vu32*) (0x00000004);
  Jump_To_Application = (pFunction) JumpAddress;
  Jump_To_Application();
}

void set_default(void)
{
	memcpy(ConfigMsg.mac, DEFAULT_MAC, 6);
  memcpy(ConfigMsg.lip, DEFAULT_LIP, 4);
  memcpy(ConfigMsg.sub, DEFAULT_SN, 4);
  memcpy(ConfigMsg.gw,  DEFAULT_GW, 4);
  memcpy(ConfigMsg.dns, DEFAULT_DNS, 4);
	memcpy(ConfigMsg.crc_flag, DEFAULT_flag, 6);
	ConfigMsg.sw_ver[0]=FW_VER_HIGH;
	ConfigMsg.sw_ver[1]=FW_VER_LOW;
	write_config_to_eeprom();//write to flash
}

void set_network(void)
{
  uint8 ip[4];
	if(ip_define)
	{
		printf("ip from defination!\r\n");
		set_default();
	}
	if(ip_eeprom)
		printf("ip from eeprom!\r\n");
	
  setSHAR(ConfigMsg.mac);
  setSUBR(ConfigMsg.sub);
  setGAR(ConfigMsg.gw);
  setSIPR(ConfigMsg.lip);
  //Init. TX & RX Memory size of w5200
  sysinit(txsize, rxsize); 
  setRTR(2000);//200ms
  setRCR(3);
  getSIPR (ip);
  printf("IP : %d.%d.%d.%d\r\n", ip[0],ip[1],ip[2],ip[3]);
  getSUBR(ip);
  printf("SN : %d.%d.%d.%d\r\n", ip[0],ip[1],ip[2],ip[3]);
  getGAR(ip);
  printf("GW : %d.%d.%d.%d\r\n", ip[0],ip[1],ip[2],ip[3]);
}
void read_config_from_eeprom(void)
{
	uint16_t i;
	for(i=0;i<CONFIG_MSG_LEN;i++)
	{
		*(uint8_t *)(ConfigMsg.mac+i)=at24c16_read(i);
		//printf("%d-",*(uint8_t *)(ConfigMsg.mac+i));
	}
}

void write_config_to_eeprom(void)
{
	uint16_t i;
	for(i=0;i<CONFIG_MSG_LEN;i++)
	{
		at24c16_write(i,*(ConfigMsg.mac+i));
	}
}
void write_temp_to_eeprom(void)
{
	uint16_t i;
	for(i=0;i<CONFIG_MSG_LEN-6;i++)
	{
		at24c16_write(i,*(ConfigMsg.mac+i));
	}
}

void do_udp_config(void)
{
  uint8 sckState;
  uint16 len;
  uint8 rIP[4];
  //0104
  uint8 broadcastIp[4]={255,255,255,255};
  uint16 rPort;
  //clear RecvMsg
  uint16 msg_len=sizeof(RecvMsg);
  memset(RecvMsg.op, 0, msg_len);
  
  sckState=getSn_SR(SOCK_CONFIG);
  switch(sckState)
  {
  case SOCK_UDP:

    if((len=getSn_RX_RSR(SOCK_CONFIG))>0)
    {
      len=recvfrom(SOCK_CONFIG, (uint8 *)&BUFPUB, len, rIP, &rPort);
      memcpy((uint8 *)&RecvMsg,(uint8 *)&BUFPUB,sizeof(RecvMsg));
      //FIND: searching, SETT: setting, FACT: factory reset, FIRS: firmware uploading
      if((RecvMsg.op[0]=='F') && (RecvMsg.op[1]=='I') && (RecvMsg.op[2]=='N') && (RecvMsg.op[3]=='D'))
      {
        RecvMsg = ConfigMsg;
        RecvMsg.op[0]='F';RecvMsg.op[1]='I';RecvMsg.op[2]='N';RecvMsg.op[3]='D';
        sendto(SOCK_CONFIG, (const uint8 *)RecvMsg.op, sizeof(RecvMsg), broadcastIp, rPort);
        if(ConfigMsg.debug) printf("FIND from %d.%d.%d.%d\r\n", rIP[0],rIP[1],rIP[2],rIP[3]);
      }
      else if((RecvMsg.op[0]=='S') && (RecvMsg.op[1]=='E') && (RecvMsg.op[2]=='T') && (RecvMsg.op[3]=='T'))
      {
        if((RecvMsg.mac[0]==ConfigMsg.mac[0]) && (RecvMsg.mac[1]==ConfigMsg.mac[1]) && (RecvMsg.mac[2]==ConfigMsg.mac[2]) && 
           (RecvMsg.mac[3]==ConfigMsg.mac[3]) && (RecvMsg.mac[4]==ConfigMsg.mac[4]) && (RecvMsg.mac[5]==ConfigMsg.mac[5]))
        {
          ConfigMsg = RecvMsg;//update config msg
          ConfigMsg.sw_ver[1] = FW_VER_LOW;
          ConfigMsg.sw_ver[0] = FW_VER_HIGH;
          sendto(SOCK_CONFIG, (uint8 *)&ConfigMsg, sizeof(ConfigMsg), broadcastIp, rPort);         
          write_temp_to_eeprom();//write to flash
          reboot_flag=1;
        }
      }
      else if((RecvMsg.op[0]=='F') && (RecvMsg.op[1]=='I') && (RecvMsg.op[2]=='R') && (RecvMsg.op[3]=='M'))
      {
        if((RecvMsg.mac[0]==ConfigMsg.mac[0]) && (RecvMsg.mac[1]==ConfigMsg.mac[1]) && (RecvMsg.mac[2]==ConfigMsg.mac[2]) && 
           (RecvMsg.mac[3]==ConfigMsg.mac[3]) && (RecvMsg.mac[4]==ConfigMsg.mac[4]) && (RecvMsg.mac[5]==ConfigMsg.mac[5]))
        {
         // ConfigMsg.state=CONFIGTOOL_FW_UP;
          write_config_to_eeprom();
          //response, send FIRM to config tool.
          sendto(SOCK_CONFIG, (uint8 *)&RecvMsg, sizeof(ConfigMsg), broadcastIp, rPort);
          reboot_flag=1;
        }
      }
    }
    break;
  case SOCK_CLOSED:
    socket(SOCK_CONFIG, Sn_MR_UDP, REMOTE_CLIENT_PORT, 0);
    break;
  }
}
