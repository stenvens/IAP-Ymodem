#include "boot_interface.h"
#include "boot_param.h"
#include "bsp_flash.h"


/*********************************************************************
* @brief  �����ò���FLASH�ı�־
*---------------------------------------------------------------------
* @param  ��uint8_t mode	,	(0,��ʾAPP�Ѳ��� / 1,��ʾAPP��д��)
*---------------------------------------------------------------------
* @retval ��none
*********************************************************************/
void boot_interface_set_app_erased_flag(uint8_t mode)
{
	if(mode == 1)
		boot_param.tab.usart_iap_status = FLASH_APP_HAS_WRITTEN;
	else
		boot_param.tab.usart_iap_status = FALSH_APP_HAS_ERASED;
	
	boot_param_write();
}

/*********************************************************************
* @brief  ������Ƿ������������APP
*---------------------------------------------------------------------
* @param  ��none
*---------------------------------------------------------------------
* @retval ��true,��������	/ 	false,��������
*********************************************************************/
bool boot_interface_check_app_valid(void)
{
	if(boot_param.tab.usart_iap_status == FLASH_APP_HAS_WRITTEN)
		return true;
	else
		return false;
}

/*********************************************************************
* @brief  �����app�Ƿ��ѱ�������
*---------------------------------------------------------------------
* @param  ��none
*---------------------------------------------------------------------
* @retval ��true,��	/ 	false,��
*********************************************************************/
bool boot_interface_check_app_erased(void)
{
	if(boot_param.tab.usart_iap_status != FALSH_APP_HAS_ERASED)
		return true;
	else
		return false;
}

/*********************************************************************
* @brief  ������Ƿ�����APP����
*---------------------------------------------------------------------
* @param  ��none
*---------------------------------------------------------------------
* @retval ��true,����	/ 	false,������
*********************************************************************/
bool boot_interface_check_app_existed(void)
{
    uint8_t i = 0;
	
	//���APP��ַ�ĵ�30Kbyte����ʼ��ǰ32����ַ�Ƿ�Ϊ0xff(���ڲ�Flash���������ȫ0xff)
	for(i=0; i<32; i++)
	{
		//App����Ŀǰ����1K��������������Ϊ2�����AppС��1K����Ҫ�޸�
		if(bsp_flash_read_byte(BLOCK_APP1_START + MCU_FLASH_PAGE_SIZE*2 + i) != 0xff)
		{
			return true; //˵���Ѵ���APP
		}
	}
	
	return false; //������APP
}

