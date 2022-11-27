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
	/* ������Ϻ����� */
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


	__set_PRIMASK(1);  		/* ���ж� */

	/* FLASH ���� */
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
  	/* Flash ��������ֹдFlash���ƼĴ��� */
//  	HAL_FLASH_Lock();

  	__set_PRIMASK(0);  		/* ���ж� */

	return 0;
	
err:
  	/* Flash ��������ֹдFlash���ƼĴ��� */
//  	HAL_FLASH_Lock();

  	__set_PRIMASK(0);  		/* ���ж� */

	return 1;
	
#endif
}



/*
*********************************************************************************************************
*	�� �� ��: bsp_ReadCpuFlash
*	����˵��: ��ȡCPU Flash������
*	��    ��:  _ucpDst : Ŀ�껺����
*			 _ulFlashAddr : ��ʼ��ַ
*			 _ulSize : ���ݴ�С����λ���ֽڣ�
*	�� �� ֵ: 1=�ɹ���0=ʧ��
*********************************************************************************************************
*/
uint8_t bsp_flash_read(uint32_t _ulFlashAddr, uint8_t *_ucpDst, uint32_t _ulSize)
{
	uint32_t i;

	if (_ulFlashAddr + _ulSize >((FLASH_END_ADDR+1)-FLASH_PAGESIZE))
	{
		return 0;
	}

	/* ����Ϊ0ʱ����������,������ʼ��ַΪ���ַ����� */
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
