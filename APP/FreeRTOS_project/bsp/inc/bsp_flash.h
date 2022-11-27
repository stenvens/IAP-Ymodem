#ifndef __BSP_FLASH_H_
#define __BSP_FLASH_H_
#include "config.h"


#define PAR_ADDRESS    		  (FLASH_END_ADDR+1)-(2*FLASH_PAGESIZE)//�����̻�����ʼ��ַ����ʼ��ַ
#define FLASH_BASE_ADDR     (uint32_t)(0x08000000U)	
#define FLASH_END_ADDR      (uint32_t)(0x0803FFFF)

#define FLASH_PAGESIZE	   (0x800U)		/* ҳ��С���ֽ� */


uint8_t  bsp_flash_erase(uint32_t  address,uint32_t page_num);
uint8_t bsp_flash_write(uint32_t _ulFlashAddr, uint8_t *_ucpSrc, uint32_t _ulSize);
uint8_t bsp_flash_read(uint32_t _ulFlashAddr, uint8_t *_ucpDst, uint32_t _ulSize);
#endif

