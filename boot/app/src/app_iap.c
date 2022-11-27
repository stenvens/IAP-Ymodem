#include "board.h"

typedef void (*pFunction)(void);
pFunction Jump_To_Application;


#ifdef STM32F103x
//	extern UART_HandleTypeDef UART_Handler;
#endif
/*******************************************************************************
Function Name : StartApp
Description   : ��ת��Ӧ�ó���
Input         :
Output        :
Return        :
Other         :
*******************************************************************************/
void jump_to_app(void)
{
      uint32_t LJumpAddress;
    /* Test if user code is programmed starting from address "ApplicationAddress" */
    /* ��ջָ��ָ��RAM,RAM���׵�ַ��0x20000000,���û�������׵�ַ��ŵ��Ƕ�ջָ��,
    ������˵��һ����ȷ���û������Ѿ��������û�������*/
    if(((*(__IO uint32_t*) APP_ADDRESS) & 0x2FFE0000) == 0x20000000)
    {
        /* Jump to user application */
        LJumpAddress = *(__IO uint32_t*) (APP_ADDRESS+4);
        Jump_To_Application = (pFunction) LJumpAddress;
        /* Initialize user application's Stack Pointer */
        __set_MSP(*(__IO uint32_t*) APP_ADDRESS);
		    /*��תǰ�ȹر��ж�*/
				__set_PRIMASK(1);
				/* �رյδ�ʱ������λ��Ĭ��ֵ */
				SysTick->CTRL = 0;
				SysTick->LOAD = 0;
				SysTick->VAL = 0;
				/* �ر������жϣ���������жϹ����־ */
				for (int i = 0; i < 8; i++)
				{
					NVIC->ICER[i]=0xFFFFFFFF;
					NVIC->ICPR[i]=0xFFFFFFFF;
				}
//				HAL_NVIC_DisableIRQ(UART_IRQ_NUM);
//				__HAL_UART_ENABLE_IT(&UART_Handler, UART_IT_RXNE);
				__set_PRIMASK(0);
				
				/* ��RTOS���̣�����������Ҫ������Ϊ��Ȩ��ģʽ��ʹ��MSPָ�� */
				__set_CONTROL(0);
        Jump_To_Application();      // ��ת��APPӦ�ó���		
    }

}

