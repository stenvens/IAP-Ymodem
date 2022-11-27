/* BSPͷ�ļ� */
#include "bsp_wdt.h"

/* �ٷ���ͷ�ļ� */
#include "wdt.h"


#if 0

//���Ź��жϻص�����
void WdtCallback(void)
{
	//�ڴ˴����и�λ����������˳��ִ�У���APP���ڲ����ж�ģʽ���Ź�
}

//�ڲ����Ź���ʼ�� (�ж�ģʽ)
void InnerWdt_Init(void)
{
	stc_wdt_config_t  stcWdt_Config;
	
	DDL_ZERO_STRUCT(stcWdt_Config);
	
	Sysctrl_SetPeripheralGate(SysctrlPeripheralWdt,TRUE);
	
	stcWdt_Config.u8LoadValue = 0x0b;//3.2s
    stcWdt_Config.enResetEnable = WINT_EN;
    stcWdt_Config.pfnWdtIrqCb = WdtCallback;
	
	Wdt_Init(&stcWdt_Config);
	
    Wdt_Start();
}

//�ڲ����Ź�ι��������������ʱ���һ��֮ǰ����ι����
void InnerWdt_Feed(void)
{
	Wdt_Feed();
}
#endif

#if 1
//�ڲ����Ź���ʼ�� (��λģʽ)
void InnerWdt_Init(void)
{
	stc_wdt_config_t  stcWdt_Config;
	
	DDL_ZERO_STRUCT(stcWdt_Config);
	
	Sysctrl_SetPeripheralGate(SysctrlPeripheralWdt,TRUE);
	
	stcWdt_Config.u8LoadValue = 0x0b;//3.2s
    stcWdt_Config.enResetEnable = WRESET_EN;
	
	Wdt_Init(&stcWdt_Config);
    
    Wdt_Start();
}

//�ڲ����Ź�ι��������������ʱ���һ��֮ǰ����ι����
void InnerWdt_Feed(void)
{
	Wdt_Feed();
}
#endif

