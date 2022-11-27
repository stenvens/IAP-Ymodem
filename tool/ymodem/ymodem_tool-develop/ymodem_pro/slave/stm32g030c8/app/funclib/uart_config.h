#ifndef _UART_CONFIG_H_
#define _UART_CONFIG_H_
#include <stdint.h>
#include <string.h>
#include <stdio.h>


//配置串口
typedef struct
{
  	uint32_t recv_length;
  	uint32_t send_length;
	uint8_t recv_buf[50];
	uint8_t send_buf[50];
}config_uart_t;

//变量外部声明
extern config_uart_t config_uart;

//函数声明
void uart_config_send(char *buf, uint16_t length);
void uart_config_transmition(void);
void uart_recv_proc(void);

#endif





