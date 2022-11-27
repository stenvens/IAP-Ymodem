/* BSP头文件 */
#include "bsp_uart.h"

/* 官方库头文件 */
#ifdef USE_UART_LIB
#include "uart.h"
#endif


/*********************************************** UART1 ***********************************************/
uint8_t uart1_rx_buf[UART1_BUF_MAXIMUM] = {0}; //USART1接收缓冲区
uint16_t uart1_rx_cnt = 0; //USART1当前接收长度

/*********************************************************************
* @brief  ：UART1的IO初始化	 
*********************************************************************/
static void bsp_uart1_gpio_init(void)
{
    stc_gpio_config_t stcGpioCfg; //GPIO端口配置结构体定义
    DDL_ZERO_STRUCT(stcGpioCfg); //清除结构体值为0
	
	Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);  //打开GPIO外设时钟门控
	
    stcGpioCfg.enDir = GpioDirOut; //输出
    Gpio_Init(UART1_TX_PORT, UART1_TX_PIN, &stcGpioCfg); //TX
    Gpio_SetAfMode(UART1_TX_PORT, UART1_TX_PIN, GpioAf1); //设置复用输出
	
    stcGpioCfg.enDir = GpioDirIn; //输入
    Gpio_Init(UART1_RX_PORT, UART1_RX_PIN, &stcGpioCfg); //RX
    Gpio_SetAfMode(UART1_RX_PORT, UART1_RX_PIN, GpioAf1); //设置复用输出
}

/*********************************************************************
* @brief  ：UART1的IO反初始化 
*********************************************************************/
static void bsp_uart1_gpio_deinit(void)
{
    stc_gpio_config_t stcGpioCfg; //GPIO端口配置结构体定义
    DDL_ZERO_STRUCT(stcGpioCfg); //清除结构体值为0
	
	Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);  //打开GPIO外设时钟门控
	
    stcGpioCfg.enDir = GpioDirIn; //输入
    Gpio_Init(UART1_TX_PORT, UART1_TX_PIN, &stcGpioCfg); //TX
	
    stcGpioCfg.enDir = GpioDirIn; //输入
    Gpio_Init(UART1_RX_PORT, UART1_RX_PIN, &stcGpioCfg); //RX
	
	M0P_UART1->SCON_f.REN = 0u; //禁止使能接收功能
	M0P_UART1->SCON_f.RCIE = 0u;
}

/*********************************************************************
* @brief  ：UART1反初始化 
*********************************************************************/
void bsp_uart1_deinit(void)
{
	bsp_uart1_gpio_deinit();
}

#ifdef USE_UART_LIB
/************************************ 库函数版 ************************************/
//函数声明
void TxIntCallback(void);
void RxIntCallback(void);
void ErrIntCallback(void);
void PErrIntCallBack(void);
void CtsIntCallBack(void);

//UART1初始化
void bsp_uart1_init(void)
{
	uint32_t u32Scnt = 0;
	stc_uart_config_t    stcConfig; //串口配置结构体
    stc_uart_irq_cb_t    stcUartIrqCb; //串口中断结构体
    stc_uart_baud_t      stcBaud; //串口波特率结构体
	//en_uart_mmdorck_t enTb8;
	stc_uart_multimode_t stcMulti;
	
	DDL_ZERO_STRUCT(stcConfig); //清除结构体为0
    DDL_ZERO_STRUCT(stcUartIrqCb);
    DDL_ZERO_STRUCT(stcBaud);
	DDL_ZERO_STRUCT(stcMulti);
	
	bsp_uart1_gpio_init(); //IO配置
		
	Sysctrl_SetPeripheralGate(SysctrlPeripheralUart1,TRUE); //开启UART1时钟
	
	stcUartIrqCb.pfnRxIrqCb   = RxIntCallback; //绑定回调函数
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
	stcConfig.enRunMode = UartMode1; //4种模式 (非校验)
    stcConfig.enStopBit = Uart1bit; //1位停止位
	
	stcMulti.enMulti_mode = UartNormal; //正常工作模式
	Uart_SetMultiMode(UARTCH1,&stcMulti);//多主机单独配置

    Uart_Init(UARTCH1, &stcConfig); //初始化USART1
	
	Uart_SetClkDiv(UARTCH1, Uart8Or16Div); //设置UART1时钟频率
    stcBaud.u32Pclk = Sysctrl_GetPClkFreq();
    stcBaud.enRunMode = UartMode3;
    stcBaud.u32Baud = UART1_BAUD; //装入波特率
    u32Scnt = Uart_CalScnt(UARTCH1, &stcBaud); //获取转换后的波特率设置值
    Uart_SetBaud(UARTCH1, u32Scnt); //设置波特率
	
	Uart_ClrStatus(UARTCH1, UartTC); //清除发送中断状态
	Uart_ClrStatus(UARTCH1, UartRC); //清除接收中断状态
	Uart_EnableFunc(UARTCH1, UartRx); //使能接收功能
	Uart_EnableIrq(UARTCH1, UartRxIrq);
}

