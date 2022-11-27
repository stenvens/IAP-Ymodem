/* BSPͷ�ļ� */
#include "bsp_flash.h"

/* �ٷ���ͷ�ļ� */
#include "flash.h"


/************************************
����HC32F030C8��Flash����:
	0x0000 - 0x01FF --> Sector0
	0x0200 - 0x03FF --> Sector1
	...
	0xFE00 - 0xFFFF --> Sector127
************************************/


/*********************************************************************
* @brief  ��Flash�жϷ�����(�������)		 
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
* @brief  ��Flashдһ���ֽ�
*---------------------------------------------------------------------
* @param  ��uint32_t FlashAddr,		оƬ�ڲ�Flash��ַ
*			uint8_t Data,			1���ֽ����� 
*---------------------------------------------------------------------
* @retval ��true,д��ɹ� / false,д��ʧ��
*********************************************************************/
bool bsp_flash_write_byte(uint32_t FlashAddr, uint8_t Data)
{
	en_result_t enResult; //���󷵻�ֵ
	
	if(FlashAddr > 0xFFFF) //���HC32L136K8ϵ��
	{
		return false;
	}
    
    //FLASH �ֽ�д��У��
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
* @brief  ��Flashдһ����
*---------------------------------------------------------------------
* @param  ��uint32_t FlashAddr,		оƬ�ڲ�Flash��ַ
*			uint16_t Data,			1�������� 
*---------------------------------------------------------------------
* @retval ��true,д��ɹ� / false,д��ʧ��
*********************************************************************/
bool bsp_flash_write_word(uint32_t FlashAddr, uint32_t Data)
{
	en_result_t enResult; //���󷵻�ֵ
	
	if(FlashAddr%4 != 0) //���д���ַ���ֶ����
		return false;
	
    //FLASH ��д��У��
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
* @brief  ��Flash����
*---------------------------------------------------------------------
* @param  ��uint32_t FlashAddr,		оƬ�ڲ�Flash��ַ
*			uint32_t block_num,		����������
*---------------------------------------------------------------------
* @retval ��true,�����ɹ� / false,����ʧ��
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
