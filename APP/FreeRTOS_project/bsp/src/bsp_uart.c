#include "config.h"
#include "bsp_uart.h"
#include "stdio.h"
#include "ring_buffer.h"


extern  RING_BUFF	recvBuff;


//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0){}//ѭ������,ֱ���������   
    USART1->DR = (uint8_t)ch;      
	return ch;
}



void bsp_uartinit(uint32_t baud)
{
#ifdef STM32F103x
	
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	USART_InitStructure.USART_BaudRate 		= baud;																		//���ڲ�����
	USART_InitStructure.USART_WordLength 	= USART_WordLength_8b;										//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits 		= USART_StopBits_1;												//һ��ֹͣλ
	USART_InitStructure.USART_Parity 			= USART_Parity_No;												//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//��Ӳ������������
	USART_InitStructure.USART_Mode 				= USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ

  USART_Init(UART_NUM, &USART_InitStructure); 		//��ʼ������1
  USART_ITConfig(UART_NUM, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(UART_NUM, ENABLE);                    //ʹ�ܴ���1 

	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel 										= UART_IRQ_NUM;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	=	3 ;			//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 				= 3;			//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd 								= ENABLE;	//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);																	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
#endif	
		RING_BUFFInit(&recvBuff);
}


uint32_t uart_Send(uint8_t *pbuf,uint32_t len)
{
#ifdef STM32F103x	
	for(int i = 0;i<len;i++)
	  USART_SendData(UART_NUM,pbuf[i]);
#endif	
	return 0;
}


/**
  * @brief   			�����жϺ���   
  * @note    			none  
	* @param[in]   	none
	* @param[out]   none
  * @retval 
  */
void UART_IRQFUN(void)
{
#ifdef STM32F103x	
		uint8_t res;
		if(USART_GetITStatus(UART_NUM, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
			USART_ClearITPendingBit(UART_NUM, USART_IT_RXNE);
			res =USART_ReceiveData(USART1);	//��ȡ���յ�������
			Ring_BuffWrite(&recvBuff,res);
		}
#endif	

}
