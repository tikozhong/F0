
#include "config.h"
#include "w5500/socket.h"
#include "util.h"
#include "w5500/w5500.h"
#include "device.h"
#include "at24c16.h"

#include <stdio.h> 
#include <string.h>

extern uint8*  SRC_MAC_ADDR     ;    // Local MAC address
extern uint8*   GET_SN_MASK    ;     // Subnet mask received from the DHCP server
extern uint8*   GET_GW_IP      ;     // Gateway ip address received from the DHCP server
extern uint8*   GET_DNS_IP     ;    // DNS server ip address received from the DHCP server
extern uint8*   DHCP_HOST_NAME;   // HOST NAME
extern uint8*   GET_SIP       ; 
CONFIG_MSG_DHCP  ConfigMsg, RecvMsg;

extern CONFIG_MSG Config_Msg;
extern CHCONFIG_TYPE_DEF Chconfig_Type_Def;
uint8 txsize[MAX_SOCK_NUM] = {2,2,2,2,2,2,2,2};
uint8 rxsize[MAX_SOCK_NUM] = {2,2,2,2,2,2,2,2};
extern uint8 MAC[6];//public buffer for DHCP, DNS, HTTP
uint8 pub_buf[1460];
extern uint8 Enable_DHCP;
extern uint8 Dest_IP[4] ;
extern uint16 Dest_PORT ;
uint8 Dest_IP[4] = {192, 168, 1, 99}; //DST_IP Address
uint16 Dest_PORT = 5000; //DST_IP port

void Reset_W5500(void)
{
  GPIO_ResetBits(GPIOB, WIZ_RESET);
  Delay_us(500);  
  GPIO_SetBits(GPIOB, WIZ_RESET);
  Delay_ms(1600);
}
//reboot 
void reboot(void)
{
  pFunction Jump_To_Application;
  uint32 JumpAddress;
  JumpAddress = *(vu32*) (0x00000004);
  Jump_To_Application = (pFunction) JumpAddress;
  Jump_To_Application();
}
void USART1_Init(void)
{
  USART_InitTypeDef USART_InitStructure;

  /* USARTx configuration ------------------------------------------------------*/
  /* USARTx configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //Enable rx enable, 
  
  /* Configure the USARTx */ 
  USART_Init(USART1, &USART_InitStructure);
  /* Enable the USARTx */
  USART_Cmd(USART1, ENABLE);
}

 
int fputc(int ch, FILE *f)
{
/* 将Printf内容发往串口 */
  USART_SendData(USART1, (unsigned char) ch);
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); 
  return (ch);
}

void USART1_Putc(unsigned char c)
{
    USART_SendData(USART1, c);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET );
}

void USART1_Puts(char * str)
{
    while(*str)
    {
        USART_SendData(USART1, *str++);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    }
}
void USART1_IRQHandler(void)
{
	uint16 str1;    
  	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
  	{
     USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
     str1=USART_ReceiveData(USART1);

     USART_SendData(USART1, str1);
     while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); 
     USART_ITConfig( USART1,USART_IT_RXNE, ENABLE);
  	}
}

void Set_network(void)
{
    uint8 tmp_array[6];
    uint8 i;
    uint8 mac[6]={0x00,0x08,0xdc,0x11,0x11,0x15};

    // MAC ADDRESS
     for (i = 0 ; i < 6; i++) ConfigMsg.mac[i] = mac[i];
    // Local IP ADDRESS
     for (i = 0 ; i < 4; i++) ConfigMsg.lip[i] = GET_SIP[i];
    // GateWay ADDRESS
     for (i = 0 ; i < 4; i++) ConfigMsg.gw[i] = GET_GW_IP[i];
    // Subnet Mask ADDRESS
     for (i = 0 ; i < 4; i++) ConfigMsg.sub[i] = GET_SN_MASK[i];
   
    setSHAR(ConfigMsg.mac);
    setSUBR(ConfigMsg.sub);
    setGAR(ConfigMsg.sub);
    setSIPR(ConfigMsg.lip);

    sysinit(txsize, rxsize);

    getSHAR(tmp_array);
   // printf("\r\nMAC : %.2X.%.2X.%.2X.%.2X.%.2X.%.2X", tmp_array[0],tmp_array[1],tmp_array[2],tmp_array[3],tmp_array[4],tmp_array[5]);

    getSIPR (tmp_array);
   // printf("\r\nIP : %d.%d.%d.%d", tmp_array[0],tmp_array[1],tmp_array[2],tmp_array[3]);

    getSUBR(tmp_array);
   // printf("\r\nSN : %d.%d.%d.%d", tmp_array[0],tmp_array[1],tmp_array[2],tmp_array[3]);

    getGAR(tmp_array);
   // printf("\r\nGW : %d.%d.%d.%d", tmp_array[0],tmp_array[1],tmp_array[2],tmp_array[3]);
}


void write_config_to_eeprom(void)
{
  uint8 data;
  uint16 i,j;
  uint16 dAddr=0;
  for (i = 0, j = 0; i < (uint8)(sizeof(ConfigMsg)-4);i++) 
  {
    data = *(uint8 *)(ConfigMsg.mac+j);
    at24c16_write(dAddr, data);
    dAddr += 1;
    j +=1;
  }
}
void set_default(void)
{
  
  uint8 mac[6]={0x00,0x08,0xdc,0x11,0x11,0x15};
  uint8 lip[4]={192,168,1,111};
  uint8 sub[4]={255,255,255,0};
  uint8 gw[4]={192,168,1,1};
  uint8 dns[4]={8,8,8,8};
  memcpy(ConfigMsg.lip, lip, 4);
  memcpy(ConfigMsg.sub, sub, 4);
  memcpy(ConfigMsg.gw,  gw, 4);
  memcpy(ConfigMsg.mac, mac,6);
  memcpy(ConfigMsg.dns,dns,4);
  /*
  uint8 dhcp;
  uint8 debug;

  uint16 fw_len;
  uint8 state;
  */
  ConfigMsg.dhcp=0;
  ConfigMsg.debug=1;
  ConfigMsg.fw_len=0;
  
  ConfigMsg.state=NORMAL_STATE;
  ConfigMsg.sw_ver[0]=FW_VER_HIGH;
  ConfigMsg.sw_ver[1]=FW_VER_LOW;
  
}
void get_config(void)
{
  uint16 i; 
  uint8 tmp=0;
  for (i =0; i < CONFIG_MSG_LEN; i++) 
  {
    tmp=at24c16_read(i);
    *(ConfigMsg.mac+i) = tmp;
  }
  if((ConfigMsg.mac[0]==0xff)&&(ConfigMsg.mac[1]==0xff)&&(ConfigMsg.mac[2]==0xff)&&(ConfigMsg.mac[3]==0xff)&&(ConfigMsg.mac[4]==0xff)&&(ConfigMsg.mac[5]==0xff))
  {
    set_default();
  }
}
