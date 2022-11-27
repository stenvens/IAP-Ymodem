#ifndef _BSP_UART_H_
#define _BSP_UART_H_
#include "main.h"

//USART1波特率
#define USART1_BAUD			115200	

//USART2波特率
#define USART2_BAUD			115200	

//USART1最大缓存长度
#define USART1_BUF_MAXIMUM	2200

//USART2最大缓存长度
#define USART2_BUF_MAXIMUM	50

extern uint8_t usart1_rx_buf[USART1_BUF_MAXIMUM];
extern uint16_t usart1_rx_cnt;
extern uint8_t usart2_rx_buf[USART2_BUF_MAXIMUM];
extern uint16_t usart2_rx_cnt;

void bsp_usart1_init(void);
void bsp_usart1_deinit(void);
void bsp_usart1_send(uint8_t *buf, uint16_t length);
void bsp_usart2_init(void);
void bsp_usart2_deinit(void);
void bsp_usart2_send(uint8_t *buf, uint16_t length);
	
#endif

