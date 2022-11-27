#include "bsp_flash.h"


/*********************************************************************
* @brief  ：Flash读一个字节
*---------------------------------------------------------------------
* @param  ：uint32_t FlashAddr,		芯片内部Flash地址
*---------------------------------------------------------------------
* @retval ：该地址对应的1个字节数据 
*********************************************************************/
uint8_t bsp_flash_read_byte(uint32_t FlashAddr)
{
	return *((volatile uint8_t*)FlashAddr);
}

/*********************************************************************
* @brief  ：Flash读一个字
*---------------------------------------------------------------------
* @param  ：uint32_t FlashAddr,		芯片内部Flash地址
*---------------------------------------------------------------------
* @retval ：该地址对应的1个字数据 
*********************************************************************/
uint32_t bsp_flash_read_word(uint32_t FlashAddr)
{
	return *((volatile uint32_t*)FlashAddr);
}

/*********************************************************************
* @brief  ：不检查的写入数据(32位)
*---------------------------------------------------------------------
* @param  ：uint32_t WriteAddr	,	写入的地址	
*			uint32_t data		,	写入的数据
*---------------------------------------------------------------------
* @retval ：true, 写入成功 / false, 写入失败
*********************************************************************/
bool bsp_flash_write_word(uint32_t WriteAddr, uint32_t data)
{
	HAL_FLASH_Unlock();
	
	if( HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, WriteAddr, data) != HAL_OK )
	{
		HAL_FLASH_Lock();
		return false; //写入错误
	}
	
	HAL_FLASH_Lock();
	return true; //写入成功
}

/*********************************************************************
* @brief  ：擦除内部Flash
*---------------------------------------------------------------------
* @param  ：uint32_t erase_start_address	, 	要擦出的内部Flash起始地址
*			uint16_t erase_page_num			,	擦除页数
*---------------------------------------------------------------------
* @retval ：true,擦除成功 / false,擦除失败
*********************************************************************/
bool bsp_flash_erase_block(uint32_t erase_start_address, uint16_t erase_page_num)
{
	FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t PAGEError = 0;
	
	HAL_FLASH_Unlock();
	
	EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES; //按页擦除
	EraseInitStruct.PageAddress = erase_start_address; //指定擦除起始地址
	EraseInitStruct.NbPages     = erase_page_num; //擦除页数
	
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
	{
		HAL_FLASH_Lock();
		return false; //擦除错误
	}

	HAL_FLASH_Lock();
	return true; //出成功
}

/*********************************************************************
* @brief  ：将代码写入Flash (需保证写之前该页已擦除)
*---------------------------------------------------------------------
* @param  ：uint32_t address    ,	写入的Flash首地址 
*			uint8_t *buf        , 	要写入的数据指针
*			uint32_t length     ,	数据字节长度
*---------------------------------------------------------------------
* @retval ：none
*********************************************************************/
void boot_interface_write_flash(uint32_t address, uint8_t *buf, uint32_t length)
{
    uint32_t i = 0;
	uint32_t start_address = address;
	uint32_t data = 0;
	
	//长度为0时直接返回
	if(length == 0)
	{
		return ;
	}
	
	//长度非4字节倍数时则补齐新的4字节
	if(length%4 != 0)
	{
		length = (length/4)*4 + 4;
	}

	for (i=0; i<(length/4); i++)
	{	
		//小端格式存放
		data = buf[i*4 + 0];
		data |= buf[i*4 + 1] << 8;
		data |= buf[i*4 + 2] << 16;
		data |= buf[i*4 + 3] << 24;
		bsp_flash_write_word(start_address, data);
		start_address += 4;
	}	
}