//UART1 发送中断回调函数
void TxIntCallback(void)
{     
}

//UART1 接收中断回调函数(不需要清除状态和读取状态)
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

//UART1 错误中断回调函数
void ErrIntCallback(void)
{
}

//UART1 错误中断回调函数
void PErrIntCallBack(void)
{
}

//UART1 流控中断回调函数
void CtsIntCallBack(void)
{
}

//USART1发送字符
void bsp_uart1_send_byte(uint8_t data)
{
	Uart_SendData(UARTCH1, data);
}

//USART1发送
void bsp_uart1_send(uint8_t *buf, uint16_t length)
{
	uint16_t i = 0;
	for(i=0; i<length; i++)
	{
		bsp_uart1_send_byte(buf[i]);
	}
}
/************************************ 库函数版 ************************************/
#else
/************************************ 寄存器版 ************************************/
/*********************************************************************
* @brief  ：UART1的Mode3模式计算波特率
*---------------------------------------------------------------------
* @param  ：uint32_t baud, 		波特率
*			uint32_t pclk,		PCLK时钟频率
*---------------------------------------------------------------------
* @retval ：uint32_t u32Scnt,	波特率设置计数值
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
* @brief  ：UART1初始化	 
*********************************************************************/
void bsp_uart1_init(void)
{
	uint32_t u32Scnt = 0;
	uint32_t u32Pclk = 0;
	IRQn_Type enIrqIndex;
	
	bsp_uart1_gpio_init(); //IO配置
		
	Sysctrl_SetPeripheralGate(SysctrlPeripheralUart1,TRUE); //开启UART1时钟  
	
	M0P_UART1->SCON_f.SM = 1u; ///<模式1
	M0P_UART1->SCON_f.STOPBIT = 0u; ///<1位停止位 
	
//	enIrqIndex = (IRQn_Type)(UART1_IRQn); //uart1
//    NVIC_ClearPendingIRQ(enIrqIndex);
//    NVIC_SetPriority(enIrqIndex,IrqLevel3);
//    NVIC_EnableIRQ(enIrqIndex); //使能中断
    
	M0P_UART1->SCON_f.OVER = 1u,///<模式0无效，模式1/3为8分频，模式2为16分频 设置UART1时钟频率
    
	u32Pclk = Sysctrl_GetPClkFreq();
	u32Scnt = uart1_calulate_scnt(UART1_BAUD, u32Pclk); //装入波特率 获取转换后的波特率设置值
	M0P_UART1->SCNT = u32Scnt; //设置波特率
	
//	M0P_UART1->ICR_f.TCCF = 0; //清除发送中断状态
//	M0P_UART1->ICR_f.RCCF = 0; //清除接收中断状态

	M0P_UART1->SCON_f.REN = 1u; //使能接收功能
//	M0P_UART1->SCON_f.RCIE = 1u; 
}

