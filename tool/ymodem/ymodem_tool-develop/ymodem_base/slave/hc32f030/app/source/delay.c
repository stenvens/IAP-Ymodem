/* ����ͷ�ļ� */
#include "delay.h"

/* BSPͷ�ļ� */
#include "sysctrl.h"

static uint8_t  fac_us=0;							//us��ʱ������			   
			   
//��ʼ���ӳٺ���
//��ʹ��OS��ʱ��,�˺������ʼ��OS��ʱ�ӽ���
//SYSTICK��ʱ�ӹ̶�ΪHCLKʱ�ӵ�1/8
//SYSCLK:ϵͳʱ��
void delay_init(uint8_t SYSCLK)
{
	fac_us = SYSCLK;				
}								    

//��ʱnus
//nusΪҪ��ʱ��us��.		    								   
void delay_us(uint32_t nus)
{		
	uint32_t temp;	    	 
	SysTick->LOAD=nus*fac_us; 					//ʱ�����	  		 
	SysTick->VAL=0x00;        					//��ռ�����
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk; //��ʼ����	  
	//���ڻ���оƬ������Ҫ����SysTick_CTRL_CLKSOURCE_Msk
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//�ȴ�ʱ�䵽��   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//�رռ�����
	SysTick->VAL =0X00;      					 //��ռ�����	 
}

//��ʱnms
//nmsΪҪ��ʱ��ms��.
void delay_ms(uint16_t nms)
{	 		  	  
	uint32_t i;

    for(i = 0; i < nms; i++) 
	{
		delay_us(1000);
	}
} 

