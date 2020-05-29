#include "config.h"
#include "timer.h"
#include "usart.h"
#include "seg.h"
#include "mcu_init.h"
/* Private variables ---------------------------------------------------------*/
uint32 ms=0;
uint8 sec=0,min=0;
uint8 hour=0;
uint32 mode_ms;
//for seg
uint16 inact=0;//uint: s
uint16 recon=0;//uint: ms
uint16 nagle_count=0;
uint16 plus_wait_count=0;//uint: ms
//http timer
uint16 cgi_post_wait_time=0;
uint8 http_time_flag = 0;
uint16 http_time_cnt = 0;//after 5 minutes, the pwd_rxd flag will be reset.
uint8 read_PHY_flag = 0;

extern uint32  dhcp_time;
extern uint8 ip_setted;//dhcp 

void Timer_Configuration(void)
{
	  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	
		/* 设置TIM2CLK 为 72MHZ */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
    TIM_TimeBaseStructure.TIM_Period=1000;
    TIM_TimeBaseStructure.TIM_Prescaler= 71;
	
		/* 对外部时钟进行采样的时钟分频,这里没有用到 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
    
	  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
		
    TIM_Cmd(TIM2, ENABLE);
}
/**************************/
/*****Timer2 interrupt*****/
/**************************/
void Timer2_ISR(void)
{
  ms++;//mili second
  nagle_count++;
	    //device working time in second
  ConfigMsg.on_time++;
  recon++;
  if(plus_wait_flag==1)//出现了3个+	
	{
		plus_wait_count++;	
		if( plus_wait_count>999)//收到+++ 延时1S内无新数据  
		{
			plus_wait_count=0;
			plus_wait_flag=0;
			change_to_at=1;//有待改变
		}			
	}
  
  if(ms==1000)//second
  {     
    sec++;
    inact++;
    /*ka_period++;*/
    //ka_interval++;
    //dhcp
		if(LED3==0) LED3=1;
			else LED3=0;
		if(sec%2==0) read_PHY_flag=1;
			dhcp_time++;
		ms=0;
	
  }
  if(sec==60)//minute
  {   
    //http timeout counter
		if(http_time_flag) http_time_cnt++;
		min++;
		sec=0;
  }
  if(min==60)//hour
  {
    min=0;
		hour++;
  }
  if(hour>=24)//day
  {
    hour=0;
    //reboot_flag=1;
  }
}

