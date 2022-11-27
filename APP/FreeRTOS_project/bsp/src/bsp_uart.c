#include "config.h"
#include "bsp_uart.h"
#include "stdio.h"
#include "ring_buffer.h"


extern  RING_BUFF	recvBuff;


//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0){}//循环发送,直到发送完毕   
    USART1->DR = (uint8_t)ch;      
	return ch;
}



void bsp_uartinit(uint32_t baud)
{
#ifdef STM32F103x
	
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	USART_InitStructure.USART_BaudRate 		= baud;																		//串口波特率
	USART_InitStructure.USART_WordLength 	= USART_WordLength_8b;										//字长为8位数据格式
	USART_InitStructure.USART_StopBits 		= USART_StopBits_1;												//一个停止位
	USART_InitStructure.USART_Parity 			= USART_Parity_No;												//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//无硬件数据流控制
	USART_InitStructure.USART_Mode 				= USART_Mode_Rx | USART_Mode_Tx;					//收发模式

  USART_Init(UART_NUM, &USART_InitStructure); 		//初始化串口1
  USART_ITConfig(UART_NUM, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(UART_NUM, ENABLE);                    //使能串口1 

	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel 										= UART_IRQ_NUM;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	=	3 ;			//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 				= 3;			//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd 								= ENABLE;	//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);																	//根据指定的参数初始化VIC寄存器
  
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
  * @brief   			串口中断函数   
  * @note    			none  
	* @param[in]   	none
	* @param[out]   none
  * @retval 
  */
void UART_IRQFUN(void)
{
#ifdef STM32F103x	
		uint8_t res;
		if(USART_GetITStatus(UART_NUM, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
			USART_ClearITPendingBit(UART_NUM, USART_IT_RXNE);
			res =USART_ReceiveData(USART1);	//读取接收到的数据
			Ring_BuffWrite(&recvBuff,res);
		}
#endif	

}
