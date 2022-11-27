#include "config.h"
#include "FreeRTOS.h"
#include "task.h"

static volatile uint32_t tickcnt = 0;
extern void xPortSysTickHandler(void);
//void  delay_ms(uint32_t nms)
//{
//  uint32_t time_cur = tickcnt;
//	while((tickcnt - time_cur) < nms)
//	{}

//}

/**
  * @brief   获取计数值  
  * @note    none  
  * @param   none
  * @retval 
  */
uint32_t  get_tickcnt(void)
{

	return tickcnt;

}

/**
  * @brief   滴答定时器初始化  
  * @note    none  
  * @param   none
  * @retval 
  */
uint32_t SysTick_init(uint32_t ticks)
{
#ifdef STM32F103x
    RCC_ClocksTypeDef RCC_Clocks;
		RCC_GetClocksFreq(&RCC_Clocks);
#endif
	uint32_t systick;
	systick = (RCC_Clocks.HCLK_Frequency / (1000U));
  if ((ticks - 1UL) > SysTick_LOAD_RELOAD_Msk)
  {
    return (1UL);                                                   /* Reload value impossible */
  }

  SysTick->LOAD  = (uint32_t)(systick - 1UL);                         /* set reload register */
  NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL); /* set Priority for Systick Interrupt */
  SysTick->VAL   = 0UL;                                             /* Load the SysTick Counter Value */
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                   SysTick_CTRL_TICKINT_Msk   |
                   SysTick_CTRL_ENABLE_Msk;                         /* Enable SysTick IRQ and SysTick Timer */
  return (0UL);                                                     /* Function successful */
}

//systick中断服务函数
void SysTick_Handler(void)
{	
    if(xTaskGetSchedulerState()!=taskSCHEDULER_NOT_STARTED)//系统已经运行
    {
        xPortSysTickHandler();	
    }
}

