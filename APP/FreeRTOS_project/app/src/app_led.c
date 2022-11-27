#include "config.h"
#include "FreeRTOS.h"
#include "task.h"
#include "protocol.h"
//LED0任务函数 
void led0_task(void *pvParameters)
{
    while(1)
    {
        GPIO_SetBits(LED0_PORT,LED0_PIN);
        vTaskDelay(500);
        GPIO_ResetBits(LED0_PORT,LED0_PIN);
        vTaskDelay(500);
    }
}   

//LED1任务函数
void led1_task(void *pvParameters)
{
    while(1)
    {
				pro_dis();
        GPIO_SetBits(LED1_PORT,LED1_PIN);
        vTaskDelay(500);
        GPIO_ResetBits(LED1_PORT,LED1_PIN);
        vTaskDelay(500);
    }
}
