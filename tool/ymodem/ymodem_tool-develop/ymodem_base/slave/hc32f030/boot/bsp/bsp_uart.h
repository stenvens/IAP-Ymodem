#ifndef _BSP_UART_H
#define _BSP_UART_H

#include "gpio.h"

//��ʹ��UART�⣬�����uart.c������
//ע��ԭ�пⲨ����Ĭ��uint16_t��ʹ�üĴ�����֧������uint32_t
//#define USE_UART_LIB	1

/*********************************************** UART1 ***********************************************/
//UART1_TX�˿ڶ���
#define UART1_TX_PORT		GpioPortA
#define UART1_TX_PIN		GpioPin2

//UART1_RX�˿ڶ���
#define UART1_RX_PORT		GpioPortA
#define UART1_RX_PIN		GpioPin3

//UART1������
#define UART1_BAUD			9600	

//UART1��󻺴泤��
#define UART1_BUF_MAXIMUM	10

//�����ⲿ����
extern uint8_t uart1_rx_buf[UART1_BUF_MAXIMUM];
extern uint16_t uart1_rx_cnt;

//��������
void bsp_uart1_deinit(void);
void bsp_uart1_init(void);
void bsp_uart1_send_byte(uint8_t data);
void bsp_uart1_send(uint8_t *buf, uint16_t length);
/*********************************************** UART1 ***********************************************/


/*********************************************** UART0 ***********************************************/
//UART0_TX�˿ڶ���
#define UART0_TX_PORT		GpioPortB
#define UART0_TX_PIN		GpioPin8

//UART0_RX�˿ڶ���
#define UART0_RX_PORT		GpioPortB
#define UART0_RX_PIN		GpioPin9

//UART0-TX�˿ڲ���
#define UART0_TX_ON			Gpio_SetIO(UART0_TX_PORT, UART0_TX_PIN)
#define UART0_TX_OFF		Gpio_ClrIO(UART0_TX_PORT, UART0_TX_PIN)

//UART0-RX�˿ڲ���
#define UART0_RX_ON			Gpio_SetIO(UART0_RX_PORT, UART0_RX_PIN)
#define UART0_RX_OFF		Gpio_ClrIO(UART0_RX_PORT, UART0_RX_PIN)


//UART0������
#define UART0_BAUD			9600	

//UART0��󻺴泤��
#define UART0_BUF_MAXIMUM	10 //��ʱû�õ����Ϳ�����С��ֵ

//�����ⲿ����
extern uint8_t uart0_rx_buf[UART0_BUF_MAXIMUM];
extern uint16_t uart0_rx_cnt;

//��������
void bsp_uart0_deinit(void);
void bsp_uart0_init(void);
void bsp_uart0_send_byte(uint8_t data);
void bsp_uart0_send(uint8_t *buf, uint16_t length);

/*********************************************** UART0 ***********************************************/




#endif

