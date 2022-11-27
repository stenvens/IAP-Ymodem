#include "ring_buffer.h"
#include "bsp_systick.h"

 USART_RING_BUFFER ring_buf={0};
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
	uint32_t tick_cur = 0;
	while(__BUF_IS_EMPTY(ring_buf.rx_head, ring_buf.rx_tail))
	{
		tick_cur = get_tickcnt();
		//判断是否有数据，超过设定的时间没收到返回0
		if((tick_cur - Runsystick )> timeout)	// 1s 时间完成.
		{
		  return 0;
		}
	}
	
	if(!(__BUF_IS_EMPTY(ring_buf.rx_head, ring_buf.rx_tail)))
	{
		*data = ring_buf.rx[ring_buf.rx_tail];
		__BUF_INCR(ring_buf.rx_tail);
		return 1;
	}
	else
	{
		return 0;
	}
}
