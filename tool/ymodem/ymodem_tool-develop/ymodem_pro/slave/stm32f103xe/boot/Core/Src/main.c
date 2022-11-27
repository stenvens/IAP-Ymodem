/********************************************************************************************************
*   项目名称 : IAP_Ymodem_pro (Bootloader)
*   平    台 : STM32F103xE
*	作    者 : biglu
*	功能说明 : 
*				-1-	USART1(PA9/PA10)作为通讯串口,作为与上位机升级工具用。
* 
*				-2-	升级协议采用Ymodem，支持128Byte和1KByte。
*					Ymodem部分截取修改于ST的IAP例子。
*
*				-3-	分区修改位置：boot_interface.h。
*
*				-4-	上电后600ms内若收到升级信号'st'，则进入Ymodem升级流程。
*					若未收到信号，则进行跳转判断。
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
#include "boot_interface.h"
#include "boot_param.h"
#include "boot_iap.h"


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
		while(0 == LL_USART_IsActiveFlag_RXNE(USART1)) //等待RC标志
		{
			if((timeout == 0)||((HAL_GetTick() - tickstart ) > timeout))return 0; //超时
			
			//ORE错误判断
			__IO uint32_t isr_reg = LL_USART_ReadReg(USART1, SR);
			if (isr_reg & LL_USART_SR_ORE)
			{
				LL_USART_ClearFlag_ORE(USART1);
			}
		}
		buf[i] = LL_USART_ReceiveData8(USART1);
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
	bsp_usart1_send(buf, size);
}

/*********************************************************************
* @brief  ：外设初始化
*********************************************************************/
void bsp_init(void)
{
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

	NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);
	
	LL_GPIO_AF_Remap_SWJ_NOJTAG();
	
	/* Configure the system clock */
	SystemClock_Config();
	
	//systick delay init
	HAL_InitTick();
	
	//iap usart
	bsp_usart1_init();
}

/*********************************************************************
* @brief  ：外设反初始化
*********************************************************************/
void bsp_deinit(void)
{
	//iap usart
	bsp_usart1_deinit();
	
	HAL_DeInitTick();
}

/*********************************************************************
* @brief  ：确认跳转条件
*********************************************************************/
void check_jump_condition(void)
{
	/*
	确认跳转APP条件:
	1. 未与升级工具通讯上，app未曾被擦写过，但有合法的app --> 跳转到app
	2. 未与升级工具通讯上，app未曾被擦写过，但没有合法的app --> 不跳转，等待
	3. 未与升级工具通讯上，app被擦写过 --> 不跳转，等待
	*/
	if(boot_interface_check_app_erased() == true) //如果从来没升级过 (即第一次烧录大HEX文件或只烧录了Bootloader)
	{    
		if(boot_interface_check_app_existed() == true) //跳转条件1
		{   
			printf("app exist, jump to app... (app never erased & valid app)\r\n");
			bsp_deinit();
			__disable_irq(); //关闭所有中端
			iap_load_app(BLOCK_APP1_START);
		}
		else //跳转条件2
		{
			printf("app not exist, wait... (app never erased & no valid app)\r\n");
		}
	}
	else //跳转条件3
	{
		printf("app not exist, wait... (app erased)\r\n");
	}
}

/*********************************************************************
* @brief  ：确认升级信号
*---------------------------------------------------------------------
* @retval ：0,未收到		/		1,收到	
*********************************************************************/
uint8_t check_update_signal(void)
{
	uint8_t buf[2] = {0};
	
	if(uart_receive(buf, 2, 600) == 1)
	{
		if(buf[0] == 's' && buf[1] == 't')
		{
			return 1;
		}
	}
	
	return 0;
}

/*********************************************************************
* @brief  ：等待重启信号
*---------------------------------------------------------------------
* @param  ：none
*---------------------------------------------------------------------
* @retval ：0	,	一定时间内未收到重启信号
*			1	,	收到重启信号
*********************************************************************/
uint8_t wait_reboot_signal(void)
{
	uint8_t ret = 0;
	uint8_t cnt = 0;
	uint8_t char1 = 0;
	char *dest_buf = "reboot";
	
	while(cnt < strlen(dest_buf))
	{
		if(uart_receive(&char1, 1, 50) == 1 && char1 == dest_buf[cnt])
		{
			ret = 1;
		}
		else
		{
			ret = 0;
			break;
		}
		
		cnt++;
	}
	
	return ret;
}

/*********************************************************************
* @brief  ：主函数
*********************************************************************/
int main(void)
{  	
	bsp_init();
	boot_param_read();
	
	//等待升级信号
	if(check_update_signal() == 0)
	{
		//无升级新号，则确认跳转条件
		check_jump_condition();
	}

	//Ymodem升级交互
	Main_Menu();
	
	while (1)
	{
		//等待重启信号以再次进入升级模式
		if(wait_reboot_signal() == 1)
		{
			iap_reboot();
		}
	}
}

