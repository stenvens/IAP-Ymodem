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
* @brief  ����ʼ��
*********************************************************************/
static void bsp_init(void)
{
	//ʱ�ӳ�ʼ��
	RCH16_SysClkConfig();
	delay_init(16);
	
	//IAP����
	bsp_uart1_init();
	
	//���ж�
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
		
		//�������ý��մ���
		uart_recv_proc();
		
		//test log
		if(++i >= (50 * 5))
		{
			i = 0;
			printf("run...\r\n");
			
		}		
	}
}