/*********************************************************************
* @brief  ：UART1接收字节数据
*---------------------------------------------------------------------
* @param  ：none
*---------------------------------------------------------------------
* @retval ：接收到一个字节数据
*********************************************************************/
uint8_t _Uart1_ReceiveData(void)
{
    return (M0P_UART1->SBUF_f.DATA);
}

/*********************************************************************
* @brief  ：UART1判断状态
*---------------------------------------------------------------------
* @param  ：none
*---------------------------------------------------------------------
* @retval ：布尔值
*********************************************************************/
boolean_t _Uart1_GetStatus(void)
{
    boolean_t bRetVal = FALSE;
        
	bRetVal = (M0P_UART1->ISR_f.TC == 1) ? TRUE : FALSE;

    return bRetVal;
}

/*********************************************************************
* @brief  ：USART1发送字符
*---------------------------------------------------------------------
* @param  ：uint8_t data,	要发送的一个字节数据
*---------------------------------------------------------------------
* @retval ：none
*********************************************************************/
void bsp_uart1_send_byte(uint8_t data)
{
	M0P_UART1->SBUF_f.DATA = data;
    while(FALSE == _Uart1_GetStatus())
    {}
    M0P_UART1->ICR_f.TCCF = 0; 
}

/*********************************************************************
* @brief  ：USART1发送
*---------------------------------------------------------------------
* @param  ：uint8_t *buf, 		要发送的数组内容
*			uint16_t length,	发送数据的字节长度
*---------------------------------------------------------------------
* @retval ：none
*********************************************************************/
void bsp_uart1_send(uint8_t *buf, uint16_t length)
{
	uint16_t i = 0;
	for(i=0; i<length; i++)
	{
		bsp_uart1_send_byte(buf[i]);
	}
}
/************************************ 寄存器版 ************************************/
#endif
/*********************************************** UART1 ***********************************************/


/*********************************************** UART0 ***********************************************/
uint8_t uart0_rx_buf[UART0_BUF_MAXIMUM] = {0}; //USART0接收缓冲区
uint16_t uart0_rx_cnt = 0; //USART0当前接收长度

//UART0的IO初始化
static void bsp_uart0_gpio_init(void)
{
    stc_gpio_config_t stcGpioCfg; //GPIO端口配置结构体定义
    DDL_ZERO_STRUCT(stcGpioCfg); //清除结构体值为0
	
	Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);  //打开GPIO外设时钟门控
	
    stcGpioCfg.enDir = GpioDirOut; //输出
    Gpio_Init(UART0_TX_PORT, UART0_TX_PIN, &stcGpioCfg); //TX
    Gpio_SetAfMode(UART0_TX_PORT, UART0_TX_PIN, GpioAf7); //设置复用输出 (AF具体参照用户手册)
	
    stcGpioCfg.enDir = GpioDirIn; //输入
    Gpio_Init(UART0_RX_PORT, UART0_RX_PIN, &stcGpioCfg); //RX
    Gpio_SetAfMode(UART0_RX_PORT, UART0_RX_PIN, GpioAf7); //设置复用输出
}

//UART0的IO反初始化
static void bsp_uart0_gpio_deinit(void)
{
    stc_gpio_config_t stcGpioCfg; //GPIO端口配置结构体定义
    DDL_ZERO_STRUCT(stcGpioCfg); //清除结构体值为0
	
	Sysctrl_SetPeripheralGate(SysctrlPeripheralGpio,TRUE);  //打开GPIO外设时钟门控
	
    stcGpioCfg.enDir = GpioDirIn; //输出
    Gpio_Init(UART0_TX_PORT, UART0_TX_PIN, &stcGpioCfg); //TX
	
    stcGpioCfg.enDir = GpioDirIn; //输入
    Gpio_Init(UART0_RX_PORT, UART0_RX_PIN, &stcGpioCfg); //RX
	
	M0P_UART0->SCON_f.REN = 0u; //使能接收功能
	M0P_UART0->SCON_f.RCIE = 0u; 
}

