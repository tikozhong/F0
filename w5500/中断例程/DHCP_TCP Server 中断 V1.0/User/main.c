#include "mcu_init.h"
#include "config.h"
#include "device.h"
#include "spi2.h"
#include "socket.h"
#include "w5500.h"
#include "at24c16.h"
#include "util.h"
#include "dhcp.h"
#include "loopback.h"


#include <stdio.h>

extern uint8 txsize[];
extern uint8 rxsize[];

uint8 buffer[2048];/*����һ��2KB�Ļ���*/
CONFIG_MSG Config_Msg;
CHCONFIG_TYPE_DEF Chconfig_Type_Def;

uint8 TX_BUF[TX_RX_MAX_BUF_SIZE]; // TX Buffer for applications

uint8 I_STATUS[MAX_SOCK_NUM];
uint8 ch_status[MAX_SOCK_NUM] = { 0, }; /** 0:close, 1:ready, 2:connected */

/* Private variables ---------------------------------------------------------*/
// Configuration Network Information of W5500
uint8 Enable_DHCP = OFF;
uint8 MAC[6] = {0x00, 0x08, 0xDC, 0x47, 0x47, 0x54};//MAC Address
extern void Set_network(void);
uint8 C_Flag = 0;
extern uint8 dhcp_state;
int main()
{
  uint8 dhcpret=0;
  uint8 tmp_array[4] = {0};
  RCC_Configuration(); /* ���õ�Ƭ��ϵͳʱ��*/
  GPIO_Configuration();/* ����GPIO*/
  NVIC_Configuration();/* ����Ƕ���ж�����*/
  
  Systick_Init(72);/* ��ʼ��Systick����ʱ��*/
  USART1_Init(); /*��ʼ������ͨ��:115200@8-n-1*/
  at24c16_init();/*��ʼ��eeprom*/
  printf("W5500 EVB initialization over.\r\n");
  
  Reset_W5500();/*Ӳ����W5500*/
  WIZ_SPI_Init();/*��ʼ��SPI�ӿ�*/
  printf("W5500 initialized!\r\n");  
  
  //set_default(); 

  init_dhcp_client();
  setRTR(2000);
  setRCR(5);
  IINCHIP_WRITE(Sn_MR(7), 0x20);//TCPģʽ�¹ر�����ʱACK
  IINCHIP_WRITE(Sn_IMR(7), 0x0F);

  IINCHIP_WRITE(IMR, 0xF0);
  IINCHIP_WRITE(SIMR, 0xFE); 
  while(1)
  {
    dhcpret = check_DHCP_state(SOCK_DHCP);
    switch(dhcpret)
    {
      case DHCP_RET_NONE:
        break;
      case DHCP_RET_TIMEOUT:
        break;
      case DHCP_RET_UPDATE:
	  	Set_network();
	    printf("DHCP OK!\r\n");  
		C_Flag = 1;
        break;
      case DHCP_RET_CONFLICT:
	    C_Flag = 0;
		printf("DHCP Fail!\r\n");
		dhcp_state = STATE_DHCP_READY;
		break; 
      default:
        break;
    }

	if(C_Flag == 1)
	{
		getSIPR(tmp_array);
		loopback_tcps(7, 8080);
		loopback_tcps(6, 8060);
	}
  }
}
