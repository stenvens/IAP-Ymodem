/* BSPͷ�ļ� */
#include "bsp_uart.h"

/* �ٷ���ͷ�ļ� */
#ifdef USE_UART_LIB
#include "uart.h"
#endif


/*********************************************** UART1 ***********************************************/
uint8_t uart1_rx_buf[UART1_BUF_MAXIMUM] = {0}; //USART1���ջ�����
uint16_t uart1_rx_cnt = 0; //USART1��ǰ���ճ���

/*********************************************************************
* @brief  ��UART1��IO��ʼ��	 
*********************************************************************/
static void bsp_uart1_gpio_init(void)
{
    stc_gpio_config_t stcGpioCfg; //GPIO�˿����ýṹ�嶨��
    DDL_ZERO_STRUCT(stcGpioCfg); //����ṹ��ֵΪ0
	
	Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);  //��GPIO����ʱ���ſ�
	
    stcGpioCfg.enDir = GpioDirOut; //���
    Gpio_Init(UART1_TX_PORT, UART1_TX_PIN, &stcGpioCfg); //TX
    Gpio_SetAfMode(UART1_TX_PORT, UART1_TX_PIN, GpioAf1); //���ø������
	
    stcGpioCfg.enDir = GpioDirIn; //����
    Gpio_Init(UART1_RX_PORT, UART1_RX_PIN, &stcGpioCfg); //RX
    Gpio_SetAfMode(UART1_RX_PORT, UART1_RX_PIN, GpioAf1); //���ø������
}

/*********************************************************************
* @brief  ��UART1��IO����ʼ�� 
*********************************************************************/
static void bsp_uart1_gpio_deinit(void)
{
    stc_gpio_config_t stcGpioCfg; //GPIO�˿����ýṹ�嶨��
    DDL_ZERO_STRUCT(stcGpioCfg); //����ṹ��ֵΪ0
	
	Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);  //��GPIO����ʱ���ſ�
	
    stcGpioCfg.enDir = GpioDirIn; //����
    Gpio_Init(UART1_TX_PORT, UART1_TX_PIN, &stcGpioCfg); //TX
	
    stcGpioCfg.enDir = GpioDirIn; //����
    Gpio_Init(UART1_RX_PORT, UART1_RX_PIN, &stcGpioCfg); //RX
	
	M0P_UART1->SCON_f.REN = 0u; //��ֹʹ�ܽ��չ���
	M0P_UART1->SCON_f.RCIE = 0u;
}

/*********************************************************************
* @brief  ��UART1����ʼ�� 
*********************************************************************/
void bsp_uart1_deinit(void)
{
	bsp_uart1_gpio_deinit();
}

#ifdef USE_UART_LIB
/************************************ �⺯���� ************************************/
//��������
void TxIntCallback(void);
void RxIntCallback(void);
void ErrIntCallback(void);
void PErrIntCallBack(void);
void CtsIntCallBack(void);

