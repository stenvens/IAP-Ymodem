/********************************************************************************************************
*   项目名称 : IAP_Ymodem (App)
*   平    台 : STM32G030C8xx
*	作    者 : biglu
*	功能说明 : 
*				① USART2(PA2/PA3)作为通讯串口,作为与上位机升级工具用和串口指令交互。
* 
*               ② 串口交互指令表放于uart_config.c下的uart_cmd_table[]，
*				   比如用"get_device_version"来测试成功下载了新的APP。
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
	/* HAL库初始化 */
	HAL_Init();
	
	/* 系统初始化 */	
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

