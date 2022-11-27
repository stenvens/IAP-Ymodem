#include "bsp_uart.h"
#include <stdio.h>


/******************************************* Printf��ӳ�� ******************************************/
#if 1
#pragma import(__use_no_semihosting)
//��׼����Ҫ��֧�ֺ���
struct __FILE
{
    int handle;
};

FILE __stdout;
/**
 * @brief	����_sys_exit()�Ա���ʹ�ð�����ģʽ
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
 * @brief	�ض���fputc����
 *
 * @param	ch		����ַ���
 * @param	f		�ļ�ָ��
 *
 * @return  void
 */
int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
	
    return ch;
}
#endif
/***************************************************************************************************/


/********************************************** ����1 **********************************************/
UART_HandleTypeDef huart1;
uint8_t usart1_rx_buf[USART1_BUF_MAXIMUM] = {0}; //USART1���ջ�����
uint16_t usart1_rx_cnt = 0; //USART1��ǰ���ճ���

/*********************************************************************
* @brief  ������1��ʼ��
*********************************************************************/
void bsp_usart1_init(void)
{
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&huart1);
	
	//���������ж�
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
}

/*********************************************************************
* @brief  ������1��λ
*********************************************************************/
void bsp_usart1_deinit(void)
{
	HAL_UART_DeInit(&huart1);
}

/*********************************************************************
* @brief  ������1��������
*---------------------------------------------------------------------
* @param  ��uint8_t *buf		,	��������	
*			uint16_t length		,	���ݳ���
*---------------------------------------------------------------------
* @retval ��none
*********************************************************************/
void bsp_usart1_send(uint8_t *buf, uint16_t length)
{
	HAL_UART_Transmit(&huart1, buf, length, 0xFFFF);
}

/*********************************************************************
* @brief  ������1�жϷ������
*********************************************************************/
void USART1_IRQHandler(void)
{
	volatile uint8_t res = 0;
	
	if((__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE) != RESET))
	{		
		res = (USART1->DR & (uint16_t)0x01FF);
		usart1_rx_buf[usart1_rx_cnt++] = res;
		if(usart1_rx_cnt >= USART1_BUF_MAXIMUM)
		{
			usart1_rx_cnt = 0;
		}
	}
	
	HAL_UART_IRQHandler(&huart1);
}
/***************************************************************************************************/


/********************************************** ����2 **********************************************/
UART_HandleTypeDef huart2;
uint8_t usart2_rx_buf[USART2_BUF_MAXIMUM] = {0}; //USART2���ջ�����
uint16_t usart2_rx_cnt = 0; //USART2��ǰ���ճ���

/*********************************************************************
* @brief  ������2��ʼ��
*********************************************************************/
void bsp_usart2_init(void)
{
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&huart2);

	//���������ж�
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
}

/*********************************************************************
* @brief  ������2��λ
*********************************************************************/
void bsp_usart2_deinit(void)
{
	HAL_UART_DeInit(&huart2);
}

/*********************************************************************
* @brief  ������2��������
*---------------------------------------------------------------------
* @param  ��uint8_t *buf		,	��������	
*			uint16_t length		,	���ݳ���
*---------------------------------------------------------------------
* @retval ��none
*********************************************************************/
void bsp_usart2_send(uint8_t *buf, uint16_t length)
{
	HAL_UART_Transmit(&huart2, buf, length, 0xFFFF);
}

/*********************************************************************
* @brief  ������2�жϷ������
*********************************************************************/
void USART2_IRQHandler(void)
{
	volatile uint8_t res = 0;
	
	if((__HAL_UART_GET_FLAG(&huart2, UART_FLAG_RXNE) != RESET))
	{		
		res = (USART2->DR & (uint16_t)0x01FF);
	}
	
	HAL_UART_IRQHandler(&huart2);
}
/***************************************************************************************************/

/*********************************************************************
* @brief  ������ MspInit
*********************************************************************/
void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if(huart->Instance==USART1)
	{
		/* Peripheral clock enable */
		__HAL_RCC_USART1_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		
		/**USART1 GPIO Configuration
		PA9     ------> USART1_TX
		PA10     ------> USART1_RX
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_9;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_10;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		//USART1�жϺ����ȼ�����
		HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(USART1_IRQn);	
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
		GPIO_InitStruct.Pin = GPIO_PIN_2;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_3;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		//USART2�жϺ����ȼ�����
		HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(USART2_IRQn);	
	}
}

/*********************************************************************
* @brief  ������ MspDeInit
*********************************************************************/
void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{
	if(huart->Instance==USART1)
	{
		/* Peripheral clock disable */
		__HAL_RCC_USART1_CLK_DISABLE();

		/**USART1 GPIO Configuration
		PC4     ------> USART1_TX
		PC5     ------> USART1_RX
		*/
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);
		
		//����1�жϹر�
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
		
		//����2�жϹر�
		HAL_NVIC_DisableIRQ(USART2_IRQn);
	}
}

