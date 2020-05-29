#include "stm32f10x.h"
#include "config.h"
#include "w5500.h"
#include "socket.h"
#include "util.h"
#include "loopback.h"
#include <stdio.h>
#include <string.h>
#include "User_App.h"
#include "Types.h"


#define tick_second 1

extern uint8 ch_status[MAX_SOCK_NUM];
extern CHCONFIG_TYPE_DEF Chconfig_Type_Def;
extern uint32_t presentTime;
uint16 any_port = 1000;
extern uint8 I_STATUS[MAX_SOCK_NUM];
extern uint16 SSIZE[MAX_SOCK_NUM]; /**< Max Tx buffer size by each channel */
extern uint16 RSIZE[MAX_SOCK_NUM]; /**< Max Rx buffer size by each channel */

extern void SOCK_DISCON(SOCKET s);

void loopback_tcps(SOCKET s, uint16 port)
{	
	uint16 len = 0;//, cnt = 0;
	uint8 * data_buf = (uint8*) TX_BUF;
	uint8 tmp = 0;

		if(I_STATUS[s] == SOCK_CLOSED)
		{// cloes
			if(!ch_status[s]) 
			{
				ch_status[s] = 1;
				if(socket(s,Sn_MR_TCP,port,0x00) == 0)	  /* reinitialize the socket */
				{
					ch_status[s] = 0;
				}
				else	
				{
					listen(s);
				}
			}
		}  
	 
		if(I_STATUS[s]&Sn_IR_CON)
		{// connected
			ch_status[s] = 2;
			I_STATUS[s] &= ~(0x01);
			//break;
		}
		if(I_STATUS[s]&Sn_IR_DISCON)
		{// discon
			if ((len = getSn_RX_RSR(s)) > 0)		/* check Rx data */
			{
				if (len > TX_RX_MAX_BUF_SIZE) 
				len = TX_RX_MAX_BUF_SIZE; /* if Rx data size is lager than TX_RX_MAX_BUF_SIZE */
			    recv(s, data_buf, len);		/* read the received data */
						data_buf[len]=0x00;
				printf("%s\r\n",data_buf);
			}
			SOCK_DISCON(s);
			//break;
		}
		if(I_STATUS[s]&Sn_IR_RECV)
		{
			IINCHIP_WRITE(IMR, 0x00);//开中断屏蔽IP,UNREACH,PPPoE,Magic Packet
			IINCHIP_WRITE(SIMR, 0x00);//开socket中断屏蔽  
			IINCHIP_ISR_DISABLE();
			tmp = I_STATUS[s];
			I_STATUS[s] &= ~(0x04); // RECV
			IINCHIP_ISR_ENABLE();
 
			IINCHIP_WRITE(IMR, 0xF0);
			IINCHIP_WRITE(SIMR, 0xFF);
		// recv
			if (tmp & Sn_IR_RECV)
			{
				if((len = getSn_RX_RSR(s)) > 0)		/* check Rx data */
				{
					if (len > TX_RX_MAX_BUF_SIZE) 
					len = TX_RX_MAX_BUF_SIZE;	/* if Rx data size is lager than TX_RX_MAX_BUF_SIZE */
					len = recv(s, data_buf, len); /* read the received data */
					data_buf[len]=0;  //防止多打印
					printf("%s\r\n",data_buf);
					send(s,data_buf,len);
					tmp = I_STATUS[s];
				}
			 }
			//break;
		}
		if(I_STATUS[s]&Sn_IR_SEND_OK)
		{
			// send ok
			I_STATUS[s] &= ~(0x10);
			//break;
		}

#if (__MCU_TYPE__ == __MCU_STM32F103__)
		/**/
		if(GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6) == 0x00) EXTI_GenerateSWInterrupt(EXTI_Line6);

#endif


}


