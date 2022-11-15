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
	UART_Handler.Init.BaudRate		=baud;				    		//������
	UART_Handler.Init.WordLength	=UART_WORDLENGTH_8B;  //�ֳ�Ϊ8λ���ݸ�ʽ
	UART_Handler.Init.StopBits		=UART_STOPBITS_1;	    //һ��ֹͣλ
	UART_Handler.Init.Parity			=UART_PARITY_NONE;	  //����żУ��λ
	UART_Handler.Init.HwFlowCtl		=UART_HWCONTROL_NONE; //��Ӳ������
	UART_Handler.Init.Mode				=UART_MODE_TX_RX;		  //�շ�ģʽ
	HAL_UART_Init(&UART_Handler);					   					  //HAL_UART_Init()��ʹ��UART
	HAL_NVIC_EnableIRQ(UART_IRQ_NUM);										//ʹ��USART1�ж�ͨ��
	HAL_NVIC_SetPriority(UART_IRQ_NUM,3,3);							//��ռ���ȼ�3�������ȼ�3
	__HAL_UART_ENABLE_IT(&UART_Handler, UART_IT_RXNE);
	rb1 = get_ring_buffer();
//	HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������
#endif	
}


uint32_t uart_Send(uint8_t *pbuf,uint32_t len)
{
#ifdef STM32F103x	
	HAL_UART_Transmit(&UART_Handler,(uint8_t*)&pbuf,len,1000);
	while(__HAL_UART_GET_FLAG(&UART_Handler, UART_FLAG_TXE)!=SET);	// �ȴ����ͽ���.
#endif	
	return 0;
}


//�ض���fputc���� 
int fputc(int ch, FILE *f)
{   
#ifdef STM32F103x	
	HAL_UART_Transmit(&UART_Handler,(uint8_t*)&ch,1,1000);
	while(__HAL_UART_GET_FLAG(&UART_Handler, UART_FLAG_TXE)!=SET);	// �ȴ����ͽ���.
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
