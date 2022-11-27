#ifndef _UART_H_
#define _UART_H_
#include "config.h"

void bsp_uartinit(uint32_t baud);
uint32_t uart_Send(uint8_t *pbuf,uint32_t len);
#endif
