#include "bsp_uart.h"
#include "config.h"
#include "stdio.h"
#include "ring_buffer.h"

static UART_HandleTypeDef UART_Handler;
static USART_RING_BUFFER *rb1 = NULL;

void bsp_uartinit(uint32_t baud)
{
#ifdef STM32F103x
	
	UART_Handler.Instance					=UART_NUM;					  
	UART_Handler.Init.BaudRate		=baud;				    		//波特率
	UART_Handler.Init.WordLength	=UART_WORDLENGTH_8B;  //字长为8位数据格式
	UART_Handler.Init.StopBits		=UART_STOPBITS_1;	    //一个停止位
	UART_Handler.Init.Parity			=UART_PARITY_NONE;	  //无奇偶校验位
	UART_Handler.Init.HwFlowCtl		=UART_HWCONTROL_NONE; //无硬件流控
	UART_Handler.Init.Mode				=UART_MODE_TX_RX;		  //收发模式
	HAL_UART_Init(&UART_Handler);					   					  //HAL_UART_Init()会使能UART
	HAL_NVIC_EnableIRQ(UART_IRQ_NUM);										//使能USART1中断通道
	HAL_NVIC_SetPriority(UART_IRQ_NUM,3,3);							//抢占优先级3，子优先级3
	__HAL_UART_ENABLE_IT(&UART_Handler, UART_IT_RXNE);
	rb1 = get_ring_buffer();
//	HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE);//该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
#endif	
}


uint32_t uart_Send(uint8_t *pbuf,uint32_t len)
{
#ifdef STM32F103x	
	HAL_UART_Transmit(&UART_Handler,(uint8_t*)&pbuf,len,1000);
	while(__HAL_UART_GET_FLAG(&UART_Handler, UART_FLAG_TXE)!=SET);	// 等待发送结束.
#endif	
	return 0;
}


//重定义fputc函数 
int fputc(int ch, FILE *f)
{   
#ifdef STM32F103x	
	HAL_UART_Transmit(&UART_Handler,(uint8_t*)&ch,1,1000);
	while(__HAL_UART_GET_FLAG(&UART_Handler, UART_FLAG_TXE)!=SET);	// 等待发送结束.
#endif	
	return ch;
}


/**
* @brief This function handles USART1 global interrupt.
*/
void UART_IRQFUN(void)
{
#ifdef STM32F103x	
  /* USER CODE BEGIN USART1_IRQn 0 */
   uint8_t res;
   if(__HAL_UART_GET_FLAG(&UART_Handler, UART_FLAG_RXNE)!= RESET)
   {
	   HAL_UART_Receive(&UART_Handler,(uint8_t*)&res,sizeof(res),1000);
	 		//* save the receive data.
    if (!__BUF_IS_FULL(rb1->rx_head,rb1->rx_tail))
	  {
			rb1->rx[rb1->rx_head] = res;
			__BUF_INCR(rb1->rx_head);
    }
	 
   }
	__HAL_UART_CLEAR_FLAG(&UART_Handler, UART_FLAG_RXNE);

  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&UART_Handler);
  /* USER CODE BEGIN USART1_IRQn 1 */
#endif	
}
