/* ϵͳͷ�ļ� */
#include <stdbool.h>
#include <string.h>

/* ����ͷ�ļ� */
#include "boot_iap.h"


iapfun jump2app; 
void MSR_MSP(unsigned  int addr);	//���ö�ջ��ַ

//����ջ����ַ
//addr:ջ����ַ
__asm void MSR_MSP(unsigned  int addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}

//��ת��Ӧ�ó����
//appxaddr:�û�������ʼ��ַ.
void iap_load_app(uint32_t appxaddr)
{
	if(((*(__IO uint32_t*)appxaddr)&0x2FFE0000)==0x20000000)	//���ջ����ַ�Ƿ�Ϸ�.
	{ 
		jump2app=(iapfun)*(__IO uint32_t*)(appxaddr+4);		//�û��������ڶ�����Ϊ����ʼ��ַ(��λ��ַ)		
		MSR_MSP(*(__IO uint32_t*)appxaddr);					//��ʼ��APP��ջָ��(�û��������ĵ�һ�������ڴ��ջ����ַ)
		jump2app();									//��ת��APP.
	}
}	

//�����λ
void iap_reboot(void)
{
	__disable_irq();       // �ر������ж�
	NVIC_SystemReset();      // ��λ
}
