#include "bsp_timer.h"
#include <stdio.h>


TIM_HandleTypeDef htim1;

/*********************************************************************
* @brief  ：通用定时器1中断初始化
*---------------------------------------------------------------------
* @param  ：uint16_t arr		,	自动重装值
*			uint16_t psc		,	时钟预分频数
*---------------------------------------------------------------------
* @retval ：none
*********************************************************************/
void bsp_timer1_init(uint16_t arr, uint16_t psc)
{
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	
	//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = psc; //分频系数
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP; //向上计数器
	htim1.Init.Period = arr; //自动装载值
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; //时钟分频因子
	//htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE; //禁止自动重装载
	HAL_TIM_Base_Init(&htim1);
	
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig);
  
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig);
	
	HAL_TIM_Base_Start_IT(&htim1); //使能定时器2和定时器2更新中断：TIM_IT_UPDATE   
}

/*********************************************************************
* @brief  ：通用定时器1复位
*********************************************************************/
void bsp_timer1_deinit(void)
{
	HAL_TIM_Base_DeInit(&htim1);
}

/*********************************************************************
* @brief  ：通用定时器 MspInit
*********************************************************************/
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
	if(htim_base->Instance==TIM1)
	{
		/* Peripheral clock enable */
		__HAL_RCC_TIM1_CLK_ENABLE();
		
		HAL_NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 1, 0); //设置中断优先级，抢占优先级1
		HAL_NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn); //开启TIM1中断   
	}
}

/*********************************************************************
* @brief  ：通用定时器 MspDeInit
*********************************************************************/
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{
	if(htim_base->Instance==TIM1)
	{
		/* Peripheral clock disable */
		__HAL_RCC_TIM1_CLK_DISABLE();
		
		HAL_NVIC_DisableIRQ(TIM1_BRK_UP_TRG_COM_IRQn); //关闭TIM1中断   
	}
}

/*********************************************************************
* @brief  ：通用定时器中断
*********************************************************************/
void TIM1_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim1);
}

/*********************************************************************
* @brief  ：定时器回调
*********************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	static uint16_t cnt = 0;
	
	if(htim == (&htim1))
    {
		if(++cnt == 10)
		{
			cnt = 0;
			printf("timer1\r\n");
		}
    }
}

