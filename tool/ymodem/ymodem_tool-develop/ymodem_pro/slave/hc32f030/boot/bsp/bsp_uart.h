#ifndef _BSP_UART_H
#define _BSP_UART_H

#include "gpio.h"

//若使用UART库，则添加uart.c到工程
//注：原有库波特率默认uint16_t，使用寄存器可支持设置uint32_t
//#define USE_UART_LIB	1

/*********************************************** UART1 ***********************************************/
//UART1_TX端口定义
#define UART1_TX_PORT		GpioPortA
#define UART1_TX_PIN		GpioPin2

//UART1_RX端口定义
#define UART1_RX_PORT		GpioPortA
#define UART1_RX_PIN		GpioPin3

//UART1波特率
#define UART1_BAUD			9600	

//UART1最大缓存长度
#define UART1_BUF_MAXIMUM	10

//变量外部声明
extern uint8_t uart1_rx_buf[UART1_BUF_MAXIMUM];
extern uint16_t uart1_rx_cnt;

//函数声明
void bsp_uart1_deinit(void);
void bsp_uart1_init(void);
void bsp_uart1_send_byte(uint8_t data);
void bsp_uart1_send(uint8_t *buf, uint16_t length);
/*********************************************** UART1 ***********************************************/


/*********************************************** UART0 ***********************************************/
//UART0_TX端口定义
#define UART0_TX_PORT		GpioPortB
#define UART0_TX_PIN		GpioPin8

//UART0_RX端口定义
#define UART0_RX_PORT		GpioPortB
#define UART0_RX_PIN		GpioPin9

//UART0-TX端口操作
#define UART0_TX_ON			Gpio_SetIO(UART0_TX_PORT, UART0_TX_PIN)
#define UART0_TX_OFF		Gpio_ClrIO(UART0_TX_PORT, UART0_TX_PIN)

//UART0-RX端口操作
#define UART0_RX_ON			Gpio_SetIO(UART0_RX_PORT, UART0_RX_PIN)
#define UART0_RX_OFF		Gpio_ClrIO(UART0_RX_PORT, UART0_RX_PIN)


//UART0波特率
#define UART0_BAUD			9600	

//UART0最大缓存长度
#define UART0_BUF_MAXIMUM	10 //暂时没用到，就开个很小的值

//变量外部声明
extern uint8_t uart0_rx_buf[UART0_BUF_MAXIMUM];
extern uint16_t uart0_rx_cnt;

//函数声明
void bsp_uart0_deinit(void);
void bsp_uart0_init(void);
void bsp_uart0_send_byte(uint8_t data);
void bsp_uart0_send(uint8_t *buf, uint16_t length);

/*********************************************** UART0 ***********************************************/




#endif

