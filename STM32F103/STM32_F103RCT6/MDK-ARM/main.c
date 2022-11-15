#include "stdio.h"
#include "config.h"
#include "bsp_systick.h"
#include "bsp_gpio.h"
#include "bsp_uart.h"
#include "bsp_flash.h"
#include "app_ymodem.h"
static uint8_t UartRecBuf[248] ={0};
static uint32_t code_size = 0;

uint8_t test[67] = {0};

#if CM_BACKTRACE == 1

#include "cm_backtrace.h"

#endif
int main(void)
{
	
	int i =0;
#ifdef STM32F103x
		HAL_Init(); //��ʼ��HAL��
	  HAL_StatusTypeDef ret = HAL_OK;
    RCC_OscInitTypeDef RCC_OscInitStructure; 
    RCC_ClkInitTypeDef RCC_ClkInitStructure;
    
    RCC_OscInitStructure.OscillatorType=RCC_OSCILLATORTYPE_HSE;    	//ʱ��ԴΪHSE
    RCC_OscInitStructure.HSEState=RCC_HSE_ON;                      	//��HSE
		RCC_OscInitStructure.HSEPredivValue=RCC_HSE_PREDIV_DIV1;		//HSEԤ��Ƶ
    RCC_OscInitStructure.PLL.PLLState=RCC_PLL_ON;					//��PLL
    RCC_OscInitStructure.PLL.PLLSource=RCC_PLLSOURCE_HSE;			//PLLʱ��Դѡ��HSE
    RCC_OscInitStructure.PLL.PLLMUL=RCC_PLL_MUL9; 							//��PLL��Ƶ����
    ret=HAL_RCC_OscConfig(&RCC_OscInitStructure);//��ʼ��
	
    if(ret!=HAL_OK) while(1);
    
    //ѡ��PLL��Ϊϵͳʱ��Դ��������HCLK,PCLK1��PCLK2
    RCC_ClkInitStructure.ClockType=(RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStructure.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK;		//����ϵͳʱ��ʱ��ԴΪPLL
    RCC_ClkInitStructure.AHBCLKDivider=RCC_SYSCLK_DIV1;				//AHB��Ƶϵ��Ϊ1
    RCC_ClkInitStructure.APB1CLKDivider=RCC_HCLK_DIV2; 				//APB1��Ƶϵ��Ϊ2
    RCC_ClkInitStructure.APB2CLKDivider=RCC_HCLK_DIV1; 				//APB2��Ƶϵ��Ϊ1
    ret=HAL_RCC_ClockConfig(&RCC_ClkInitStructure,FLASH_LATENCY_2);	//ͬʱ����FLASH��ʱ����Ϊ2WS��Ҳ����3��CPU���ڡ�
		
    if(ret!=HAL_OK) while(1);
#endif
		SysTick_init(SYSTICK_FRE);
	  uart_gpio_init();
	  bsp_uartinit(115200);
#if CM_BACKTRACE == 1

		cm_backtrace_init("YMODEM_UPDATE","V1.0","V1.0");
		printf("ymodem_test!\r\n");
#endif

	for( i = 0;i<67;i++)
	{
	
		test[i] = i;
	}
	bsp_flash_erase(0x8004000U,1);
	
	bsp_flash_write(0x8004000U,test,65);
  

	/* Ymodem���� */
	code_size = Ymodem_Receive(UartRecBuf, APP_ADDRESS);
	if(code_size != 0)
	{
		
	
	}
	else
	{
	  
//		JumpToApp();
	
	}
	/* ��ת��APP */
//	JumpToApp();
	 
	while(1)
	{
		
	
	}
}
