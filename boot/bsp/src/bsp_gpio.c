#include "bsp_gpio.h"

/**
  * @brief   UART对应GPIO初始化   
  * @note    none  
  * @param   none
  * @retval 
  */
void uart_gpio_init(void)
{
#ifdef STM32F103x
		GPIO_InitTypeDef  GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

		GPIO_InitStructure.GPIO_Pin 	= UART_TX_PIN;
		GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(UART_TX_PORT, &GPIO_InitStructure);

		GPIO_InitStructure.GPIO_Pin 	= UART_RX_PIN;
		GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_IN_FLOATING;
		GPIO_Init(UART_RX_PORT, &GPIO_InitStructure);
#endif

}

