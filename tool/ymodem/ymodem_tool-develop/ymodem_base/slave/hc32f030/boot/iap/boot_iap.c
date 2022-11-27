/* 系统头文件 */
#include <stdbool.h>
#include <string.h>

/* 其他头文件 */
#include "boot_iap.h"


iapfun jump2app; 
void MSR_MSP(unsigned  int addr);	//设置堆栈地址

//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(unsigned  int addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}

//跳转到应用程序段
//appxaddr:用户代码起始地址.
void iap_load_app(uint32_t appxaddr)
{
	if(((*(__IO uint32_t*)appxaddr)&0x2FFE0000)==0x20000000)	//检查栈顶地址是否合法.
	{ 
		jump2app=(iapfun)*(__IO uint32_t*)(appxaddr+4);		//用户代码区第二个字为程序开始地址(复位地址)		
		MSR_MSP(*(__IO uint32_t*)appxaddr);					//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
		jump2app();									//跳转到APP.
	}
}	

//软件复位
void iap_reboot(void)
{
	__disable_irq();       // 关闭所有中端
	NVIC_SystemReset();      // 复位
}
