/* BSP头文件 */
#include "bsp_wdt.h"

/* 官方库头文件 */
#include "wdt.h"


#if 0

//看门狗中断回调函数
void WdtCallback(void)
{
	//在此处进行复位操作并不能顺利执行，故APP现在不用中断模式看门狗
}

//内部看门狗初始化 (中断模式)
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

//内部看门狗喂狗（宜在最大溢出时间的一半之前进行喂狗）
void InnerWdt_Feed(void)
{
	Wdt_Feed();
}
#endif

#if 1
//内部看门狗初始化 (复位模式)
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

//内部看门狗喂狗（宜在最大溢出时间的一半之前进行喂狗）
void InnerWdt_Feed(void)
{
	Wdt_Feed();
}
#endif

