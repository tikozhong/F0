#include "usart.h"
#include "device.h"
#include "spi2.h"
#include "ult.h"
#include "socket.h"
#include "w5500.h"
#include "24c16.h"
#include "md5.h"
#include "string.h"

extern uint8 txsize[];										// 引用外部变量，声明Socket发送缓存大小
extern uint8 rxsize[];										// 引用外部变量，声明Socket接收缓存大小

uint8 buffer[2048];												// 定义一个2KB的数组，用来存放Socket的通信数据

int main(void)
{
  uint8 server_ip[4]={192,168,1,250};			// 配置远程服务器IP地址
	uint16 server_port1=5000;								// 配置远程服务器端口
	uint16 server_port2=6000;								// 配置远程服务器端口
	
  uint16 local_port=6000;									// 初始化一个本地端口
  uint16 len=0;
	
  /***** MCU时钟初始化 *****/							  
	Systick_Init(72);	
	
	/***** 中断控制器配置 *****/
	NVIC_Configuration();
	
	/***** GPIO、SPI初始化 *****/
	GPIO_Configuration();			
	WIZ_SPI_Init();
	
	/***** 串口初始化 *****/
	USART1_Init(); 		
	
	/***** 初始化eeprom *****/
  at24c16_init();
	
	/***** 硬重启W5500 *****/
	Reset_W5500();
	
	/***** W5500的IP信息初始化 *****/
	set_default(); 														// 设置默认MAC、IP、GW、SUB、DNS
	set_network();														// 配置初始化IP信息并打印，初始化8个Socket
	
	/***** 打开W5500的Keepalive功能 *****/
	setkeepalive(0);													//开启心跳检测
	
	printf("TCP Server IP: %d.%d.%d.%d \r\n",server_ip[0],server_ip[1],server_ip[2],server_ip[3]);
	printf("W5500 Init Complete!\r\n");
  printf("Start TCP Client Test!\r\n"); 
  
	/*Socket状态机，MCU通过读Sn_SR(0)的值进行判断Socket应该处于何种状态
		Sn_SR状态描述：
		0x00		SOCK_CLOSED
		0x13		SOCK_INIT
		0x14		SOCK_LISTEN
		0x17		SOCK_ESTABLISHED
		0x1C		SOCK_CLOSE_WAIT
		0x22		SOCK_UDP
	*/
	while(1)																				// Socket状态机
	{	
		int i;
		for(i=0;i<=1;i++)
		{
			switch(getSn_SR(i))														// 获取socket0的状态
			{
				case SOCK_INIT:															// Socket处于初始化完成(打开)状态
					
						printf("Socket%d-1 ",i);
						Delay_ms(500);
				
				if(i==0)
				{
						connect(i, server_ip,server_port1);			// 配置Sn_CR为CONNECT，并向TCP服务器发出连接请求
				}
						
				else
				{
						connect(i, server_ip,server_port2);			// 配置Sn_CR为CONNECT，并向TCP服务器发出连接请求
				}
				break;
				case SOCK_ESTABLISHED:											// Socket处于连接建立状态
					
						printf("Socket%d-2 ",i);
						Delay_ms(500);
				
						if(getSn_IR(i) & Sn_IR_CON)   					
						{
							setSn_IR(i, Sn_IR_CON);								// Sn_IR的CON位置1，通知W5500连接已建立
						}
						// 数据回环测试程序：数据从上位机服务器发给W5500，W5500接收到数据后再回给服务器
						len=getSn_RX_RSR(i);										// len=Socket0接收缓存中已接收和保存的数据大小
						if(len>0)
						{
							recv(i,buffer,len);										// W5500接收来自服务器的数据，并通过SPI发送给MCU
							printf("Socket%d:%s\r\n",i,buffer);							// 串口打印接收到的数据
//							send(i,buffer,len);										// 接收到数据后再回给服务器，完成数据回环
						}
						// W5500从串口发数据给客户端程序，数据需以回车结束
						if(USART_RX_STA & 0x8000)								// 判断串口数据是否接收完成
						{					   
							len=USART_RX_STA & 0x3fff;						// 获取串口接收到数据的长度
							send(i,USART_RX_BUF,len);							// W5500向客户端发送数据
							USART_RX_STA=0;												// 串口接收状态标志位清0
							memset(USART_RX_BUF,0,len+1);					// 串口接收缓存清0
						}
				break;
				case SOCK_CLOSE_WAIT:												// Socket处于等待关闭状态
					
						printf("Socket%d-3 ",i);
						Delay_ms(500);
				
						len=getSn_RX_RSR(i);										// len=Socket0接收缓存中已接收和保存的数据大小
						if(len>0)
						{
							recv(i,buffer,len);										// W5500接收来自服务器的数据，并通过SPI发送给MCU
							printf("%s\r\n",buffer);							// 串口打印接收到的数据
							send(i,buffer,len);										// 接收到数据后再回给服务器，完成数据回环
						}
						disconnect(i);																// 断开Socket0的连接
				break;
				case SOCK_CLOSED:														// Socket处于关闭状态
					
						printf("Socket%d-4 ",i);
						Delay_ms(500);
				
						socket(i,Sn_MR_TCP,local_port++,Sn_MR_ND);		// 打开Socket0，并配置为TCP无延时模式，打开一个本地端口
				break;
			}
		}
	}
}