void loopback_tcpc(SOCKET s, uint16 port)
{
	uint16 len; 						
	uint8 * data_buf = (u8*) TX_BUF;
	uint8	tmp = 0;
	switch (I_STATUS[s])
	{
		case SOCK_CLOSED://无中断
			// cloes
			if(!ch_status[s])
			{
				printf("\r\n%d : Loop-Back TCP Client Started. port: %d", s, port);
				ch_status[s] = 1;
			}
			if(socket(s, Sn_MR_TCP, port, 0x00) == 0)    /* reinitialize the socket */
			{
				printf("\a%d : Fail to create socket.",s);
				ch_status[s] = 0;
			}
			else	
				connect(s, Chconfig_Type_Def.destip, Chconfig_Type_Def.port);
			break;
		case Sn_IR_CON: 
			// connected
			ch_status[s] = 2;
			I_STATUS[s] &= ~(0x01);
			break;
		case Sn_IR_DISCON: 
			// discon
			if ((len = getSn_RX_RSR(s)) > 0)		/* check Rx data */
			{
				if (len > TX_RX_MAX_BUF_SIZE) len = TX_RX_MAX_BUF_SIZE; /* if Rx data size is lager than TX_RX_MAX_BUF_SIZE */
				len = recv(s, data_buf, len);		/* read the received data */
			}
			SOCK_DISCON(s);
			break;
		case Sn_IR_RECV: 
			IINCHIP_WRITE(IMR, 0x00);
			IINCHIP_WRITE(SIMR, 0x00);
			IINCHIP_ISR_DISABLE();
			tmp = I_STATUS[s];
			I_STATUS[s] &= ~(0x04); // RECV
			IINCHIP_ISR_ENABLE();
			IINCHIP_WRITE(IR, 0xF0);
			IINCHIP_WRITE(SIR, 0xFF);
			// recv
			if (tmp & Sn_IR_RECV)
			{
				if((len = getSn_RX_RSR(s)) > 0)		/* check Rx data */
				{
					if (len > TX_RX_MAX_BUF_SIZE) 
						len = TX_RX_MAX_BUF_SIZE;	/* if Rx data size is lager than TX_RX_MAX_BUF_SIZE */
					len = recv(s, data_buf, len); /* read the received data */
					printf("recv_data_len : %d, getSn_RX_RSR(%d) : %d\r\n", len, s, getSn_RX_RSR(s));	
				}
			}
			break;
		case Sn_IR_SEND_OK: 
			// send ok
			I_STATUS[s] &= ~(0x10);
			break;
	}
}


void loopback_udp(SOCKET s, uint16 port)
{
	uint16 len;
	uint8 * data_buf = (u8*) TX_BUF;
	uint32_t destip = 0;
	uint16 destport;
    uint8 addr[4] = {192, 168, 11, 237},tmp;
	
	switch (I_STATUS[s])
	{
		case SOCK_CLOSED:                                               /* if a socket is closed */
			printf("\r\n%d : Loop-Back UDP Started. port :%d", s, port);
			if(socket(s,Sn_MR_UDP,port,0x00)== 0)    /* reinitialize the socket */
				printf("\a%d : Fail to create socket.",s);
			break;
		case Sn_IR_SEND_OK: 
			// send ok
			I_STATUS[s] &= ~(0x10);
			break;
		case Sn_IR_RECV: 
			IINCHIP_WRITE(IMR, 0x00);
			IINCHIP_WRITE(SIMR, 0x00);
			IINCHIP_ISR_DISABLE();
			tmp = I_STATUS[s];
			I_STATUS[s] &= ~(0x04); // RECV
			IINCHIP_ISR_ENABLE();
			IINCHIP_WRITE(IR, 0xF0);
			IINCHIP_WRITE(SIR, 0xFF);
			if (tmp & Sn_IR_RECV)
			{
				if ((len = getSn_RX_RSR(s)) > 0) 			/* check Rx data */
				{
					if (len > TX_RX_MAX_BUF_SIZE) 
						len = TX_RX_MAX_BUF_SIZE;	/* if Rx data size is lager than TX_RX_MAX_BUF_SIZE */
					/* the data size to read is MAX_BUF_SIZE. */
					len = recvfrom(s, data_buf, len,(uint8*)&destip,&destport);	/* read the received data */
					/*if(sendto(s, data_buf, len,(uint8*)&destip,destport) == 0)	// send the received data 
					{
						printf("\a\a\a%d : System Fatal Error.", s);
					}*/
				}
				IINCHIP_WRITE(Sn_DIPR0(s),addr[0]);
				IINCHIP_WRITE((Sn_DIPR0(s) + 1),addr[1]);
				IINCHIP_WRITE((Sn_DIPR0(s) + 2),addr[2]);
				IINCHIP_WRITE((Sn_DIPR0(s) + 3),addr[3]);
				IINCHIP_WRITE(Sn_DPORT0(s),(uint8)((port & 0xff00) >> 8));
				IINCHIP_WRITE((Sn_DPORT0(s) + 1),(uint8)(port & 0x00ff));
			}
			break;
	}
}

