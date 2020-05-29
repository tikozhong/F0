/* Includes ------------------------------------------------------------------*/
#include "config.h"
#include "bsp_systick.h"
#include "bsp_gpio.h"
#include "bsp_usart.h"
#include "bsp_spi2.h"

#include "device.h"
#include "utility.h"
#include "socket.h"
#include "w5500.h"
#include "24c16.h"
#include <stdio.h>

int main(void)
{

	Systick_Init(72);/* 初始化Systick工作时钟*/
	USART1_Configuration();
  GPIO_Configuration();
	at24c16_init();
	Reset_W5500();
	SPI2_Configuration();
//	erase_eeprom(0, 255);
	read_config_from_eeprom();
	if(ConfigMsg.crc_flag[0]!='W'||ConfigMsg.crc_flag[1]!='I'||ConfigMsg.crc_flag[2]!='Z'
		||ConfigMsg.crc_flag[3]!='N'||ConfigMsg.crc_flag[4]!='E'||ConfigMsg.crc_flag[5]!='T')
	{
		erase_eeprom(0,255);
		ip_define=1;
		set_network();
	}
	else
	{
		ip_eeprom=1;
		set_network();
	}
  printf("Network is ready.\r\n");	
//	set_default();
//	set_network();
  while (1)
  {
		do_udp_config();
		if(reboot_flag) reboot();
  }
}

