#include "bsp_sysclk.h"


void bsp_sysclk_hsi_16_init(void)
{
	/* HCLK_Frequency < 24MHz default LL_FLASH_LATENCY_0 0WS */
		
	/* HSI configuration and activation */
	LL_RCC_HSI_Enable();
	while(LL_RCC_HSI_IsReady() != 1)
	{
	}

	/* Set AHB prescaler*/
	LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

	/* Sysclk activation on the HSI */
	LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);
	while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI)
	{
	}

	/* Set APB1 prescaler*/
	LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
	LL_Init1msTick(16000000);
	/* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
	LL_SetSystemCoreClock(16000000);
	LL_RCC_SetUSARTClockSource(LL_RCC_USART2_CLKSOURCE_PCLK1);
}

