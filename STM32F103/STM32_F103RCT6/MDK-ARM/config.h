#ifndef __CONFIG_H_
#define __CONFIG_H_

#define CM_BACKTRACE   1

#ifdef STM32F103x

#include "stm32f1xx.h"
#define UART_TX_PORT 	GPIOA
#define UART_TX_PIN  	GPIO_PIN_9

#define UART_RX_PORT 	GPIOA
#define UART_RX_PIN  	GPIO_PIN_10

#define UART_NUM  		USART1
#define UART_IRQ_NUM  USART1_IRQn
#define UART_IRQFUN 	USART1_IRQHandler
#define UART_CLK      __HAL_RCC_USART1_CLK_ENABLE
#endif
	

#define FLASG_PAGE_SIZE    
#define APP_ADDRESS    			0x8004000U    //应用程序的起始地址
#define FLASH_BASE_ADDR     (uint32_t)(FLASH_BASE)		/* 0x08000000 */
#define FLASH_END_ADDR      (uint32_t)(0x0803FFFF)

#define FLASH_PAGESIZE	   (0x800U)		/* 页大小，字节 */
#define SYSTICK_FRE         1000

#endif

