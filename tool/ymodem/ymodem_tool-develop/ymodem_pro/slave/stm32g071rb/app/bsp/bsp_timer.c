#include "bsp_timer.h"
#include <stdio.h>


TIM_HandleTypeDef htim2;

/*********************************************************************
* @brief  ��ͨ�ö�ʱ��2�жϳ�ʼ��
*---------------------------------------------------------------------
* @param  ��uint16_t arr		,	�Զ���װֵ
*			uint16_t psc		,	ʱ��Ԥ��Ƶ��
*---------------------------------------------------------------------
* @retval ��none
*********************************************************************/
void bsp_timer2_init(uint16_t arr, uint16_t psc)
{
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	
	//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = psc; //��Ƶϵ��
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP; //���ϼ�����
	htim2.Init.Period = arr; //�Զ�װ��ֵ
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; //ʱ�ӷ�Ƶ����
	//htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE; //��ֹ�Զ���װ��
	HAL_TIM_Base_Init(&htim2);
	
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig);
  
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig);
	
	HAL_TIM_Base_Start_IT(&htim2); //ʹ�ܶ�ʱ��2�Ͷ�ʱ��2�����жϣ�TIM_IT_UPDATE   
}

/*********************************************************************
* @brief  ��ͨ�ö�ʱ��2��λ
*********************************************************************/
void bsp_timer2_deinit(void)
{
	HAL_TIM_Base_DeInit(&htim2);
}

/*********************************************************************
* @brief  ��ͨ�ö�ʱ�� MspInit
*********************************************************************/
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
	if(htim_base->Instance==TIM2)
	{
		/* Peripheral clock enable */
		__HAL_RCC_TIM2_CLK_ENABLE();
		
		HAL_NVIC_SetPriority(TIM2_IRQn, 1, 0); //�����ж����ȼ�����ռ���ȼ�1
		HAL_NVIC_EnableIRQ(TIM2_IRQn); //����TIM6�ж�   
	}
}

/*********************************************************************
* @brief  ��ͨ�ö�ʱ�� MspDeInit
*********************************************************************/
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{
	if(htim_base->Instance==TIM2)
	{
		/* Peripheral clock disable */
		__HAL_RCC_TIM2_CLK_DISABLE();
		
		HAL_NVIC_DisableIRQ(TIM2_IRQn); //�ر�TIM2�ж�   
	}
}

/*********************************************************************
* @brief  ��ͨ�ö�ʱ���ж�
*********************************************************************/
void TIM2_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim2);
}

/*********************************************************************
* @brief  ����ʱ���ص�
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