//UART1��ʼ��
void bsp_uart1_init(void)
{
	uint32_t u32Scnt = 0;
	stc_uart_config_t    stcConfig; //�������ýṹ��
    stc_uart_irq_cb_t    stcUartIrqCb; //�����жϽṹ��
    stc_uart_baud_t      stcBaud; //���ڲ����ʽṹ��
	//en_uart_mmdorck_t enTb8;
	stc_uart_multimode_t stcMulti;
	
	DDL_ZERO_STRUCT(stcConfig); //����ṹ��Ϊ0
    DDL_ZERO_STRUCT(stcUartIrqCb);
    DDL_ZERO_STRUCT(stcBaud);
	DDL_ZERO_STRUCT(stcMulti);
	
	bsp_uart1_gpio_init(); //IO����
		
	Sysctrl_SetPeripheralGate(SysctrlPeripheralUart1,TRUE); //����UART1ʱ��
	
	stcUartIrqCb.pfnRxIrqCb   = RxIntCallback; //�󶨻ص�����
    stcUartIrqCb.pfnTxIrqCb   = TxIntCallback;
    stcUartIrqCb.pfnRxFEIrqCb = ErrIntCallback;
    stcUartIrqCb.pfnPEIrqCb   = PErrIntCallBack;
    stcUartIrqCb.pfnCtsIrqCb  = CtsIntCallBack;
	stcConfig.pstcIrqCb = &stcUartIrqCb;
    stcConfig.bTouchNvic = TRUE;
  
	if(TRUE == stcConfig.bTouchNvic)
	{
		EnableNvic(UART1_IRQn,IrqLevel3,TRUE);
	}
	stcConfig.enRunMode = UartMode1; //4��ģʽ (��У��)
    stcConfig.enStopBit = Uart1bit; //1λֹͣλ
	
	stcMulti.enMulti_mode = UartNormal; //��������ģʽ
	Uart_SetMultiMode(UARTCH1,&stcMulti);//��������������

    Uart_Init(UARTCH1, &stcConfig); //��ʼ��USART1
	
	Uart_SetClkDiv(UARTCH1, Uart8Or16Div); //����UART1ʱ��Ƶ��
    stcBaud.u32Pclk = Sysctrl_GetPClkFreq();
    stcBaud.enRunMode = UartMode3;
    stcBaud.u32Baud = UART1_BAUD; //װ�벨����
    u32Scnt = Uart_CalScnt(UARTCH1, &stcBaud); //��ȡת����Ĳ���������ֵ
    Uart_SetBaud(UARTCH1, u32Scnt); //���ò�����
	
	Uart_ClrStatus(UARTCH1, UartTC); //��������ж�״̬
	Uart_ClrStatus(UARTCH1, UartRC); //��������ж�״̬
	Uart_EnableFunc(UARTCH1, UartRx); //ʹ�ܽ��չ���
	Uart_EnableIrq(UARTCH1, UartRxIrq);
}

//UART1 �����жϻص�����
void TxIntCallback(void)
{     
}

//UART1 �����жϻص�����(����Ҫ���״̬�Ͷ�ȡ״̬)
void RxIntCallback(void)
{
	uint8_t res = 0;

	res = Uart_ReceiveData(UARTCH1);
	uart1_rx_buf[uart1_rx_cnt++] = res;
	if(uart1_rx_cnt >= UART1_BUF_MAXIMUM)
	{
		uart1_rx_cnt = 0;
	}
}

//UART1 �����жϻص�����
void ErrIntCallback(void)
{
}

//UART1 �����жϻص�����
void PErrIntCallBack(void)
{
}

//UART1 �����жϻص�����
void CtsIntCallBack(void)
{
}

//USART1�����ַ�
void bsp_uart1_send_byte(uint8_t data)
{
	Uart_SendData(UARTCH1, data);
}

//USART1����
void bsp_uart1_send(uint8_t *buf, uint16_t length)
{
	uint16_t i = 0;
	for(i=0; i<length; i++)
	{
		bsp_uart1_send_byte(buf[i]);
	}
}
/************************************ �⺯���� ************************************/
#else
/************************************ �Ĵ����� ************************************/
/*********************************************************************
* @brief  ��UART1��Mode3ģʽ���㲨����
*---------------------------------------------------------------------
* @param  ��uint32_t baud, 		������
*			uint32_t pclk,		PCLKʱ��Ƶ��
*---------------------------------------------------------------------
* @retval ��uint32_t u32Scnt,	���������ü���ֵ
*********************************************************************/
static uint32_t uart1_calulate_scnt(uint32_t baud, uint32_t pclk)
{
	uint32_t u32Scnt = 0;
    uint8_t u8Over = 0;
    u8Over = M0P_UART1->SCON_f.OVER;
	if(0 == u8Over)
	{
		u8Over = 16;
	}
	else
	{
		u8Over = 8;
	}
	u32Scnt = pclk/(baud*u8Over);
    return u32Scnt;
}

