#include "boot_interface.h"
#include "boot_param.h"
#include "bsp_flash.h"


/*********************************************************************
* @brief  ：设置擦除FLASH的标志
*---------------------------------------------------------------------
* @param  ：uint8_t mode	,	(0,表示APP已擦除 / 1,表示APP已写入)
*---------------------------------------------------------------------
* @retval ：none
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
* @brief  ：检测是否可以允许运行APP
*---------------------------------------------------------------------
* @param  ：none
*---------------------------------------------------------------------
* @retval ：true,可以运行	/ 	false,不能运行
*********************************************************************/
bool boot_interface_check_app_valid(void)
{
	if(boot_param.tab.usart_iap_status == FLASH_APP_HAS_WRITTEN)
		return true;
	else
		return false;
}

/*********************************************************************
* @brief  ：检测app是否已被擦除过
*---------------------------------------------------------------------
* @param  ：none
*---------------------------------------------------------------------
* @retval ：true,是	/ 	false,否
*********************************************************************/
bool boot_interface_check_app_erased(void)
{
	if(boot_param.tab.usart_iap_status != FALSH_APP_HAS_ERASED)
		return true;
	else
		return false;
}

/*********************************************************************
* @brief  ：检测是否已有APP存在
*---------------------------------------------------------------------
* @param  ：none
*---------------------------------------------------------------------
* @retval ：true,存在	/ 	false,不存在
*********************************************************************/
bool boot_interface_check_app_existed(void)
{
    uint8_t i = 0;
	
	//检测APP地址的第30Kbyte处开始的前32个地址是否都为0xff(即内部Flash擦除后就是全0xff)
	for(i=0; i<32; i++)
	{
		//App程序目前大于1K，所以下面设置为2，如果App小于1K，需要修改
		if(bsp_flash_read_byte(BLOCK_APP1_START + MCU_FLASH_PAGE_SIZE*2 + i) != 0xff)
		{
			return true; //说明已存在APP
		}
	}
	
	return false; //不存在APP
}

