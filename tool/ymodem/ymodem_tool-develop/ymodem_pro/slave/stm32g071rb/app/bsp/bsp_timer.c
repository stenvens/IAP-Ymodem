#include "bsp_timer.h"
#include <stdio.h>


TIM_HandleTypeDef htim2;

/*********************************************************************
* @brief  ：通用定时器2中断初始化
*---------------------------------------------------------------------
* @param  ：uint16_t arr		,	自动重装值
*			uint16_t psc		,	时钟预分频数
*---------------------------------------------------------------------
* @retval ：none
*********************************************************************/
void bsp_timer2_init(uint16_t arr, uint16_t psc)
{
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	
	//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = psc; //分频系数
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP; //向上计数器
	htim2.Init.Period = arr; //自动装载值
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; //时钟分频因子
	//htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE; //禁止自动重装载
	HAL_TIM_Base_Init(&htim2);
	
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig);
  
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);
	
	HAL_TIM_Base_Start_IT(&htim2); //使能定时器2和定时器2更新中断：TIM_IT_UPDATE   
}

/*********************************************************************
* @brief  ：通用定时器2复位
*********************************************************************/
void bsp_timer2_deinit(void)
{
	HAL_TIM_Base_DeInit(&htim2);
}

/*********************************************************************
* @brief  ：通用定时器 MspInit
*********************************************************************/
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
	if(htim_base->Instance==TIM2)
	{
		/* Peripheral clock enable */
		__HAL_RCC_TIM2_CLK_ENABLE();
		
		HAL_NVIC_SetPriority(TIM2_IRQn, 1, 0); //设置中断优先级，抢占优先级1
		HAL_NVIC_EnableIRQ(TIM2_IRQn); //开启TIM6中断   
	}
}

/*********************************************************************
* @brief  ：通用定时器 MspDeInit
*********************************************************************/
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{
	if(htim_base->Instance==TIM2)
	{
		/* Peripheral clock disable */
		__HAL_RCC_TIM2_CLK_DISABLE();
		
		HAL_NVIC_DisableIRQ(TIM2_IRQn); //关闭TIM2中断   
	}
}

/*********************************************************************
* @brief  ：通用定时器中断
*********************************************************************/
void TIM2_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim2);
}

/*********************************************************************
* @brief  ：定时器回调
*********************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static uint16_t cnt = 0;
	
	if(htim == (&htim2))
    {
		if(++cnt == 10)
		{
			cnt = 0;
			printf("timer2\r\n");
		}
    }
}