/*********************************************************************
* @brief  ��UART1��ʼ��	 
*********************************************************************/
void bsp_uart1_init(void)
{
	uint32_t u32Scnt = 0;
	uint32_t u32Pclk = 0;
	IRQn_Type enIrqIndex;
	
	bsp_uart1_gpio_init(); //IO����
		
	Sysctrl_SetPeripheralGate(SysctrlPeripheralUart1,TRUE); //����UART1ʱ��  
	
	M0P_UART1->SCON_f.SM = 1u; ///<ģʽ1
	M0P_UART1->SCON_f.STOPBIT = 0u; ///<1λֹͣλ 
	
//	enIrqIndex = (IRQn_Type)(UART1_IRQn); //uart1
//    NVIC_ClearPendingIRQ(enIrqIndex);
//    NVIC_SetPriority(enIrqIndex,IrqLevel3);
//    NVIC_EnableIRQ(enIrqIndex); //ʹ���ж�
    
	M0P_UART1->SCON_f.OVER = 1u,///<ģʽ0��Ч��ģʽ1/3Ϊ8��Ƶ��ģʽ2Ϊ16��Ƶ ����UART1ʱ��Ƶ��
    
	u32Pclk = Sysctrl_GetPClkFreq();
	u32Scnt = uart1_calulate_scnt(UART1_BAUD, u32Pclk); //װ�벨���� ��ȡת����Ĳ���������ֵ
	M0P_UART1->SCNT = u32Scnt; //���ò�����
	
//	M0P_UART1->ICR_f.TCCF = 0; //��������ж�״̬
//	M0P_UART1->ICR_f.RCCF = 0; //��������ж�״̬

	M0P_UART1->SCON_f.REN = 1u; //ʹ�ܽ��չ���
//	M0P_UART1->SCON_f.RCIE = 1u; 
}

/*********************************************************************
* @brief  ��UART1�����ֽ�����
*---------------------------------------------------------------------
* @param  ��none
*---------------------------------------------------------------------
* @retval �����յ�һ���ֽ�����
*********************************************************************/
uint8_t _Uart1_ReceiveData(void)
{
    return (M0P_UART1->SBUF_f.DATA);
}

/*********************************************************************
* @brief  ��UART1�ж�״̬
*---------------------------------------------------------------------
* @param  ��none
*---------------------------------------------------------------------
* @retval ������ֵ
*********************************************************************/
boolean_t _Uart1_GetStatus(void)
{
    boolean_t bRetVal = FALSE;
        
	bRetVal = (M0P_UART1->ISR_f.TC == 1) ? TRUE : FALSE;

    return bRetVal;
}

/*********************************************************************
* @brief  ��USART1�����ַ�
*---------------------------------------------------------------------
* @param  ��uint8_t data,	Ҫ���͵�һ���ֽ�����
*---------------------------------------------------------------------
* @retval ��none
*********************************************************************/
void bsp_uart1_send_byte(uint8_t data)
{
	M0P_UART1->SBUF_f.DATA = data;
    while(FALSE == _Uart1_GetStatus())
    {}
    M0P_UART1->ICR_f.TCCF = 0; 
}

/*********************************************************************
* @brief  ��USART1����
*---------------------------------------------------------------------
* @param  ��uint8_t *buf, 		Ҫ���͵���������
*			uint16_t length,	�������ݵ��ֽڳ���
*---------------------------------------------------------------------
* @retval ��none
*********************************************************************/
void bsp_uart1_send(uint8_t *buf, uint16_t length)
{
	uint16_t i = 0;
	for(i=0; i<length; i++)
	{
		bsp_uart1_send_byte(buf[i]);
	}
}
/************************************ �Ĵ����� ************************************/
#endif
/*********************************************** UART1 ***********************************************/


