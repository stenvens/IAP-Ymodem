/* 其他头文件 */
#include "delay.h"

/* BSP头文件 */
#include "sysctrl.h"

static uint8_t  fac_us=0;							//us延时倍乘数			   
			   
//初始化延迟函数
//当使用OS的时候,此函数会初始化OS的时钟节拍
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK:系统时钟
void delay_init(uint8_t SYSCLK)
{
	fac_us = SYSCLK;				
}								    

//延时nus
//nus为要延时的us数.		    								   
void delay_us(uint32_t nus)
{		
	uint32_t temp;	    	 
	SysTick->LOAD=nus*fac_us; 					//时间加载	  		 
	SysTick->VAL=0x00;        					//清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk; //开始倒数	  
	//对于华大芯片，这里要加上SysTick_CTRL_CLKSOURCE_Msk
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
	SysTick->VAL =0X00;      					 //清空计数器	 
}

//延时nms
//nms为要延时的ms数.
void delay_ms(uint16_t nms)
{	 		  	  
	uint32_t i;

    for(i = 0; i < nms; i++) 
	{
		delay_us(1000);
	}
} 

