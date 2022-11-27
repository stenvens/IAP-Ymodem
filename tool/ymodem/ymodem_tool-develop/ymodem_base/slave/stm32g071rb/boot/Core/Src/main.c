/********************************************************************************************************
*   项目名称 : IAP_Ymodem (Bootloader)
*   平    台 : STM32G071RBxx
*	作    者 : biglu
*	功能说明 : 
*				-1-	USART1(PA9/PA10)作为通讯串口,作为与上位机升级工具用。
* 
*				-2-	升级协议采用Ymodem，支持128Byte和1KByte。
*					Ymodem部分截取修改于ST的IAP例子。
*
*				-3-	分区修改位置：boot_interface.h。
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
* @brief  ：串口接收数据
*---------------------------------------------------------------------
* @param  ：uint8_t *buf		,		接收缓存区
*			uint16_t size		,		指定接收长度
*			uint32_t timeout	,		接收超时时间
*---------------------------------------------------------------------
* @retval ：0,接收超时			/		1,接收成功
*********************************************************************/
uint8_t uart_receive(uint8_t *buf, uint16_t size, uint32_t timeout)
{
	uint32_t tickstart = 0;

	tickstart = HAL_GetTick();
	
	for(uint16_t i=0; i<size; i++)
	{
		while(0 == LL_USART_IsActiveFlag_RXNE(USART2)) //等待RC标志
		{
			if((timeout == 0)||((HAL_GetTick() - tickstart ) > timeout))return 0; //超时
			
			//ORE错误判断
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
* @brief  ：串口发送数据
*---------------------------------------------------------------------
* @param  ：uint8_t *buf		,		接收缓存区
*			uint16_t size		,		指定接收长度
*---------------------------------------------------------------------
* @retval ：none
*********************************************************************/
void uart_send(uint8_t *buf, uint16_t size)
{
	bsp_usart2_send(buf, size);
}

/*********************************************************************
* @brief  ：外设初始化
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
* @brief  ：外设反初始化
*********************************************************************/
void bsp_deinit(void)
{
	//iap usart
	bsp_usart2_deinit();
	
	HAL_DeInitTick();
}

/*********************************************************************
* @brief  ：主函数
*********************************************************************/
int main(void)
{  	
	bsp_init();

	//Ymodem升级交互
    Main_Menu();
	
	while (1)
	{
		LL_mDelay(1000);
	}
}

