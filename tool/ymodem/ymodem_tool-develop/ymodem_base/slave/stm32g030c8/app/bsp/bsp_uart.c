#include "bsp_uart.h"
#include <stdio.h>


/******************************************* Printf重映射 ******************************************/
#if 1
#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
    int handle;
};

FILE __stdout;
/**
 * @brief	定义_sys_exit()以避免使用半主机模式
 *
 * @param	void
 *
 * @return  void
 */
void _sys_exit(int x)
{
    x = x;
}
/**
 * @brief	重定义fputc函数
 *
 * @param	ch		输出字符量
 * @param	f		文件指针
 *
 * @return  void
 */
int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&huart2, (uint8_t *)&ch, 1, 0xFFFF);
	
    return ch;
}
#endif
/***************************************************************************************************/


/********************************************** 串口1 **********************************************/
UART_HandleTypeDef huart1;
uint8_t usart1_rx_buf[USART1_BUF_MAXIMUM] = {0}; //USART1接收缓冲区
uint16_t usart1_rx_cnt = 0; //USART1当前接收长度

/*********************************************************************
* @brief  ：串口1初始化
*********************************************************************/
void bsp_usart1_init(void)
{
	huart1.Instance = USART1;
	huart1.Init.BaudRate = USART1_BAUD;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	HAL_UART_Init(&huart1);
	
	HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8);
	HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8);
	HAL_UARTEx_DisableFifoMode(&huart1);
	
	//开启接收中断
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
}

/*********************************************************************
* @brief  ：串口1复位
*********************************************************************/
void bsp_usart1_deinit(void)
{
	HAL_UART_DeInit(&huart1);
}

/*********************************************************************
* @brief  ：串口1发送数据
*---------------------------------------------------------------------
* @param  ：uint8_t *buf		,	数据内容	
*			uint16_t length		,	数据长度
*---------------------------------------------------------------------
* @retval ：none
*********************************************************************/
void bsp_usart1_send(uint8_t *buf, uint16_t length)
{
	HAL_UART_Transmit(&huart1, buf, length, 0xFFFF);
}

/*********************************************************************
* @brief  ：串口1中断服务程序
*********************************************************************/
void USART1_IRQHandler(void)
{
	volatile uint8_t res = 0;
	
	if((__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE) != RESET))
	{		
		res = (USART1->RDR & (uint16_t)0x01FF);
		usart1_rx_buf[usart1_rx_cnt++] = res;
		if(usart1_rx_cnt >= USART1_BUF_MAXIMUM)
		{
			usart1_rx_cnt = 0;
		}
	}
	
	HAL_UART_IRQHandler(&huart1);
}
/***************************************************************************************************/


/********************************************** 串口2 **********************************************/
UART_HandleTypeDef huart2;
uint8_t usart2_rx_buf[USART2_BUF_MAXIMUM] = {0}; //USART2接收缓冲区
uint16_t usart2_rx_cnt = 0; //USART2当前接收长度

/*********************************************************************
* @brief  ：串口2初始化
*********************************************************************/
void bsp_usart2_init(void)
{
	huart2.Instance = USART2;
	huart2.Init.BaudRate = USART2_BAUD;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	HAL_UART_Init(&huart2);

	HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8);
	HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8);
	HAL_UARTEx_DisableFifoMode(&huart2);
	
	//开启接收中断
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
}

/*********************************************************************
* @brief  ：串口2复位
*********************************************************************/
void bsp_usart2_deinit(void)
{
	HAL_UART_DeInit(&huart2);
}

/*********************************************************************
* @brief  ：串口2发送数据
*---------------------------------------------------------------------
* @param  ：uint8_t *buf		,	数据内容	
*			uint16_t length		,	数据长度
*---------------------------------------------------------------------
* @retval ：none
*********************************************************************/
void bsp_usart2_send(uint8_t *buf, uint16_t length)
{
	HAL_UART_Transmit(&huart2, buf, length, 0xFFFF);
}

/*********************************************************************
* @brief  ：串口2中断服务程序
*********************************************************************/
void USART2_IRQHandler(void)
{
	volatile uint8_t res = 0;
	
	if((__HAL_UART_GET_FLAG(&huart2, UART_FLAG_RXNE) != RESET))
	{		
		res = (USART2->RDR & (uint16_t)0x01FF);
	}
	
	HAL_UART_IRQHandler(&huart2);
}
/***************************************************************************************************/

/*********************************************************************
* @brief  ：串口 MspInit
*********************************************************************/
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if(huart->Instance==USART1)
	{
		/* Peripheral clock enable */
		__HAL_RCC_USART1_CLK_ENABLE();

		__HAL_RCC_GPIOB_CLK_ENABLE();
		/**USART1 GPIO Configuration
		PB6     ------> USART1_TX
		PB7     ------> USART1_RX
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF0_USART1;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		
		//USART1中断和优先级配置
		HAL_NVIC_EnableIRQ(USART1_IRQn);
		HAL_NVIC_SetPriority(USART1_IRQn, 2, 0);	
	}
	else if(huart->Instance==USART2)
	{
		/* Peripheral clock enable */
		__HAL_RCC_USART2_CLK_ENABLE();

		__HAL_RCC_GPIOA_CLK_ENABLE();
		/**USART2 GPIO Configuration
		PA2     ------> USART2_TX
		PA3     ------> USART2_RX
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF1_USART2;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		//USART2中断和优先级配置
		HAL_NVIC_EnableIRQ(USART2_IRQn);
		HAL_NVIC_SetPriority(USART2_IRQn, 2, 1);	
	}
}

/*********************************************************************
* @brief  ：串口 MspDeInit
*********************************************************************/
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
	if(huart->Instance==USART1)
	{
		/* Peripheral clock disable */
		__HAL_RCC_USART1_CLK_DISABLE();

		/**USART1 GPIO Configuration
		PB6     ------> USART1_TX
		PB7     ------> USART1_RX
		*/
		HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6|GPIO_PIN_7);
		
		//串口1中断关闭
		HAL_NVIC_DisableIRQ(USART1_IRQn);
	}
	else if(huart->Instance==USART2)
	{
		/* Peripheral clock disable */
		__HAL_RCC_USART2_CLK_DISABLE();

		/**USART2 GPIO Configuration
		PA2     ------> USART2_TX
		PA3     ------> USART2_RX
		*/
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);
		
		//串口2中断关闭
		HAL_NVIC_DisableIRQ(USART2_IRQn);
	}
}

