/********************************************************************************************************
*   项目名称 : IAP_Ymodem_base (Bootloader)
*   平    台 : HC32F030J8TA
*	作    者 : biglu
*	功能说明 : 
*				-1-	USART1(PA2/PA3)作为通讯串口,作为与上位机升级工具用。
* 
*				-2-	升级协议采用Ymodem，支持128Byte和1KByte。
*					Ymodem部分截取修改于ST的IAP例子。
*
*				-3-	分区修改位置：boot_interface.h。
*
********************************************************************************************************/

//lib files
#include "ddl.h"
#include "flash.h"
#include "hal_delay.h"

//bsp files
#include "bsp_uart.h"

//iap files
#include "menu.h"


//函数声明
extern void FlashInt(void);

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
		while(0 == M0P_UART1->ISR_f.RC) //等待RC标志
		{
			if((timeout == 0)||((HAL_GetTick() - tickstart ) > timeout))return 0; //超时
		}
		M0P_UART1->ICR_f.RCCF = 0; //清除RC标志
		buf[i] = M0P_UART1->SBUF_f.DATA;
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
	bsp_uart1_send(buf, size);
}

/*********************************************************************
* @brief  ：外设初始化
*********************************************************************/
void bsp_init(void)
{
	//上电默认4M-内部RC
	Flash_Init(FlashInt, 1, TRUE); //FLASH初始化
	
	//初始化滴答时钟源
	HAL_InitTick();
	
	//IAP串口
	bsp_uart1_init(); 
	
	//开中断
	__enable_irq();
}

/*********************************************************************
* @brief  ：外设反初始化
*********************************************************************/
void bsp_deinit(void)
{
	//iap usart
	bsp_uart1_deinit();
	
	HAL_DeInitTick();
}

int32_t main(void)
{
	//初始化
	bsp_init();

	//Ymodem升级交互
    Main_Menu();
	
    while(1)
	{
		HAL_Delay(1000);
	}
}