/*********************************************************************
* @brief  ：UART0反初始化 
*********************************************************************/
void bsp_uart0_deinit(void)
{
	bsp_uart0_gpio_deinit();
}

#ifdef USE_UART_LIB
/************************************ 库函数版 ************************************/
//函数声明
void UART0_TxIntCallback(void);
void UART0_RxIntCallback(void);
void UART0_ErrIntCallback(void);
void UART0_PErrIntCallBack(void);
void UART0_CtsIntCallBack(void);

//UART0初始化
void USART0_Init(void)
{
	uint32_t u32Scnt = 0;
	stc_uart_config_t    stcConfig; //串口配置结构体
    stc_uart_irq_cb_t    stcUartIrqCb; //串口中断结构体
    stc_uart_baud_t      stcBaud; //串口波特率结构体
	en_uart_mmdorck_t enTb8;
	stc_uart_multimode_t stcMulti;
	
	DDL_ZERO_STRUCT(stcConfig); //清除结构体为0
    DDL_ZERO_STRUCT(stcUartIrqCb);
    DDL_ZERO_STRUCT(stcBaud);
	DDL_ZERO_STRUCT(stcMulti);
	
	bsp_uart0_gpio_init(); //IO配置
		
	Sysctrl_SetPeripheralGate(SysctrlPeripheralUart0,TRUE); //开启UART0时钟
	
	stcUartIrqCb.pfnRxIrqCb   = UART0_RxIntCallback; //绑定回调函数
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
	stcConfig.enRunMode = UartMode1; //4种模式 (无校验)
    stcConfig.enStopBit = Uart1bit; //1位停止位
	
	stcMulti.enMulti_mode = UartNormal; //正常工作模式
	Uart_SetMultiMode(UARTCH0,&stcMulti);//多主机单独配置

    Uart_Init(UARTCH0, &stcConfig); //初始化USART0
	
	Uart_SetClkDiv(UARTCH0, Uart8Or16Div); //设置UART0时钟频率
    stcBaud.u32Pclk = Sysctrl_GetPClkFreq();
    stcBaud.enRunMode = UartMode3;
    stcBaud.u32Baud = UART0_BAUD; //装入波特率
    u32Scnt = Uart_CalScnt(UARTCH0, &stcBaud); //获取转换后的波特率设置值
    Uart_SetBaud(UARTCH0, u32Scnt); //设置波特率
	
	Uart_ClrStatus(UARTCH0, UartTC); //清除发送中断状态
	Uart_ClrStatus(UARTCH0, UartRC); //清除接收中断状态
	Uart_EnableFunc(UARTCH0, UartRx); //使能接收功能
	Uart_EnableIrq(UARTCH0, UartRxIrq);
}

//UART0 发送中断回调函数
void UART0_TxIntCallback(void)
{     
}

//UART0 接收中断回调函数(不需要清除状态和读取状态)
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

//UART0 错误中断回调函数
void UART0_ErrIntCallback(void)
{
}

//UART0 错误中断回调函数
void UART0_PErrIntCallBack(void)
{
}

//UART0 流控中断回调函数
void UART0_CtsIntCallBack(void)
{
}

//USART0发送字符
void bsp_uart0_send_byte(uint8_t data)
{
	Uart_SendData(UARTCH0, data);
}

