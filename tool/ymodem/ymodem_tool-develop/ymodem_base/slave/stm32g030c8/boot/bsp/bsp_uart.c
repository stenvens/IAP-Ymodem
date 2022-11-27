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
    while (!LL_USART_IsActiveFlag_TXE(USART2))
    {
    }
	USART2->TDR = (uint8_t) ch;
	
    return ch;
}
#endif
/***************************************************************************************************/


/********************************************** ����1 **********************************************/
uint8_t usart1_rx_buf[USART1_BUF_MAXIMUM] = {0}; //USART1���ջ�����
uint16_t usart1_rx_cnt = 0; //USART1��ǰ���ճ���

/*********************************************************************
* @brief  ������1��ʼ��
*********************************************************************/
void bsp_usart1_init(void)
{
	LL_USART_InitTypeDef USART_InitStruct = {0};
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* Peripheral clock enable */
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);

	/**USART1 GPIO Configuration
	PB6   ------> USART1_TX
	PB7   ------> USART1_RX
	*/
	GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LL_GPIO_PIN_7;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
	LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* USART1 interrupt Init */
//	NVIC_SetPriority(USART1_IRQn, 0);
//	NVIC_EnableIRQ(USART1_IRQn);

	USART_InitStruct.PrescalerValue = LL_USART_PRESCALER_DIV1;
	USART_InitStruct.BaudRate = USART1_BAUD;
	USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
	USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
	USART_InitStruct.Parity = LL_USART_PARITY_NONE;
	USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
	USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
	USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
	LL_USART_Init(USART1, &USART_InitStruct);
	LL_USART_SetTXFIFOThreshold(USART1, LL_USART_FIFOTHRESHOLD_1_8);
	LL_USART_SetRXFIFOThreshold(USART1, LL_USART_FIFOTHRESHOLD_1_8);
	LL_USART_DisableFIFO(USART1);
	LL_USART_ConfigAsyncMode(USART1);

	LL_USART_Enable(USART1);

	/* Polling USART1 initialisation */
	while((!(LL_USART_IsActiveFlag_TEACK(USART1))) || (!(LL_USART_IsActiveFlag_REACK(USART1))))
	{
	}

	/* Clear Overrun flag, in case characters have already been sent to USART */
	LL_USART_ClearFlag_ORE(USART1);

//	/* Enable RXNE and Error interrupts */
//	LL_USART_EnableIT_RXNE(USART1);
//	LL_USART_EnableIT_ERROR(USART1);
}

/*********************************************************************
* @brief  ������1��λ
*********************************************************************/
void bsp_usart1_deinit(void)
{
	NVIC_DisableIRQ(USART1_IRQn);
	LL_USART_DeInit(USART1);
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
	uint16_t index = 0;

	for (index = 0; index < length; index++)
	{
		/* Wait for TXE flag to be raised */
		while (!LL_USART_IsActiveFlag_TXE(USART1)){}

		/* Write character in Transmit Data register.
		   TXE flag is cleared by writing data in TDR register */
		LL_USART_TransmitData8(USART1, *buf++);
	}

	/* Wait for TC flag to be raised for last char */
	while (!LL_USART_IsActiveFlag_TC(USART1)){}
}

/*********************************************************************
* @brief  ������1�ж��쳣����
*********************************************************************/
static void USART1_Error_Callback(void)
{
	__IO uint32_t isr_reg;

	/* Disable USARTx_IRQn */
	NVIC_DisableIRQ(USART1_IRQn);

	/* Error handling example :
	- Read USART ISR register to identify flag that leads to IT raising
	- Perform corresponding error handling treatment according to flag
	*/
	isr_reg = LL_USART_ReadReg(USART1, ISR);
	if (isr_reg & LL_USART_ISR_NE)
	{
		/* case Noise Error flag is raised : Clear NF Flag */
		LL_USART_ClearFlag_NE(USART1);
	}
	else
	{
		/* Unexpected IT source */
	}
}

/*********************************************************************
* @brief  ������1�жϷ������
*********************************************************************/
void USART1_IRQHandler(void)
{
	uint8_t res = 0;

	if (LL_USART_IsActiveFlag_RXNE(USART1) && LL_USART_IsEnabledIT_RXNE(USART1))
	{
		res = LL_USART_ReceiveData8(USART1);
		usart1_rx_buf[usart1_rx_cnt++] = res;
		if(usart1_rx_cnt >= USART1_BUF_MAXIMUM)
		{
			usart1_rx_cnt = 0;
		}
	}
	else
	{
		USART1_Error_Callback();
	}
}
/***************************************************************************************************/


