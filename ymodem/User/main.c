/*
*********************************************************************************************************
*
*	ģ������ : ������ģ��
*	�ļ����� : main.c
*	��    �� : V1.0
*	˵    �� : ���ڹ̼���¼
*              ʵ��Ŀ�ģ�
*                1. ѧϰ���ڹ̼���¼��
*              ע�����
*                1. ��ʵ���Ƽ�ʹ�ô�������SecureCRT����H7-TOOL��λ�������鿴��ӡ��Ϣ��������115200������λ8����żУ��λ�ޣ�ֹͣλ1��
*                2. ��ؽ��༭��������������TAB����Ϊ4���Ķ����ļ���Ҫ��������ʾ�����롣
*
*	�޸ļ�¼ :
*		�汾��   ����         ����        ˵��
*		V1.0    2022-06-15   Eric2013     1. CMSIS�����汾 V5.7.0
*                                         2. HAL��汾 V1.9.0
*                                         3. FatFS�汾V0.12c
*                                         
*	Copyright (C), 2020-2030, ���������� www.armfly.com
*
*********************************************************************************************************
*/	
#include "bsp.h"			 /* �ײ�Ӳ������ */



extern void DemoUartUpdate(void);


/*
*********************************************************************************************************
*	�� �� ��: main
*	����˵��: c�������
*	��    ��: ��
*	�� �� ֵ: �������(���账��)
*********************************************************************************************************
*/
int main(void)
{
	bsp_Init();		/* Ӳ����ʼ�� */
	
	DemoUartUpdate();    /* SD������ */
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/