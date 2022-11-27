#include "bsp_flash.h"


/*********************************************************************
* @brief  ��Flash��һ���ֽ�
*---------------------------------------------------------------------
* @param  ��uint32_t FlashAddr,		оƬ�ڲ�Flash��ַ
*---------------------------------------------------------------------
* @retval ���õ�ַ��Ӧ��1���ֽ����� 
*********************************************************************/
uint8_t bsp_flash_read_byte(uint32_t FlashAddr)
{
	return *((volatile uint8_t*)FlashAddr);
}

/*********************************************************************
* @brief  ��Flash��һ����
*---------------------------------------------------------------------
* @param  ��uint32_t FlashAddr,		оƬ�ڲ�Flash��ַ
*---------------------------------------------------------------------
* @retval ���õ�ַ��Ӧ��1�������� 
*********************************************************************/
uint32_t bsp_flash_read_word(uint32_t FlashAddr)
{
	return *((volatile uint32_t*)FlashAddr);
}

/*********************************************************************
* @brief  ��������д������(32λ)
*---------------------------------------------------------------------
* @param  ��uint32_t WriteAddr	,	д��ĵ�ַ	
*			uint32_t data		,	д�������
*---------------------------------------------------------------------
* @retval ��true, д��ɹ� / false, д��ʧ��
*********************************************************************/
bool bsp_flash_write_word(uint32_t WriteAddr, uint32_t data)
{
	HAL_FLASH_Unlock();
	
	if( HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, WriteAddr, data) != HAL_OK )
	{
		HAL_FLASH_Lock();
		return false; //д�����
	}
	
	HAL_FLASH_Lock();
	return true; //д��ɹ�
}

/*********************************************************************
* @brief  �������ڲ�Flash
*---------------------------------------------------------------------
* @param  ��uint32_t erase_start_address	, 	Ҫ�������ڲ�Flash��ʼ��ַ
*			uint16_t erase_page_num			,	����ҳ��
*---------------------------------------------------------------------
* @retval ��true,�����ɹ� / false,����ʧ��
*********************************************************************/
bool bsp_flash_erase_block(uint32_t erase_start_address, uint16_t erase_page_num)
{
	FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t PAGEError = 0;
	
	HAL_FLASH_Unlock();
	
	EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES; //��ҳ����
	EraseInitStruct.PageAddress = erase_start_address; //ָ��������ʼ��ַ
	EraseInitStruct.NbPages     = erase_page_num; //����ҳ��
	
	if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
	{
		HAL_FLASH_Lock();
		return false; //��������
	}

	HAL_FLASH_Lock();
	return true; //���ɹ�
}

/*********************************************************************
* @brief  ��������д��Flash (�豣֤д֮ǰ��ҳ�Ѳ���)
*---------------------------------------------------------------------
* @param  ��uint32_t address    ,	д���Flash�׵�ַ 
*			uint8_t *buf        , 	Ҫд�������ָ��
*			uint32_t length     ,	�����ֽڳ���
*---------------------------------------------------------------------
* @retval ��none
*********************************************************************/
void boot_interface_write_flash(uint32_t address, uint8_t *buf, uint32_t length)
{
    uint32_t i = 0;
	uint32_t start_address = address;
	uint32_t data = 0;
	
	//����Ϊ0ʱֱ�ӷ���
	if(length == 0)
	{
		return ;
	}
	
	//���ȷ�4�ֽڱ���ʱ�����µ�4�ֽ�
	if(length%4 != 0)
	{
		length = (length/4)*4 + 4;
	}

	for (i=0; i<(length/4); i++)
	{	
		//С�˸�ʽ���
		data = buf[i*4 + 0];
		data |= buf[i*4 + 1] << 8;
		data |= buf[i*4 + 2] << 16;
		data |= buf[i*4 + 3] << 24;
		bsp_flash_write_word(start_address, data);
		start_address += 4;
	}	
}

