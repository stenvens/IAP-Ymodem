/*****************************************************************
 * name:	ring_buffer.c
 * function:	环形队列
 * author:	sxh
 * date:	2022-11-23
 * version:	1.0.0
*****************************************************************/
#include "stdio.h"
#include "stdint.h"
#include "ring_buffer.h"

/**
  * @brief   判断环形队列是否满了   
  * @note    			none  
  * @param[in]   	buf：要判断的队列
	* @param[out]   1:满了 0：没满
  * @retval 
  */
uint8_t RING_BUFFIsFull(RING_BUFF *buf)
{
	uint8_t wirte_cnt = buf->WritePt;
	wirte_cnt++;
	return((wirte_cnt & (RING_BUFF_SIZE -1))== buf->ReadPt);
}


/**
  * @brief   判断缓冲区是否空   
  * @note    			none  
  * @param[in]   	buf：要判断的队列
	* @param[out]   1:空 0：没空
  * @retval 
  */
uint8_t RING_BUFFIsEmpty(RING_BUFF *buf)
{
	return (buf->WritePt == buf->ReadPt);
}


/**
  * @brief   获取队列的大小   
  * @note    			none  
  * @param[in]   	buf：要获取的队列
	* @param[out]   队列的大小
  * @retval 
  */
uint8_t RING_BUFFGetSize(RING_BUFF *buf)
{
	if(buf->ReadPt <= buf->WritePt)
		return buf->WritePt - buf->ReadPt;
	
	return RING_BUFF_SIZE - buf->ReadPt + buf->WritePt;
}


/**
  * @brief   获取队列剩余的大小   
  * @note    			none  
  * @param[in]   	buf：要获取的队列
	* @param[out]   队列剩余的大小
  * @retval 
  */
uint8_t RING_BUFFGetFreeSize(RING_BUFF *buf)
{
	if(buf->WritePt < buf->ReadPt)
		return buf->ReadPt - buf->WritePt-1;
	return RING_BUFF_SIZE - buf->WritePt + buf->ReadPt-1;
}

/**
  * @brief   将数据写入队列，并覆盖以前的内容  
  * @note    			none  
	* @param[in]   	buf：要写的队列 data:要写的数据
	* @param[out]   队列的大小
  * @retval 
  */
void Ring_BuffWrite(RING_BUFF *buf,uint8_t data)
{
	if(RING_BUFFIsFull(buf))
	{
		buf->Buff[buf->WritePt++] = data;
		buf->WritePt &= (RING_BUFF_SIZE - 1);
		buf->ReadPt ++;
		buf->ReadPt &= (RING_BUFF_SIZE - 1);
	}
	else
	{
		buf->Buff[buf->WritePt++] = data;
		buf->WritePt &= (RING_BUFF_SIZE - 1);
	}
}

/*********************************************************
 * name:ReadBuff
 * function:从缓冲区中读取数据
 * Input:缓冲区指针:buf
 * Output:从缓冲区中读取的一个字节
 * calling:none
 * called by:
 * Create: Sunvy
 * date:	2012-4-2
********************************************************/

/**
  * @brief   从缓冲区中读取数据
  * @note    			none  
	* @param[in]   	buf：要读取的队列 dat:缓冲区
	* @param[out]   -1:读取失败 0:读取成功
  * @retval 
  */
int8_t Ring_BuffRead(RING_BUFF *buf,uint8_t *dat)
{
	int8_t res = 0;

	if(!RING_BUFFIsEmpty(buf))
	{
		*dat = buf->Buff[buf->ReadPt++];
		buf->ReadPt &= (RING_BUFF_SIZE - 1);
		res = 1;
	}
	return res;
}
/*********************************************************
 * name:HAL_RING_BUFFInit
 * function:清空缓冲区
 * Input:Buffer Pointer:buf
 * Output:None
 * calling:none
 * called by:
 * Create: Sunvy
 * date:	2012-4-2
********************************************************/

/**
  * @brief   清空缓冲区
  * @note    			none  
	* @param[in]   	buf：要清空的的队列
	* @param[out]   none
  * @retval 
  */
void RING_BUFFInit(RING_BUFF *buf)
{
	if(buf == NULL)
		return;
	buf->ReadPt = buf->WritePt = 0;
}

