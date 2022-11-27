//lib files
#include "ddl.h"
#include "flash.h"
#include "delay.h"

//bsp files
#include "bsp_sysclk.h"
#include "bsp_uart.h"

//func files
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
	uint16_t i = 0;
		
	bsp_init();
	printf("\r\n enter app\r\n");
	
    while(1)
	{
		delay_ms(20);
		
		//串口配置接收处理
		uart_recv_proc();
		
		//test log
		if(++i >= (50 * 5))
		{
			i = 0;
			printf("run...\r\n");
			
		}		
	}
}

