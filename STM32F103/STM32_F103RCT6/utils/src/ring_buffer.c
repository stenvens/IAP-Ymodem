#include "ring_buffer.h"
#include "bsp_systick.h"

static USART_RING_BUFFER ring_buf={0};
 /**
  * @brief   清理环形队列     
  * @note    无  
  * @param   无
  * @retval  无
  */ 
void ClearRingBuf(void)
{
	ring_buf.rx_head = 0;
	ring_buf.rx_tail = 0;
}

USART_RING_BUFFER *get_ring_buffer(void)
{

	 return &ring_buf;

}

 /**
  * @brief   清理环形队列     
  * @note    无  
  * @param   无
  * @retval  无
  */ 
char GetRingBufchar(uint8_t *data,uint32_t timeout)
{
	uint32_t Runsystick = get_tickcnt();
	while(__BUF_IS_EMPTY(ring_buf.rx_head, ring_buf.rx_tail))
	{
		//判断是否有数据，超过设定的时间没收到返回0
		if((get_tickcnt() - Runsystick )> timeout)	// 1s 时间完成.
		{
		  return -1;
		}
	}
	
	if(!(__BUF_IS_EMPTY(ring_buf.rx_head, ring_buf.rx_tail)))
	{
		*data = ring_buf.rx[ring_buf.rx_tail];
		__BUF_INCR(ring_buf.rx_tail);
	}
	else
	{
		return -1;
	}
}
