/********************************************************************************************************
*   项目名称 : IAP_Ymodem (App)
*   平    台 : HC32F030J8TA
*	作    者 : biglu
*	功能说明 : 
*				① USART1(PA2/PA3)作为通讯串口,作为与上位机升级工具用。
* 
*               ② 支持串口指令输入：
*					"reboot", 用来发起升级进入bootloader
*					"get_device_version"，用于测试
*
********************************************************************************************************/

//lib files
#include "ddl.h"
#include "flash.h"
#include "delay.h"

//bsp files
#include "bsp_sysclk.h"
#include "bsp_uart.h"

//funclib files
#include "uart_config.h"


/*********************************************************************
* @brief  ：初始化
*********************************************************************/
static void bsp_init(void)
{
	//时钟初始化
	RCH16_SysClkConfig();
	delay_init(16);
	
	//IAP串口
	bsp_uart1_init();
	
	//开中断
	__enable_irq();
}

int32_t main(void)
{	
	uint16_t cnt = 0;
		
	bsp_init();
	printf("\r\n enter app\r\n");
	
    while(1)
	{
		delay_ms(10);
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

