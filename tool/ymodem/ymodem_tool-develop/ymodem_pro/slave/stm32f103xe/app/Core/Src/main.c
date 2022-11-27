/********************************************************************************************************
*   ��Ŀ���� : IAP_Ymodem (App)
*   ƽ    ̨ : STM32F103xE
*	��    �� : biglu
*	����˵�� : 
*				�� USART1(PA9/PA10)��ΪͨѶ����,��Ϊ����λ�����������á�
* 
*               �� ֧�ִ���ָ�����룺
*					"reboot", ����������������bootloader
*					"get_device_version"�����ڲ���
*
********************************************************************************************************/

//lib files
#include <stdio.h>

//bsp files
#include "bsp_uart.h"
#include "bsp_sysclk.h"

//funclib files
#include "uart_config.h"
#include "main.h"


void bsp_init(void)
{
	/* HAL���ʼ�� */
	HAL_Init();
	
	/* ϵͳ��ʼ�� */	
	SystemClock_Config();
	
	//iap usart
	bsp_usart1_init();

	__enable_irq();
}

int main(void)
{
	uint16_t cnt = 0;
	
	bsp_init();
	printf("\r\n enter app \r\n");  
	
	while (1)
	{
		HAL_Delay(10);
		cnt++;
		
		//scan per 50ms
		if(cnt%5 == 0)
		{
			uart_recv_proc();
		}
		
		//print per 1000ms
		if(cnt >= 100)
		{
			cnt = 0;
			
			printf("run...\r\n");  
		}
	}
}
