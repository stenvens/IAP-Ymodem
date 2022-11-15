#ifndef __RING_BUFFER_H_
#define __RING_BUFFER_H_
#include "config.h"

#define UART_RING_BUFSIZE 1024                                                		 	// USART 最大接收长度.
#define __BUF_MASK (UART_RING_BUFSIZE-1)                                       			// Buf mask.
#define __BUF_IS_FULL(head, tail)   		((tail&__BUF_MASK)==((head+1)&__BUF_MASK))  // Check buf is full or not.
#define __BUF_WILL_FULL(head, tail) 		((tail&__BUF_MASK)==((head+2)&__BUF_MASK)) 	// Check buf will be full in next receiving or not.
#define __BUF_IS_EMPTY(head, tail)  		((head&__BUF_MASK)==(tail&__BUF_MASK))      // Check buf is empty.
#define __BUF_RESET(bufidx) 				(bufidx=0)                                  		// Reset buf.
#define __BUF_INCR(bufidx)  				(bufidx=(bufidx+1)&__BUF_MASK)             		 	// Increase buf.

//* 结构体定义.
typedef struct
{
    __IO uint16_t rx_head;                  // 环形队列头
    __IO uint16_t rx_tail;                  // 环形队列尾
    __IO uint8_t  rx[UART_RING_BUFSIZE];    // 环形缓冲区
} USART_RING_BUFFER;                        // 环形结构体  

USART_RING_BUFFER *get_ring_buffer(void);
char GetRingBufchar(uint8_t *data,uint32_t timeout);
#endif