/*********************************************** UART0 ***********************************************/
uint8_t uart0_rx_buf[UART0_BUF_MAXIMUM] = {0}; //USART0���ջ�����
uint16_t uart0_rx_cnt = 0; //USART0��ǰ���ճ���

//UART0��IO��ʼ��
static void bsp_uart0_gpio_init(void)
{
    stc_gpio_config_t stcGpioCfg; //GPIO�˿����ýṹ�嶨��
    DDL_ZERO_STRUCT(stcGpioCfg); //����ṹ��ֵΪ0
	
	Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);  //��GPIO����ʱ���ſ�
	
    stcGpioCfg.enDir = GpioDirOut; //���
    Gpio_Init(UART0_TX_PORT, UART0_TX_PIN, &stcGpioCfg); //TX
    Gpio_SetAfMode(UART0_TX_PORT, UART0_TX_PIN, GpioAf7); //���ø������ (AF��������û��ֲ�)
	
    stcGpioCfg.enDir = GpioDirIn; //����
    Gpio_Init(UART0_RX_PORT, UART0_RX_PIN, &stcGpioCfg); //RX
    Gpio_SetAfMode(UART0_RX_PORT, UART0_RX_PIN, GpioAf7); //���ø������
}

//UART0��IO����ʼ��
static void bsp_uart0_gpio_deinit(void)
{
    stc_gpio_config_t stcGpioCfg; //GPIO�˿����ýṹ�嶨��
    DDL_ZERO_STRUCT(stcGpioCfg); //����ṹ��ֵΪ0
	
	Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);  //��GPIO����ʱ���ſ�
	
    stcGpioCfg.enDir = GpioDirIn; //���
    Gpio_Init(UART0_TX_PORT, UART0_TX_PIN, &stcGpioCfg); //TX
	
    stcGpioCfg.enDir = GpioDirIn; //����
    Gpio_Init(UART0_RX_PORT, UART0_RX_PIN, &stcGpioCfg); //RX
	
	M0P_UART0->SCON_f.REN = 0u; //ʹ�ܽ��չ���
	M0P_UART0->SCON_f.RCIE = 0u; 
}

/*********************************************************************
* @brief  ��UART0����ʼ�� 
*********************************************************************/
void bsp_uart0_deinit(void)
{
	bsp_uart0_gpio_deinit();
}

#ifdef USE_UART_LIB
/************************************ �⺯���� ************************************/
//��������
void UART0_TxIntCallback(void);
void UART0_RxIntCallback(void);
void UART0_ErrIntCallback(void);
void UART0_PErrIntCallBack(void);
void UART0_CtsIntCallBack(void);

