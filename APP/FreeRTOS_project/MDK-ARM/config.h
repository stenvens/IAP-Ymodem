#ifndef __CONFIG_H_
#define __CONFIG_H_

#include <stdio.h>
#include <stdint.h>
#include "bsp_gpio.h"
#include "bsp_uart.h"
#include "bsp_flash.h"
#include "bsp_timer.h"
#include "bsp_systick.h"
#include "cm_backtrace.h"

#define CM_BACKTRACE   1

#ifdef STM32F103x

#include "stm32f10x.h"

#endif
#define UART_TX_PORT 	GPIOA
#define UART_TX_PIN  	GPIO_Pin_9

#define UART_RX_PORT 	GPIOA
#define UART_RX_PIN  	GPIO_Pin_10

#define UART_NUM  		USART1
#define UART_IRQ_NUM  USART1_IRQn
#define UART_IRQFUN 	USART1_IRQHandler


#define LED0_PORT			GPIOA
#define LED0_PIN 			GPIO_Pin_8

#define LED1_PORT			GPIOD
#define LED1_PIN 			GPIO_Pin_2

#define SYSTICK_FRE         1000

#endif

