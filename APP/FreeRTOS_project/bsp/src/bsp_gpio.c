#include "bsp_gpio.h"
#include "stm32f10x.h"
/**
  * @brief   GPIO≥ı ºªØ   
  * @note    none  
  * @param   none
  * @retval 
  */
void bsp_gpio_init(void)
{
#ifdef STM32F103x
		GPIO_InitTypeDef  GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
		GPIO_InitStructure.GPIO_Pin 	= UART_TX_PIN;
		GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(UART_TX_PORT, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin 	= UART_RX_PIN;
		GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN_FLOATING;
		GPIO_Init(UART_RX_PORT, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin 	= LED0_PIN;
		GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
		GPIO_Init(LED0_PORT, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Pin 	= LED1_PIN;
		GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_Out_PP;
		GPIO_Init(LED1_PORT, &GPIO_InitStructure);
#endif

}

