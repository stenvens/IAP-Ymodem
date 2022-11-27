/********************************************************************************************************
*   ��Ŀ���� : IAP_Ymodem (App)
*   ƽ    ̨ : HC32F030J8TA
*	��    �� : biglu
*	����˵�� : 
*				�� USART1(PA2/PA3)��ΪͨѶ����,��Ϊ����λ�����������á�
* 
*               �� ֧�ִ���ָ�����룺
*					"reboot", ����������������bootloader
*					"get_device_version"�����ڲ���
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

