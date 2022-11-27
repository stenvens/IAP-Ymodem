#include "bsp_timer.h"
#include "FreeRTOS.h"
#include "task.h"

static volatile uint32_t tim4cnt = 0;
/**
  * @brief  定时器4初始化   
  * @note    none  
  * @param   none
  * @retval 
  */
static void bsp_timer4_init(uint32_t pre,uint32_t arr)
{
	
//  TIM_MasterConfigTypeDef sMasterConfig;
//	__HAL_RCC_TIM4_CLK_ENABLE();
//	
//	htim4.Instance = TIM4;
//  htim4.Init.Prescaler = pre-1;
//  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
//  htim4.Init.Period = arr-1;
//  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
//  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
//  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
//  {
//    printf("定时器4初始化失败\n");
//  }
//	
//	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
//  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
//  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
//  {
//     printf("定时器4初始化失败\n");
//  }
//	HAL_NVIC_SetPriority(TIM4_IRQn, 4, 0);
//	HAL_NVIC_EnableIRQ(TIM4_IRQn);
////	HAL_TIM_Base_Start(&htim4);
//	__HAL_TIM_ENABLE_IT(&htim4,TIM_IT_UPDATE);
//	bsp_timer_enable();
}

/**
  * @brief  定时器初始化   
  * @note    none  
  * @param   none
  * @retval 
  */
void bsp_timer_init(void)
{
	bsp_timer4_init(10,7200);
}


/**
  * @brief  使能定时器
  * @note   none  
  * @param  none
  * @retval 
  */
void bsp_timer_enable(void)
{

}

/**
  * @brief  失能定时器 
  * @note   none  
  * @param  none
  * @retval 
  */
void bsp_timer_disable(void)
{

}


/**
* @brief This function handles TIM4 global interrupt.
*/
void TIM4_IRQHandler(void)
{
//  /* USER CODE BEGIN TIM4_IRQn 0 */
//   if(__HAL_TIM_GET_IT_SOURCE(&htim4,TIM_IT_UPDATE) != RESET )
//	 {
//	    
//			tim4cnt++;
//		  if(tim4cnt == 10000)
//			{		
//			  printf("时间:%d秒\n",(tim4cnt));	
//				tim4cnt = 0;
//			
//			}
//			__HAL_TIM_CLEAR_IT(&htim4,TIM_IT_UPDATE);
//						
//   }
}
