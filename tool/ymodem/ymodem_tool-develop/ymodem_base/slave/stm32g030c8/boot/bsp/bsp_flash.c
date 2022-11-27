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
*			uint64_t data		,	写入的数据
*---------------------------------------------------------------------
* @retval ：true, 写入成功 / false, 写入失败
*********************************************************************/
bool bsp_flash_write_double_word(uint32_t WriteAddr, uint64_t data)
{
	HAL_FLASH_Unlock();
	
	if( HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, WriteAddr, data) != HAL_OK )
	{
		HAL_FLASH_Lock();
		return false; //写入错误
	}
	
	HAL_FLASH_Lock();
	return true; //写入成功
}

/**
  * @brief  Gets the page of a given address
  * @param  Addr: Address of the FLASH Memory
  * @retval The page of a given address
  */
static uint32_t GetPage(uint32_t Addr)
{
  return (Addr - FLASH_BASE) / FLASH_PAGE_SIZE;;
}

/*********************************************************************
* @brief  ：擦除内部Flash
*---------------------------------------------------------------------
* @param  ：uint32_t StartAddress	, 	要擦出的内部Flash起始地址
*			uint16_t ErasePageNum	,	擦除页数
*---------------------------------------------------------------------
* @retval ：true,擦除成功 / false,擦除失败
*********************************************************************/
bool bsp_flash_erase_block(uint32_t StartAddress, uint16_t ErasePageNum)
{
	FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t PAGEError = 0;
	uint32_t FirstPage = 0;
	
	/* Get the 1st page to erase */
	FirstPage = GetPage(StartAddress);
	
	HAL_FLASH_Unlock();

	EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES; //按页擦除
	EraseInitStruct.Page 		= FirstPage; //指定擦除起始地址
	EraseInitStruct.NbPages     = ErasePageNum; //擦除页数
	
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
	{
		HAL_FLASH_Lock();
		return false;
	}
	else
	{
		HAL_FLASH_Lock();
		return true;
	}
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
	uint64_t data = 0;
	uint32_t data1 = 0, data2 = 0;
	
	//长度为0时直接返回
	if(length == 0)
	{
		return ;
	}
	
	//长度非8字节倍数时则补齐新的8字节
	if(length%8 != 0)
	{
		length = (length/8)*8 + 8;
	}

	for (i=0; i<(length/8); i++)
	{	
		//小端格式存放
		data1 = buf[i*8 + 0];
		data1 |= buf[i*8 + 1] << 8;
		data1 |= buf[i*8 + 2] << 16;
		data1 |= buf[i*8 + 3] << 24;
		data2 = buf[i*8 + 4];
		data2 |= buf[i*8 + 5] << 8;
		data2 |= buf[i*8 + 6] << 16;
		data2 |= buf[i*8 + 7] << 24;
		data = (uint64_t)data2*256*256*256*256 + (uint64_t)data1;

		bsp_flash_write_double_word(start_address, data);
		start_address += 8;
	}	
}

