/********************************************************************************************************
*   ��Ŀ���� : IAP_Ymodem (App)
*   ƽ    ̨ : STM32G030C8xx
*	��    �� : biglu
*	����˵�� : 
*				�� USART2(PA2/PA3)��ΪͨѶ����,��Ϊ����λ�����������úʹ���ָ�����
* 
*               �� ���ڽ���ָ������uart_config.c�µ�uart_cmd_table[]��
*				   ������"get_device_version"�����Գɹ��������µ�APP��
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
	bsp_usart2_init();

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

