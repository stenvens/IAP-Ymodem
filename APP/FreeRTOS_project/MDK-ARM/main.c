#include "config.h"
#include "FreeRTOS.h"
#include "task.h"
#include "app_task.h"

extern uint32_t Image$$ER_IROM1$$Base;

//�������ȼ�
#define APP_START_TASK_PRIO		1
//�����ջ��С	
#define APP_START_STK_SIZE 		128  
//������
TaskHandle_t App_StartTask_Handler;
//������
void AppTaskCreate(void *pvParameters);


int main(void)
{
//	NVIC_SetVector(FLASH_BASE,(uint32_t)&Image$$ER_IROM1$$Base-FLASH_BASE;);
  BaseType_t xReturn = pdPASS;//����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS
	SysTick_init(SYSTICK_FRE);
	bsp_gpio_init();						//GPIO��ʼ��
	bsp_uartinit(115200);				//��ʼ������
	bsp_timer_init();
	printf("FreeRTOS111 Test!\n");
	/*������ʼ����*/
   xReturn =  xTaskCreate((TaskFunction_t )AppTaskCreate,   //������
                (const char*    )"AppTaskCreate",          //��������
                (uint16_t       )APP_START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  		//���ݸ��������Ĳ���
                (UBaseType_t    )APP_START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&App_StartTask_Handler);   //������              
  /* ����������� */           
  if(pdPASS == xReturn)
	{
    printf("�����������\n");
		vTaskStartScheduler();   //�������񣬿������� 
	}
	else
	{
		return -1;
	}
      
  /* ��������ִ�е����� */  
  while(1);   
}