//USART0发送
void bsp_uart0_send(uint8_t *buf, uint16_t length)
{
	uint16_t i = 0;
	for(i=0; i<length; i++)
	{
		bsp_uart0_send_byte(buf[i]);
	}
}
/************************************ 库函数版 ************************************/
#else
/************************************ 寄存器版 ************************************/
/*********************************************************************
* @brief  ：UART0的Mode3模式计算波特率
*---------------------------------------------------------------------
* @param  ：uint32_t baud, 		波特率
*			uint32_t pclk,		PCLK时钟频率
*---------------------------------------------------------------------
* @retval ：uint32_t u32Scnt,	波特率设置计数值
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

//UART0初始化
void bsp_uart0_init(void)
{
	uint32_t u32Scnt = 0;
	uint32_t u32Pclk = 0;
	IRQn_Type enIrqIndex;
	
	bsp_uart0_gpio_init(); //IO配置
		
	Sysctrl_SetPeripheralGate(SysctrlPeripheralUart0,TRUE); //开启UART0时钟
	 
	M0P_UART0->SCON_f.SM = 1u; ///<模式1
	M0P_UART0->SCON_f.STOPBIT = 0u; ///<1位停止位 
	
	enIrqIndex = (IRQn_Type)(UART0_IRQn); //uart0
    NVIC_ClearPendingIRQ(enIrqIndex);
    NVIC_SetPriority(enIrqIndex, IrqLevel2);
    NVIC_EnableIRQ(enIrqIndex); //使能中断
    
	M0P_UART0->SCON_f.OVER = 1u,///<模式0无效，模式1/3为8分频，模式2为16分频 设置UART1时钟频率
    
	u32Pclk = Sysctrl_GetPClkFreq();
	u32Scnt = uart0_calulate_scnt(UART0_BAUD, u32Pclk); //装入波特率 获取转换后的波特率设置值
	M0P_UART0->SCNT = u32Scnt; //设置波特率
	
	M0P_UART0->ICR_f.TCCF = 0; //清除发送中断状态
	M0P_UART0->ICR_f.RCCF = 0; //清除接收中断状态

	M0P_UART0->SCON_f.REN = 1u; //使能接收功能
	M0P_UART0->SCON_f.RCIE = 1u; 
}

//UART0接收字节数据
uint8_t _Uart0_ReceiveData()
{
    return (M0P_UART0->SBUF_f.DATA);
}

//UART0判断状态
boolean_t _Uart0_GetStatus(void)
{
    boolean_t bRetVal = FALSE;
        
	bRetVal = (M0P_UART0->ISR_f.TC == 1) ? TRUE : FALSE;

    return bRetVal;
}

//USART0发送字符
void bsp_uart0_send_byte(uint8_t data)
{
	M0P_UART0->SBUF_f.DATA = data;
    while(FALSE == _Uart0_GetStatus())
    {}
    M0P_UART0->ICR_f.TCCF = 0; 
}

//USART0发送
void bsp_uart0_send(uint8_t *buf, uint16_t length)
{
	uint16_t i = 0;
	for(i=0; i<length; i++)
	{
		bsp_uart0_send_byte(buf[i]);
	}
}
/************************************ 寄存器版 ************************************/
#endif
/*********************************************** UART0 ***********************************************/


#ifndef USE_UART_LIB
/*********************************************************************
* @brief  ：UART接收中断
*---------------------------------------------------------------------
* @param  ：uint8_t u8Param,  串口中断索引(0/1)
*---------------------------------------------------------------------
* @retval ：none
*********************************************************************/
void Uart_IRQHandler(uint8_t u8Param)
{
	uint8_t res = 0;
	
	if(1 == M0P_UART1->ISR_f.RC) //UART1
    {
        M0P_UART1->ICR_f.RCCF = 0; //清除RC标志
		
		res = _Uart1_ReceiveData();
		uart1_rx_buf[uart1_rx_cnt++] = res;
		if(uart1_rx_cnt >= UART1_BUF_MAXIMUM)
		{
			uart1_rx_cnt = 0;
		}
    }
	else if(1 == M0P_UART0->ISR_f.RC) //UART0
    {
        M0P_UART0->ICR_f.RCCF = 0; //清除RC标志
		
		res = _Uart0_ReceiveData();
		uart0_rx_buf[uart0_rx_cnt++] = res;
		if(uart0_rx_cnt >= UART0_BUF_MAXIMUM)
		{
			uart0_rx_cnt = 0;
		}
    }
}
#endif