//UART0��ʼ��
void USART0_Init(void)
{
	uint32_t u32Scnt = 0;
	stc_uart_config_t    stcConfig; //�������ýṹ��
    stc_uart_irq_cb_t    stcUartIrqCb; //�����жϽṹ��
    stc_uart_baud_t      stcBaud; //���ڲ����ʽṹ��
	en_uart_mmdorck_t enTb8;
	stc_uart_multimode_t stcMulti;
	
	DDL_ZERO_STRUCT(stcConfig); //����ṹ��Ϊ0
    DDL_ZERO_STRUCT(stcUartIrqCb);
    DDL_ZERO_STRUCT(stcBaud);
	DDL_ZERO_STRUCT(stcMulti);
	
	bsp_uart0_gpio_init(); //IO����
		
	Sysctrl_SetPeripheralGate(SysctrlPeripheralUart0,TRUE); //����UART0ʱ��
	
	stcUartIrqCb.pfnRxIrqCb   = UART0_RxIntCallback; //�󶨻ص�����
    stcUartIrqCb.pfnTxIrqCb   = UART0_TxIntCallback;
    stcUartIrqCb.pfnRxFEIrqCb = UART0_ErrIntCallback;
    stcUartIrqCb.pfnPEIrqCb   = UART0_PErrIntCallBack;
    stcUartIrqCb.pfnCtsIrqCb  = UART0_CtsIntCallBack;
	stcConfig.pstcIrqCb = &stcUartIrqCb;
    stcConfig.bTouchNvic = TRUE;
  
	if(TRUE == stcConfig.bTouchNvic)
	{
		EnableNvic(UART0_IRQn,IrqLevel3,TRUE);
	}
	stcConfig.enRunMode = UartMode1; //4��ģʽ (��У��)
    stcConfig.enStopBit = Uart1bit; //1λֹͣλ
	
	stcMulti.enMulti_mode = UartNormal; //��������ģʽ
	Uart_SetMultiMode(UARTCH0,&stcMulti);//��������������

    Uart_Init(UARTCH0, &stcConfig); //��ʼ��USART0
	
	Uart_SetClkDiv(UARTCH0, Uart8Or16Div); //����UART0ʱ��Ƶ��
    stcBaud.u32Pclk = Sysctrl_GetPClkFreq();
    stcBaud.enRunMode = UartMode3;
    stcBaud.u32Baud = UART0_BAUD; //װ�벨����
    u32Scnt = Uart_CalScnt(UARTCH0, &stcBaud); //��ȡת����Ĳ���������ֵ
    Uart_SetBaud(UARTCH0, u32Scnt); //���ò�����
	
	Uart_ClrStatus(UARTCH0, UartTC); //��������ж�״̬
	Uart_ClrStatus(UARTCH0, UartRC); //��������ж�״̬
	Uart_EnableFunc(UARTCH0, UartRx); //ʹ�ܽ��չ���
	Uart_EnableIrq(UARTCH0, UartRxIrq);
}

//UART0 �����жϻص�����
void UART0_TxIntCallback(void)
{     
}

//UART0 �����жϻص�����(����Ҫ���״̬�Ͷ�ȡ״̬)
void UART0_RxIntCallback(void)
{
	uint8_t res = 0;

	res = Uart_ReceiveData(UARTCH0);
	uart0_rx_buf[uart0_rx_cnt++] = res;
	if(uart0_rx_cnt >= UART0_BUF_MAXIMUM)
	{
		uart0_rx_cnt = 0;
	}
}

//UART0 �����жϻص�����
void UART0_ErrIntCallback(void)
{
}

//UART0 �����жϻص�����
void UART0_PErrIntCallBack(void)
{
}

//UART0 �����жϻص�����
void UART0_CtsIntCallBack(void)
{
}

//USART0�����ַ�
void bsp_uart0_send_byte(uint8_t data)
{
	Uart_SendData(UARTCH0, data);
}

//USART0����
void bsp_uart0_send(uint8_t *buf, uint16_t length)
{
	uint16_t i = 0;
	for(i=0; i<length; i++)
	{
		bsp_uart0_send_byte(buf[i]);
	}
}
/************************************ �⺯���� ************************************/
#else
/************************************ �Ĵ����� ************************************/
/*********************************************************************
* @brief  ��UART0��Mode3ģʽ���㲨����
*---------------------------------------------------------------------
* @param  ��uint32_t baud, 		������
*			uint32_t pclk,		PCLKʱ��Ƶ��
*---------------------------------------------------------------------
* @retval ��uint32_t u32Scnt,	���������ü���ֵ
*********************************************************************/
static uint32_t uart0_calulate_scnt(uint32_t baud, uint32_t pclk)
{
	uint32_t u32Scnt = 0;
    uint8_t u8Over = 0;
    u8Over = M0P_UART0->SCON_f.OVER;
	if(0 == u8Over)
	{
		u8Over = 16;
	}
	else
	{
		u8Over = 8;
	}
	u32Scnt = pclk/(baud*u8Over);
    return u32Scnt;
}

