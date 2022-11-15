#include "config.h"
#include "string.h"

void bsp_flash_lock(void)
{
#ifdef STM32F103x
	
	HAL_FLASH_Lock(); 
	
#endif


}

void bsp_flash_unlock(void)
{
#ifdef STM32F103x
	
	HAL_FLASH_Unlock(); 
	
#endif

}

uint8_t  bsp_flash_erase(uint32_t  address,uint32_t page_num)
{
		uint8_t ret = 0;
#ifdef STM32F103x
	uint32_t PAGEError;
	FLASH_EraseInitTypeDef EraseInitStruct;

	
	bsp_flash_unlock();
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_SR_WRPRTERR);
    /* Fill EraseInit structure*/
  EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
  EraseInitStruct.PageAddress = address;
  EraseInitStruct.NbPages     = page_num;

  if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
  {
		return  1;
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

	for (i = 0; i < _ulSize / 4; i++)	
	{
		uint32_t FlashWord;
		
		FlashWord =(_ucpSrc[i*4] << 0)|(_ucpSrc[i*4+1] << 8)|(_ucpSrc[i*4+2] << 16)|(_ucpSrc[i*4+3]<< 24);
		len_cnt += 4;
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, _ulFlashAddr, FlashWord) == HAL_OK)
		{
			_ulFlashAddr = _ulFlashAddr + 4; /* 递增，操作下一个256bit */				
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
		if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, _ulFlashAddr, FlashWord) == HAL_OK)
		{
			; // _ulFlashAddr = _ulFlashAddr + 32;		
		}		
		else
		{
			goto err;
		}
	}
  	/* Flash 加锁，禁止写Flash控制寄存器 */
  	HAL_FLASH_Lock();

  	__set_PRIMASK(0);  		/* 开中断 */

	return 0;
	
err:
  	/* Flash 加锁，禁止写Flash控制寄存器 */
  	HAL_FLASH_Lock();

  	__set_PRIMASK(0);  		/* 开中断 */

	return 1;
	
#endif
}

