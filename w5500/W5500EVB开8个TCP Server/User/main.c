#include "usart.h"
#include "device.h"
#include "spi2.h"
#include "ult.h"
#include "socket.h"
#include "w5500.h"
#include "24c16.h"
#include "md5.h"
#include "string.h"

extern uint8 txsize[];										// �����ⲿ����������Socket���ͻ����С
extern uint8 rxsize[];										// �����ⲿ����������Socket���ջ����С

uint8 buffer[2048];												// ����һ��2KB�����飬�������Socket��ͨ������

int main(void)
{
  uint16 local_port=5000;												// ����һ�����ض˿ڲ���ʼ��
  uint16 len=0; 													

	/***** MCUʱ�ӳ�ʼ�� *****/							  
	Systick_Init(72);	
	
	/***** �жϿ��������� *****/
	NVIC_Configuration();
	
	/***** GPIO��SPI��ʼ�� *****/
	GPIO_Configuration();			
	WIZ_SPI_Init();
	
	/***** ���ڳ�ʼ�� *****/
	USART1_Init(); 		
	
	/***** ��ʼ��eeprom *****/
  at24c16_init();
	
	/***** Ӳ����W5500 *****/
	Reset_W5500();
	
	/***** W5500��IP��Ϣ��ʼ�� *****/
	set_default(); 														// ����Ĭ��MAC��IP��GW��SUB��DNS
	set_network();														// ���ó�ʼ��IP��Ϣ����ӡ����ʼ��8��Socket
	
	/***** ��W5500��Keepalive���� *****/
	setkeepalive(0);
	
	printf("TCP Server Port:%d\r\n",local_port);
	printf("W5500 Init Complete!\r\n");
  printf("Start TCP Server Test!\r\n"); 

	/*Socket״̬����MCUͨ����Sn_SR(0)��ֵ�����ж�SocketӦ�ô��ں���״̬
		Sn_SR״̬������
		0x00		SOCK_CLOSED
		0x13		SOCK_INIT
		0x14		SOCK_LISTEN
		0x17		SOCK_ESTABLISHED
		0x1C		SOCK_CLOSE_WAIT
		0x22		SOCK_UDP
	*/
	while(1)																	// Socket״̬��
	{
		int i;
		for(i=0;i<=1;i++)
		{
			switch(getSn_SR(i))										// ��ȡsocketi��״̬
			{
				case SOCK_INIT:											// Socket���ڳ�ʼ�����(��)״̬
					printf("%d-1 ",i);
				Delay_ms(500);
						listen(i);											// �����ոմ򿪵ı��ض˿ڣ��ȴ��ͻ�������
				break;
				case SOCK_ESTABLISHED:							// Socket�������ӽ���״̬			
											printf("%d-2 ",i);
				Delay_ms(500);
//					printf("%d Connected\r\n",i);
					Delay_ms(1000);
					if(getSn_IR(i) & Sn_IR_CON)			
						{
							setSn_IR(i, Sn_IR_CON);				// Sn_IR��CONλ��1��֪ͨW5500�����ѽ���
							
						}
						// ���ݻػ����Գ������ݴ���λ���ͻ��˷���W5500��W5500���յ����ݺ��ٻظ��ͻ���
						len=getSn_RX_RSR(i);						// ��ȡW5500���н��ջ���Ĵ�����ֵ������len��Sn_RX_RSR��ʾ���ջ������ѽ��պͱ�������ݴ�С
						if(len>0)
						{
							recv(i,buffer,len);						// W5500�������Կͻ��˵����ݣ���ͨ��SPI���͸�MCU
							printf("%d:%s\r\n",i,buffer);	// ���ڴ�ӡ���յ�������
							send(i,buffer,len);						// ���յ����ݺ��ٻظ��ͻ��ˣ�������ݻػ�
						}
						// W5500�Ӵ��ڷ����ݸ��ͻ��˳����������Իس�����
						if(USART_RX_STA & 0x8000)				// �жϴ��������Ƿ�������
						{					   
							len=USART_RX_STA & 0x3fff;		// ��ȡ���ڽ��յ����ݵĳ���
							send(i,USART_RX_BUF,len);			// W5500��ͻ��˷�������
							USART_RX_STA=0;								// ���ڽ���״̬��־λ��0
							memset(USART_RX_BUF,0,len+1);	// ���ڽ��ջ�����0
						}
				break;
				case SOCK_CLOSE_WAIT:								// Socket���ڵȴ��ر�״̬
										printf("%d-3 ",i);
				Delay_ms(500);
						close(i);												// �ر�Socketi
				break;
				case SOCK_CLOSED:										// Socket���ڹر�״̬
										printf("%d-4 ",i);
				Delay_ms(500);
						socket(i,Sn_MR_TCP,local_port++,Sn_MR_ND);		// ��Socketi��������ΪTCP����ʱģʽ����һ�����ض˿�
				break;
			}
		}
	}
}