//UART0��ʼ��
void bsp_uart0_init(void)
{
	uint32_t u32Scnt = 0;
	uint32_t u32Pclk = 0;
	IRQn_Type enIrqIndex;
	
	bsp_uart0_gpio_init(); //IO����
		
	Sysctrl_SetPeripheralGate(SysctrlPeripheralUart0,TRUE); //����UART0ʱ��
	 
	M0P_UART0->SCON_f.SM = 1u; ///<ģʽ1
	M0P_UART0->SCON_f.STOPBIT = 0u; ///<1λֹͣλ 
	
	enIrqIndex = (IRQn_Type)(UART0_IRQn); //uart0
    NVIC_ClearPendingIRQ(enIrqIndex);
    NVIC_SetPriority(enIrqIndex, IrqLevel2);
    NVIC_EnableIRQ(enIrqIndex); //ʹ���ж�
    
	M0P_UART0->SCON_f.OVER = 1u,///<ģʽ0��Ч��ģʽ1/3Ϊ8��Ƶ��ģʽ2Ϊ16��Ƶ ����UART1ʱ��Ƶ��
    
	u32Pclk = Sysctrl_GetPClkFreq();
	u32Scnt = uart0_calulate_scnt(UART0_BAUD, u32Pclk); //װ�벨���� ��ȡת����Ĳ���������ֵ
	M0P_UART0->SCNT = u32Scnt; //���ò�����
	
	M0P_UART0->ICR_f.TCCF = 0; //��������ж�״̬
	M0P_UART0->ICR_f.RCCF = 0; //��������ж�״̬

	M0P_UART0->SCON_f.REN = 1u; //ʹ�ܽ��չ���
	M0P_UART0->SCON_f.RCIE = 1u; 
}

//UART0�����ֽ�����
uint8_t _Uart0_ReceiveData()
{
    return (M0P_UART0->SBUF_f.DATA);
}

//UART0�ж�״̬
boolean_t _Uart0_GetStatus(void)
{
    boolean_t bRetVal = FALSE;
        
	bRetVal = (M0P_UART0->ISR_f.TC == 1) ? TRUE : FALSE;

    return bRetVal;
}

//USART0�����ַ�
void bsp_uart0_send_byte(uint8_t data)
{
	M0P_UART0->SBUF_f.DATA = data;
    while(FALSE == _Uart0_GetStatus())
    {}
    M0P_UART0->ICR_f.TCCF = 0; 
}

//USART0����
void bsp_uart0_send(uint8_t *buf, uint16_t length)
{
	uint16_t i = 0;
	for(i=0; i<length; i++)
	{
		bsp_uart0_send_byte(buf[i]);
	}
}
/************************************ �Ĵ����� ************************************/
#endif
/*********************************************** UART0 ***********************************************/


#ifndef USE_UART_LIB
/*********************************************************************
* @brief  ��UART�����ж�
*---------------------------------------------------------------------
* @param  ��uint8_t u8Param,  �����ж�����(0/1)
*---------------------------------------------------------------------
* @retval ��none
*********************************************************************/
void Uart_IRQHandler(uint8_t u8Param)
{
	uint8_t res = 0;
	
	if(1 == M0P_UART1->ISR_f.RC) //UART1
    {
        M0P_UART1->ICR_f.RCCF = 0; //���RC��־
		
		res = _Uart1_ReceiveData();
		uart1_rx_buf[uart1_rx_cnt++] = res;
		if(uart1_rx_cnt >= UART1_BUF_MAXIMUM)
		{
			uart1_rx_cnt = 0;
		}
    }
	else if(1 == M0P_UART0->ISR_f.RC) //UART0
    {
        M0P_UART0->ICR_f.RCCF = 0; //���RC��־
		
		res = _Uart0_ReceiveData();
		uart0_rx_buf[uart0_rx_cnt++] = res;
		if(uart0_rx_cnt >= UART0_BUF_MAXIMUM)
		{
			uart0_rx_cnt = 0;
		}
    }
}
#endif

