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
	GPIO_InitTypeDef GPIO_Initure;
	
		__HAL_RCC_GPIOA_CLK_ENABLE();			
	  UART_CLK();
		__HAL_RCC_AFIO_CLK_ENABLE();
	
		GPIO_Initure.Pin		=UART_TX_PIN;			
		GPIO_Initure.Mode		=GPIO_MODE_AF_PP;		
		GPIO_Initure.Pull		=GPIO_PULLUP;		
		GPIO_Initure.Speed	=GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(UART_TX_PORT,&GPIO_Initure);	

		GPIO_Initure.Pin	=UART_RX_PIN;
		GPIO_Initure.Mode	=GPIO_MODE_AF_INPUT;	
		HAL_GPIO_Init(UART_TX_PORT,&GPIO_Initure);
		
#endif

}

