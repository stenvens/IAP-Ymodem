/* BSP头文件 */
#include "bsp_flash.h"

/* 官方库头文件 */
#include "flash.h"


/************************************
关于HC32F030C8的Flash划分:
	0x0000 - 0x01FF --> Sector0
	0x0200 - 0x03FF --> Sector1
	...
	0xFE00 - 0xFFFF --> Sector127
************************************/


/*********************************************************************
* @brief  ：Flash中断服务函数(无需更改)		 
*********************************************************************/
void FlashInt(void)
{
	if (TRUE == Flash_GetIntFlag(FlashPCInt))
	{
		Flash_ClearIntFlag(FlashPCInt);
		Flash_DisableIrq(FlashPCInt);
	}
	if (TRUE == Flash_GetIntFlag(FlashSlockInt))
	{
		Flash_ClearIntFlag(FlashSlockInt);
		Flash_DisableIrq(FlashSlockInt);
	}
}
	 
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
* @brief  ：Flash写一个字节
*---------------------------------------------------------------------
* @param  ：uint32_t FlashAddr,		芯片内部Flash地址
*			uint8_t Data,			1个字节数据 
*---------------------------------------------------------------------
* @retval ：true,写入成功 / false,写入失败
*********************************************************************/
bool bsp_flash_write_byte(uint32_t FlashAddr, uint8_t Data)
{
	en_result_t enResult; //错误返回值
	
	if(FlashAddr > 0xFFFF) //针对HC32L136K8系列
	{
		return false;
	}
    
    //FLASH 字节写、校验
    enResult = Flash_WriteByte(FlashAddr, Data);
    if (Ok == enResult)
    {
        if(*((volatile uint8_t*)FlashAddr) == Data)
        {
            enResult = Ok;
			return true;
        }
    }
    else
    {
        enResult = Error;
    }  
	return false;
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
* @brief  ：Flash写一个字
*---------------------------------------------------------------------
* @param  ：uint32_t FlashAddr,		芯片内部Flash地址
*			uint16_t Data,			1个字数据 
*---------------------------------------------------------------------
* @retval ：true,写入成功 / false,写入失败
*********************************************************************/
bool bsp_flash_write_word(uint32_t FlashAddr, uint32_t Data)
{
	en_result_t enResult; //错误返回值
	
	if(FlashAddr%4 != 0) //如果写入地址非字对齐的
		return false;
	
    //FLASH 字写、校验
    enResult = Flash_WriteWord(FlashAddr, Data);
    if (Ok == enResult)
    {
        if(*((volatile uint32_t*)FlashAddr) == Data)
        {
            enResult = Ok;
			return true;
        }
    }
    else
    {
        enResult = Error;
    }  
	
	return false;
}

/*********************************************************************
* @brief  ：Flash擦除
*---------------------------------------------------------------------
* @param  ：uint32_t FlashAddr,		芯片内部Flash地址
*			uint32_t block_num,		擦除块数量
*---------------------------------------------------------------------
* @retval ：true,擦除成功 / false,擦除失败
*********************************************************************/
bool bsp_flash_erase_block(uint32_t erase_start_address, uint32_t block_num)
{
	for(uint32_t i=0; i<block_num; i++)
	{
		Flash_SectorErase(erase_start_address + i*512);
	}

	return true;
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
