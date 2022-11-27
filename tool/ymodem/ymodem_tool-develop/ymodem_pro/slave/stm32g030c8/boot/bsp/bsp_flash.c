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
*			uint64_t data		,	д�������
*---------------------------------------------------------------------
* @retval ��true, д��ɹ� / false, д��ʧ��
*********************************************************************/
bool bsp_flash_write_double_word(uint32_t WriteAddr, uint64_t data)
{
	HAL_FLASH_Unlock();
	
	if( HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, WriteAddr, data) != HAL_OK )
	{
		HAL_FLASH_Lock();
		return false; //д�����
	}
	
	HAL_FLASH_Lock();
	return true; //д��ɹ�
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
* @brief  �������ڲ�Flash
*---------------------------------------------------------------------
* @param  ��uint32_t StartAddress	, 	Ҫ�������ڲ�Flash��ʼ��ַ
*			uint16_t ErasePageNum	,	����ҳ��
*---------------------------------------------------------------------
* @retval ��true,�����ɹ� / false,����ʧ��
*********************************************************************/
bool bsp_flash_erase_block(uint32_t StartAddress, uint16_t ErasePageNum)
{
	FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t PAGEError = 0;
	uint32_t FirstPage = 0;
	
	/* Get the 1st page to erase */
	FirstPage = GetPage(StartAddress);
	
	HAL_FLASH_Unlock();

	EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES; //��ҳ����
	EraseInitStruct.Page 		= FirstPage; //ָ��������ʼ��ַ
	EraseInitStruct.NbPages     = ErasePageNum; //����ҳ��
	
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
	uint64_t data = 0;
	uint32_t data1 = 0, data2 = 0;
	
	//����Ϊ0ʱֱ�ӷ���
	if(length == 0)
	{
		return ;
	}
	
	//���ȷ�8�ֽڱ���ʱ�����µ�8�ֽ�
	if(length%8 != 0)
	{
		length = (length/8)*8 + 8;
	}

	for (i=0; i<(length/8); i++)
	{	
		//С�˸�ʽ���
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

