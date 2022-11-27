/* BSP头文件 */
#include "bsp_sysclk.h"

/* 官方库头文件 */
#include "flash.h"


/*********************************************************************
* @brief  ：配置系统时钟为外部高速晶振
*---------------------------------------------------------------------
* @tip    ：当前库函数版本的问题，板子上8MHZ，还需要在sysctrl.h改宏定义SYSTEM_XTH的值
*********************************************************************/
void XTH_SysClkConfig(void)
{
	stc_sysctrl_clk_config_t stcCfg;
	
	//因要使用的时钟源HCLK将大于24M：此处设置FLASH 读等待周期为1 cycle(默认值也为1 cycle)
    Flash_WaitCycle(FlashWaitCycle1);
	
	//切换时钟前（根据外部高速晶振）设置XTH频率范围,配置晶振参数，使能目标时钟，此处为8MHz
    Sysctrl_SetXTHFreq(SysctrlXthFreq6_12MHz); 
    Sysctrl_XTHDriverConfig(SysctrlXtalDriver3);
    Sysctrl_SetXTHStableTime(SysctrlXthStableCycle16384);
    Sysctrl_ClkSourceEnable(SysctrlClkXTH, TRUE);
	
	//时钟切换
    Sysctrl_SysClkSwitch(SysctrlClkXTH);
	
	//选择外部XTH作为HCLK时钟源;
    stcCfg.enClkSrc    = SysctrlClkXTH;
    //HCLK SYSCLK/1
    stcCfg.enHClkDiv   = SysctrlHclkDiv1;
    //PCLK 为HCLK/1
    stcCfg.enPClkDiv   = SysctrlPclkDiv1;
	
    //系统时钟初始化
    Sysctrl_ClkInit(&stcCfg);
}

/*********************************************************************
* @brief  ：配置系统时钟为内部高速时钟(4MHz)
*********************************************************************/
void RCH_SysClkConfig(void)
{
	stc_sysctrl_clk_config_t stcCfg;
	
	//因要使用的时钟源HCLK小于24M：此处设置FLASH 读等待周期为0 cycle(默认值也为0 cycle)
    Flash_WaitCycle(FlashWaitCycle0);
    
    //时钟初始化前，优先设置要使用的时钟源：此处设置RCH为4MHz
    Sysctrl_SetRCHTrim(SysctrlRchFreq4MHz);
    
    //选择内部RCH作为HCLK时钟源;
    stcCfg.enClkSrc    = SysctrlClkRCH;
    //HCLK SYSCLK/1
    stcCfg.enHClkDiv   = SysctrlHclkDiv1;
    //PCLK 为HCLK/1
    stcCfg.enPClkDiv   = SysctrlPclkDiv1;
    //系统时钟初始化
    Sysctrl_ClkInit(&stcCfg);
}

/*********************************************************************
* @brief  ：配置系统时钟为内部高速时钟(16MHz)
*********************************************************************/
void RCH16_SysClkConfig(void)
{
	stc_sysctrl_clk_config_t stcCfg;
	
	/* 先转为RCH-4MHz */
	//因要使用的时钟源HCLK小于等于24M：此处设置FLASH 读等待周期为0 cycle(默认值也为0 cycle)
    Flash_WaitCycle(FlashWaitCycle0);
    
    //时钟初始化前，优先设置要使用的时钟源：此处设置RCH为4MHz
    Sysctrl_SetRCHTrim(SysctrlRchFreq4MHz);
    
    //选择内部RCH作为HCLK时钟源;
    stcCfg.enClkSrc    = SysctrlClkRCH;
    //HCLK SYSCLK/1
    stcCfg.enHClkDiv   = SysctrlHclkDiv1;
    //PCLK 为HCLK/1
    stcCfg.enPClkDiv   = SysctrlPclkDiv1;
    //系统时钟初始化
    Sysctrl_ClkInit(&stcCfg);
	
	/* 然后转为RCH-16MHz */
	//RCH内部时钟的切换需要从低到高依次切换到目标时钟（默认4MHz -> 24MHz）
    //RCH时钟的频率切换，需要先将时钟切换到RCL
    Sysctrl_SetRCLTrim(SysctrlRclFreq32768);
    Sysctrl_ClkSourceEnable(SysctrlClkRCL, TRUE);
    Sysctrl_SysClkSwitch(SysctrlClkRCL);
    
    //加载目标频率的RCH的TRIM值
    Sysctrl_SetRCHTrim(SysctrlRchFreq16MHz);
    //使能RCH（默认打开，此处可不需要再次打开）
    Sysctrl_ClkSourceEnable(SysctrlClkRCH, TRUE);
    //时钟切换到RCH
    Sysctrl_SysClkSwitch(SysctrlClkRCH);
    //关闭RCL时钟
    Sysctrl_ClkSourceEnable(SysctrlClkRCL, FALSE);
}

