#include "board.h"

/**
  * @brief   ϵͳʱ�ӳ�ʼ��  
  * @note    none  
  * @param   none
  * @retval 
  */
static void systemcoclkInit(void)
{
	#ifdef STM32F103x

	#endif
}

/**
  * @brief   �����ʼ��  
  * @note    none  
  * @param   none
  * @retval 
  */
void board_init(void)
{
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		systemcoclkInit();
		SysTick_init(SYSTICK_FRE);
	  uart_gpio_init();
	  bsp_uartinit(115200);
#if CM_BACKTRACE == 0
		cm_backtrace_init("YMODEM_UPDATE","V1.0","V1.0");
#endif

}