/********************************************** ����2 **********************************************/
uint8_t usart2_rx_buf[USART2_BUF_MAXIMUM] = {0}; //USART2���ջ�����
uint16_t usart2_rx_cnt = 0; //USART2��ǰ���ճ���

/*********************************************************************
* @brief  ������2��ʼ��
*********************************************************************/
void bsp_usart2_init(void)
{
	LL_USART_InitTypeDef USART_InitStruct = {0};
	LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	/* Peripheral clock enable */
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);

	/**USART2 GPIO Configuration
	PA2   ------> USART2_TX
	PA3   ------> USART2_RX
	*/
	GPIO_InitStruct.Pin = LL_GPIO_PIN_2;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LL_GPIO_PIN_3;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
	GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/* USART2 interrupt Init */
	NVIC_SetPriority(USART2_IRQn, 0);
	NVIC_EnableIRQ(USART2_IRQn);

	USART_InitStruct.PrescalerValue = LL_USART_PRESCALER_DIV1;
	USART_InitStruct.BaudRate = USART2_BAUD;
	USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
	USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
	USART_InitStruct.Parity = LL_USART_PARITY_NONE;
	USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
	USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
	USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
	LL_USART_Init(USART2, &USART_InitStruct);
	LL_USART_SetTXFIFOThreshold(USART2, LL_USART_FIFOTHRESHOLD_1_8);
	LL_USART_SetRXFIFOThreshold(USART2, LL_USART_FIFOTHRESHOLD_1_8);
	LL_USART_DisableFIFO(USART2);
	LL_USART_ConfigAsyncMode(USART2);

	LL_USART_Enable(USART2);

	/* Polling USART2 initialisation */
	while((!(LL_USART_IsActiveFlag_TEACK(USART2))) || (!(LL_USART_IsActiveFlag_REACK(USART2))))
	{
	}

	/* Clear Overrun flag, in case characters have already been sent to USART */
	LL_USART_ClearFlag_ORE(USART2);

//	/* Enable RXNE and Error interrupts */
//	LL_USART_EnableIT_RXNE(USART2);
//	LL_USART_EnableIT_ERROR(USART2);
}

/*********************************************************************
* @brief  ������2��λ
*********************************************************************/
void bsp_usart2_deinit(void)
{
	NVIC_DisableIRQ(USART2_IRQn);
	LL_USART_DeInit(USART2);
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
	uint16_t index = 0;

	for (index = 0; index < length; index++)
	{
		/* Wait for TXE flag to be raised */
		while (!LL_USART_IsActiveFlag_TXE(USART2)){}

		/* Write character in Transmit Data register.
		TXE flag is cleared by writing data in TDR register */
		LL_USART_TransmitData8(USART2, *buf++);
	}

	/* Wait for TC flag to be raised for last char */
	while (!LL_USART_IsActiveFlag_TC(USART2)){}
}

/*********************************************************************
* @brief  ������2�ж��쳣����
*********************************************************************/
static void USART2_Error_Callback(void)
{
	__IO uint32_t isr_reg;

	/* Disable USARTx_IRQn */
	NVIC_DisableIRQ(USART2_IRQn);

	/* Error handling example :
	- Read USART ISR register to identify flag that leads to IT raising
	- Perform corresponding error handling treatment according to flag
	*/
	isr_reg = LL_USART_ReadReg(USART2, ISR);
	if (isr_reg & LL_USART_ISR_NE)
	{
		/* case Noise Error flag is raised : Clear NF Flag */
		LL_USART_ClearFlag_NE(USART2);
	}
	else
	{
		/* Unexpected IT source */
	}
}

/*********************************************************************
* @brief  ������2�жϷ������
*********************************************************************/
void USART2_IRQHandler(void)
{
	uint8_t res = 0;

	if (LL_USART_IsActiveFlag_RXNE(USART2) && LL_USART_IsEnabledIT_RXNE(USART2))
	{
		res = LL_USART_ReceiveData8(USART2);
		usart2_rx_buf[usart2_rx_cnt++] = res;
		if(usart2_rx_cnt >= USART2_BUF_MAXIMUM)
		{
			usart2_rx_cnt = 0;
		}
	}
	else
	{
		USART2_Error_Callback();
	}
}
/***************************************************************************************************/

