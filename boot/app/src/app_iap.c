#include "board.h"

typedef void (*pFunction)(void);
pFunction Jump_To_Application;


#ifdef STM32F103x
//	extern UART_HandleTypeDef UART_Handler;
#endif
/*******************************************************************************
Function Name : StartApp
Description   : 跳转至应用程序
Input         :
Output        :
Return        :
Other         :
*******************************************************************************/
void jump_to_app(void)
{
      uint32_t LJumpAddress;
    /* Test if user code is programmed starting from address "ApplicationAddress" */
    /* 堆栈指针指向RAM,RAM的首地址是0x20000000,而用户代码的首地址存放的是堆栈指针,
    如果相等说明一个正确的用户代码已经放在了用户代码区*/
    if(((*(__IO uint32_t*) APP_ADDRESS) & 0x2FFE0000) == 0x20000000)
    {
        /* Jump to user application */
        LJumpAddress = *(__IO uint32_t*) (APP_ADDRESS+4);
        Jump_To_Application = (pFunction) LJumpAddress;
        /* Initialize user application's Stack Pointer */
        __set_MSP(*(__IO uint32_t*) APP_ADDRESS);
		    /*跳转前先关闭中断*/
				__set_PRIMASK(1);
				/* 关闭滴答定时器，复位到默认值 */
				SysTick->CTRL = 0;
				SysTick->LOAD = 0;
				SysTick->VAL = 0;
				/* 关闭所有中断，清除所有中断挂起标志 */
				for (int i = 0; i < 8; i++)
				{
					NVIC->ICER[i]=0xFFFFFFFF;
					NVIC->ICPR[i]=0xFFFFFFFF;
				}
//				HAL_NVIC_DisableIRQ(UART_IRQ_NUM);
//				__HAL_UART_ENABLE_IT(&UART_Handler, UART_IT_RXNE);
				__set_PRIMASK(0);
				
				/* 在RTOS工程，这条语句很重要，设置为特权级模式，使用MSP指针 */
				__set_CONTROL(0);
        Jump_To_Application();      // 跳转到APP应用程序		
    }

}

