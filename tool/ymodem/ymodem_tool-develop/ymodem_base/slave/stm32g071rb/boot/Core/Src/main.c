/********************************************************************************************************
*   ��Ŀ���� : IAP_Ymodem (Bootloader)
*   ƽ    ̨ : STM32G071RBxx
*	��    �� : biglu
*	����˵�� : 
*				-1-	USART1(PA9/PA10)��ΪͨѶ����,��Ϊ����λ�����������á�
* 
*				-2-	����Э�����Ymodem��֧��128Byte��1KByte��
*					Ymodem���ֽ�ȡ�޸���ST��IAP���ӡ�
*
*				-3-	�����޸�λ�ã�boot_interface.h��
*
********************************************************************************************************/

//lib files
#include <string.h>
#include <stdio.h>
#include "main.h"
#include "hal_delay.h"

//bsp files
#include "bsp_uart.h"
#include "bsp_sysclk.h"
#include "bsp_flash.h"

//iap files
#include "menu.h"

/*********************************************************************
* @brief  �����ڽ�������
*---------------------------------------------------------------------
* @param  ��uint8_t *buf		,		���ջ�����
*			uint16_t size		,		ָ�����ճ���
*			uint32_t timeout	,		���ճ�ʱʱ��
*---------------------------------------------------------------------
* @retval ��0,���ճ�ʱ			/		1,���ճɹ�
*********************************************************************/
uint8_t uart_receive(uint8_t *buf, uint16_t size, uint32_t timeout)
{
	uint32_t tickstart = 0;

	tickstart = HAL_GetTick();
	
	for(uint16_t i=0; i<size; i++)
	{
		while(0 == LL_USART_IsActiveFlag_RXNE(USART2)) //�ȴ�RC��־
		{
			if((timeout == 0)||((HAL_GetTick() - tickstart ) > timeout))return 0; //��ʱ
			
			//ORE�����ж�
			__IO uint32_t isr_reg = LL_USART_ReadReg(USART2, ISR);
			if (isr_reg & LL_USART_ISR_ORE)
			{
				LL_USART_ClearFlag_ORE(USART2);
			}
		}
		buf[i] = LL_USART_ReceiveData8(USART2);
	}
		
	return 1;
}

/*********************************************************************
* @brief  �����ڷ�������
*---------------------------------------------------------------------
* @param  ��uint8_t *buf		,		���ջ�����
*			uint16_t size		,		ָ�����ճ���
*---------------------------------------------------------------------
* @retval ��none
*********************************************************************/
void uart_send(uint8_t *buf, uint16_t size)
{
	bsp_usart2_send(buf, size);
}

/*********************************************************************
* @brief  �������ʼ��
*********************************************************************/
void bsp_init(void)
{
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
	
	//Disable the internal Pull-Up in Dead Battery pins of UCPD peripheral
	LL_SYSCFG_DisableDBATT(LL_SYSCFG_UCPD1_STROBE | LL_SYSCFG_UCPD2_STROBE);
	
	/* Configure the system clock */
	bsp_sysclk_hsi_16_init();
	
	//systick delay init
	HAL_InitTick();
	
	//nucleo board io
	LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOF);
	
	//iap usart
	bsp_usart2_init();
}

/*********************************************************************
* @brief  �����跴��ʼ��
*********************************************************************/
void bsp_deinit(void)
{
	//iap usart
	bsp_usart2_deinit();
	
	HAL_DeInitTick();
}

/*********************************************************************
* @brief  ��������
*********************************************************************/
int main(void)
{  	
	bsp_init();

	//Ymodem��������
    Main_Menu();
	
	while (1)
	{
		LL_mDelay(1000);
	}
}

