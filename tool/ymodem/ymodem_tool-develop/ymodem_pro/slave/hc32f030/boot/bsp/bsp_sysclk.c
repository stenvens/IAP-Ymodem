/* BSPͷ�ļ� */
#include "bsp_sysclk.h"

/* �ٷ���ͷ�ļ� */
#include "flash.h"


/*********************************************************************
* @brief  ������ϵͳʱ��Ϊ�ⲿ���پ���
*---------------------------------------------------------------------
* @tip    ����ǰ�⺯���汾�����⣬������8MHZ������Ҫ��sysctrl.h�ĺ궨��SYSTEM_XTH��ֵ
*********************************************************************/
void XTH_SysClkConfig(void)
{
	stc_sysctrl_clk_config_t stcCfg;
	
	//��Ҫʹ�õ�ʱ��ԴHCLK������24M���˴�����FLASH ���ȴ�����Ϊ1 cycle(Ĭ��ֵҲΪ1 cycle)
    Flash_WaitCycle(FlashWaitCycle1);
	
	//�л�ʱ��ǰ�������ⲿ���پ�������XTHƵ�ʷ�Χ,���þ��������ʹ��Ŀ��ʱ�ӣ��˴�Ϊ8MHz
    Sysctrl_SetXTHFreq(SysctrlXthFreq6_12MHz); 
    Sysctrl_XTHDriverConfig(SysctrlXtalDriver3);
    Sysctrl_SetXTHStableTime(SysctrlXthStableCycle16384);
    Sysctrl_ClkSourceEnable(SysctrlClkXTH, TRUE);
	
	//ʱ���л�
    Sysctrl_SysClkSwitch(SysctrlClkXTH);
	
	//ѡ���ⲿXTH��ΪHCLKʱ��Դ;
    stcCfg.enClkSrc    = SysctrlClkXTH;
    //HCLK SYSCLK/1
    stcCfg.enHClkDiv   = SysctrlHclkDiv1;
    //PCLK ΪHCLK/1
    stcCfg.enPClkDiv   = SysctrlPclkDiv1;
	
    //ϵͳʱ�ӳ�ʼ��
    Sysctrl_ClkInit(&stcCfg);
}

/*********************************************************************
* @brief  ������ϵͳʱ��Ϊ�ڲ�����ʱ��(4MHz)
*********************************************************************/
void RCH_SysClkConfig(void)
{
	stc_sysctrl_clk_config_t stcCfg;
	
	//��Ҫʹ�õ�ʱ��ԴHCLKС��24M���˴�����FLASH ���ȴ�����Ϊ0 cycle(Ĭ��ֵҲΪ0 cycle)
    Flash_WaitCycle(FlashWaitCycle0);
    
    //ʱ�ӳ�ʼ��ǰ����������Ҫʹ�õ�ʱ��Դ���˴�����RCHΪ4MHz
    Sysctrl_SetRCHTrim(SysctrlRchFreq4MHz);
    
    //ѡ���ڲ�RCH��ΪHCLKʱ��Դ;
    stcCfg.enClkSrc    = SysctrlClkRCH;
    //HCLK SYSCLK/1
    stcCfg.enHClkDiv   = SysctrlHclkDiv1;
    //PCLK ΪHCLK/1
    stcCfg.enPClkDiv   = SysctrlPclkDiv1;
    //ϵͳʱ�ӳ�ʼ��
    Sysctrl_ClkInit(&stcCfg);
}

/*********************************************************************
* @brief  ������ϵͳʱ��Ϊ�ڲ�����ʱ��(16MHz)
*********************************************************************/
void RCH16_SysClkConfig(void)
{
	stc_sysctrl_clk_config_t stcCfg;
	
	/* ��תΪRCH-4MHz */
	//��Ҫʹ�õ�ʱ��ԴHCLKС�ڵ���24M���˴�����FLASH ���ȴ�����Ϊ0 cycle(Ĭ��ֵҲΪ0 cycle)
    Flash_WaitCycle(FlashWaitCycle0);
    
    //ʱ�ӳ�ʼ��ǰ����������Ҫʹ�õ�ʱ��Դ���˴�����RCHΪ4MHz
    Sysctrl_SetRCHTrim(SysctrlRchFreq4MHz);
    
    //ѡ���ڲ�RCH��ΪHCLKʱ��Դ;
    stcCfg.enClkSrc    = SysctrlClkRCH;
    //HCLK SYSCLK/1
    stcCfg.enHClkDiv   = SysctrlHclkDiv1;
    //PCLK ΪHCLK/1
    stcCfg.enPClkDiv   = SysctrlPclkDiv1;
    //ϵͳʱ�ӳ�ʼ��
    Sysctrl_ClkInit(&stcCfg);
	
	/* Ȼ��תΪRCH-16MHz */
	//RCH�ڲ�ʱ�ӵ��л���Ҫ�ӵ͵��������л���Ŀ��ʱ�ӣ�Ĭ��4MHz -> 24MHz��
    //RCHʱ�ӵ�Ƶ���л�����Ҫ�Ƚ�ʱ���л���RCL
    Sysctrl_SetRCLTrim(SysctrlRclFreq32768);
    Sysctrl_ClkSourceEnable(SysctrlClkRCL, TRUE);
    Sysctrl_SysClkSwitch(SysctrlClkRCL);
    
    //����Ŀ��Ƶ�ʵ�RCH��TRIMֵ
    Sysctrl_SetRCHTrim(SysctrlRchFreq16MHz);
    //ʹ��RCH��Ĭ�ϴ򿪣��˴��ɲ���Ҫ�ٴδ򿪣�
    Sysctrl_ClkSourceEnable(SysctrlClkRCH, TRUE);
    //ʱ���л���RCH
    Sysctrl_SysClkSwitch(SysctrlClkRCH);
    //�ر�RCLʱ��
    Sysctrl_ClkSourceEnable(SysctrlClkRCL, FALSE);
}

