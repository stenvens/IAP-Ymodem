#include "bsp_timer.h"
#include <stdio.h>


TIM_HandleTypeDef htim1;

/*********************************************************************
* @brief  ��ͨ�ö�ʱ��1�жϳ�ʼ��
*---------------------------------------------------------------------
* @param  ��uint16_t arr		,	�Զ���װֵ
*			uint16_t psc		,	ʱ��Ԥ��Ƶ��
*---------------------------------------------------------------------
* @retval ��none
*********************************************************************/
void bsp_timer1_init(uint16_t arr, uint16_t psc)
{
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	
	//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = psc; //��Ƶϵ��
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP; //���ϼ�����
	htim1.Init.Period = arr; //�Զ�װ��ֵ
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; //ʱ�ӷ�Ƶ����
	//htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE; //��ֹ�Զ���װ��
	HAL_TIM_Base_Init(&htim1);
	
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig);
  
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig);
	
	HAL_TIM_Base_Start_IT(&htim1); //ʹ�ܶ�ʱ��2�Ͷ�ʱ��2�����жϣ�TIM_IT_UPDATE   
}

/*********************************************************************
* @brief  ��ͨ�ö�ʱ��1��λ
*********************************************************************/
void bsp_timer1_deinit(void)
{
	HAL_TIM_Base_DeInit(&htim1);
}

/*********************************************************************
* @brief  ��ͨ�ö�ʱ�� MspInit
*********************************************************************/
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{
	if(htim_base->Instance==TIM1)
	{
		/* Peripheral clock enable */
		__HAL_RCC_TIM1_CLK_ENABLE();
		
		HAL_NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 1, 0); //�����ж����ȼ�����ռ���ȼ�1
		HAL_NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn); //����TIM1�ж�   
	}
}

/*********************************************************************
* @brief  ��ͨ�ö�ʱ�� MspDeInit
*********************************************************************/
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{
	if(htim_base->Instance==TIM1)
	{
		/* Peripheral clock disable */
		__HAL_RCC_TIM1_CLK_DISABLE();
		
		HAL_NVIC_DisableIRQ(TIM1_BRK_UP_TRG_COM_IRQn); //�ر�TIM1�ж�   
	}
}

/*********************************************************************
* @brief  ��ͨ�ö�ʱ���ж�
*********************************************************************/
void TIM1_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim1);
}

/*********************************************************************
* @brief  ����ʱ���ص�
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

