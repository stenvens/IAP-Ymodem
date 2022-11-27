#include "config.h"
#include "string.h"

void bsp_flash_lock(void)
{
#ifdef STM32F103x
		FLASH_Lock(); 
#endif
}

void bsp_flash_unlock(void)
{
#ifdef STM32F103x
	FLASH_Unlock();
#endif
}

uint8_t  bsp_flash_erase(uint32_t  address,uint32_t page_num)
{
		uint8_t ret = 0;
#ifdef STM32F103x


	bsp_flash_unlock();
	FLASH_ClearFlag(FLASH_FLAG_PGERR|FLASH_FLAG_WRPRTERR|FLASH_FLAG_EOP);
	if(FLASH_ErasePage(address) != FLASH_COMPLETE)
	{
	  ret = 1;
	}
	/* 擦除完毕后，上锁 */
	bsp_flash_lock();	
	
#endif
return ret;
}

uint8_t bsp_flash_write(uint32_t _ulFlashAddr, uint8_t *_ucpSrc, uint32_t _ulSize)
{
#ifdef STM32F103x
	uint32_t i;
	uint8_t ucRet;
  uint32_t len_cnt = 0;
	if (_ulFlashAddr + _ulSize >((FLASH_END_ADDR+1)-FLASH_PAGESIZE))
	{
		return 0;
	}

	if (_ulSize == 0)
	{
		return 0;
	}


	__set_PRIMASK(1);  		/* 关中断 */

	/* FLASH 解锁 */
	bsp_flash_unlock();

	for (i = 0; i < (_ulSize / 4); i++)	
	{
		uint32_t FlashWord;
		
		FlashWord =(_ucpSrc[i*4] << 0)|(_ucpSrc[i*4+1] << 8)|(_ucpSrc[i*4+2] << 16)|(_ucpSrc[i*4+3]<< 24);
		len_cnt += 4;
		if(FLASH_ProgramWord(_ulFlashAddr,FlashWord) == FLASH_COMPLETE)
		{
			 _ulFlashAddr += 4;
		}
		else
		{
			 goto err;
		
		}
	}
	
	if (_ulSize % 4)
	{	
		uint32_t FlashWord=0;
		switch (_ulSize % 4){
			case 1:
						FlashWord =(_ucpSrc[len_cnt] << 0);
				break;
			case 2:
						FlashWord =(_ucpSrc[len_cnt] << 0) | (_ucpSrc[len_cnt+1] << 8);
				break;
			case 3:
						FlashWord =(_ucpSrc[len_cnt] << 0) | (_ucpSrc[len_cnt+1] << 8) | (_ucpSrc[len_cnt+1] << 16);			
				break;
			default:
				break;		
		}
		if(FLASH_ProgramWord(_ulFlashAddr,FlashWord) == FLASH_COMPLETE)
		{
			 _ulFlashAddr += 4;
		}
		else
		{
			 goto err;
		
		}
	}
  	/* Flash 加锁，禁止写Flash控制寄存器 */
//  	HAL_FLASH_Lock();

  	__set_PRIMASK(0);  		/* 开中断 */

	return 0;
	
err:
  	/* Flash 加锁，禁止写Flash控制寄存器 */
//  	HAL_FLASH_Lock();

  	__set_PRIMASK(0);  		/* 开中断 */

	return 1;
	
#endif
}



/*
*********************************************************************************************************
*	函 数 名: bsp_ReadCpuFlash
*	功能说明: 读取CPU Flash的内容
*	形    参:  _ucpDst : 目标缓冲区
*			 _ulFlashAddr : 起始地址
*			 _ulSize : 数据大小（单位是字节）
*	返 回 值: 1=成功，0=失败
*********************************************************************************************************
*/
uint8_t bsp_flash_read(uint32_t _ulFlashAddr, uint8_t *_ucpDst, uint32_t _ulSize)
{
	uint32_t i;

	if (_ulFlashAddr + _ulSize >((FLASH_END_ADDR+1)-FLASH_PAGESIZE))
	{
		return 0;
	}

	/* 长度为0时不继续操作,否则起始地址为奇地址会出错 */
	if (_ulSize == 0)
	{
		return 1;
	}

	for (i = 0; i < _ulSize; i++)
	{
		*_ucpDst++ = *(uint8_t *)_ulFlashAddr++;
	}

	return 0;
}
