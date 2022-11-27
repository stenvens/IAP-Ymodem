#include "FreeRTOS.h"
#include "task.h"
#include "app_led.h"
#include "app_task.h"

//�������ȼ�
#define LED0_TASK_PRIO		2
//�����ջ��С	
#define LED0_STK_SIZE 		50  
//������
TaskHandle_t LED0Task_Handler;
//������
void led0_task(void *pvParameters);

//�������ȼ�
#define LED1_TASK_PRIO		3
//�����ջ��С	
#define LED1_STK_SIZE 		50  
//������
TaskHandle_t LED1Task_Handler;
//������
void led1_task(void *pvParameters);



extern TaskHandle_t App_StartTask_Handler;
/**
  * @brief   ���񴴽�   
  * @note    ���е����񶼷�������������  
  * @param   none
  * @retval 
  */
void AppTaskCreate(void)
{
  BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
  
  taskENTER_CRITICAL();           //�����ٽ���
	//����LED0����
	xReturn = xTaskCreate((TaskFunction_t )led0_task,     	
							(const char*    )"led0_task",   	
							(uint16_t       )LED0_STK_SIZE, 
							(void*          )NULL,				
							(UBaseType_t    )LED0_TASK_PRIO,	
							(TaskHandle_t*  )&LED0Task_Handler);  
  if(pdPASS == xReturn)
    printf("����led0_task����ɹ�!\r\n");		
	
	//����LED1����
	xReturn = xTaskCreate((TaskFunction_t )led1_task,     
							(const char*    )"led1_task",   
							(uint16_t       )LED1_STK_SIZE, 
							(void*          )NULL,
							(UBaseType_t    )LED1_TASK_PRIO,
							(TaskHandle_t*  )&LED1Task_Handler); 
  if(pdPASS == xReturn)
    printf("����led1_task����ɹ�!\r\n");
							
	vTaskDelete(App_StartTask_Handler); //ɾ��App_StartTask_Handler����
  							
	taskEXIT_CRITICAL();            //�˳��ٽ���
}


