#include "ring_buffer.h"
#include "bsp_systick.h"

 USART_RING_BUFFER ring_buf={0};
 /**
  * @brief   �����ζ���     
  * @note    ��  
  * @param   ��
  * @retval  ��
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
  * @brief   �����ζ���     
  * @note    ��  
  * @param   ��
  * @retval  ��
  */ 
char GetRingBufchar(uint8_t *data,uint32_t timeout)
{
	uint32_t Runsystick = get_tickcnt();
	uint32_t tick_cur = 0;
	while(__BUF_IS_EMPTY(ring_buf.rx_head, ring_buf.rx_tail))
	{
		tick_cur = get_tickcnt();
		//�ж��Ƿ������ݣ������趨��ʱ��û�յ�����0
		if((tick_cur - Runsystick )> timeout)	// 1s ʱ�����.
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
