#include "FreeRTOS.h"
#include "task.h"
#include "app_led.h"
#include "app_task.h"

//任务优先级
#define LED0_TASK_PRIO		2
//任务堆栈大小	
#define LED0_STK_SIZE 		50  
//任务句柄
TaskHandle_t LED0Task_Handler;
//任务函数
void led0_task(void *pvParameters);

//任务优先级
#define LED1_TASK_PRIO		3
//任务堆栈大小	
#define LED1_STK_SIZE 		50  
//任务句柄
TaskHandle_t LED1Task_Handler;
//任务函数
void led1_task(void *pvParameters);



extern TaskHandle_t App_StartTask_Handler;
/**
  * @brief   任务创建   
  * @note    所有的任务都放在这里，方便管理  
  * @param   none
  * @retval 
  */
void AppTaskCreate(void)
{
  BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
  
  taskENTER_CRITICAL();           //进入临界区
	//创建LED0任务
	xReturn = xTaskCreate((TaskFunction_t )led0_task,     	
							(const char*    )"led0_task",   	
							(uint16_t       )LED0_STK_SIZE, 
							(void*          )NULL,				
							(UBaseType_t    )LED0_TASK_PRIO,	
							(TaskHandle_t*  )&LED0Task_Handler);  
  if(pdPASS == xReturn)
    printf("创建led0_task任务成功!\r\n");		
	
	//创建LED1任务
	xReturn = xTaskCreate((TaskFunction_t )led1_task,     
							(const char*    )"led1_task",   
							(uint16_t       )LED1_STK_SIZE, 
							(void*          )NULL,
							(UBaseType_t    )LED1_TASK_PRIO,
							(TaskHandle_t*  )&LED1Task_Handler); 
  if(pdPASS == xReturn)
    printf("创建led1_task任务成功!\r\n");
							
	vTaskDelete(App_StartTask_Handler); //删除App_StartTask_Handler任务
  							
	taskEXIT_CRITICAL();            //退出临界区
}


