/******************************************************************************
* Copyright (C) 2018, Huada Semiconductor Co.,Ltd All rights reserved.
*
* This software is owned and published by:
* Huada Semiconductor Co.,Ltd ("HDSC").
*
* BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
* BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
*
* This software contains source code for use with HDSC
* components. This software is licensed by HDSC to be adapted only
* for use in systems utilizing HDSC components. HDSC shall not be
* responsible for misuse or illegal use of this software for devices not
* supported herein. HDSC is providing this software "AS IS" and will
* not be responsible for issues arising from incorrect user implementation
* of the software.
*
* Disclaimer:
* HDSC MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
* REGARDING THE SOFTWARE (INCLUDING ANY ACOOMPANYING WRITTEN MATERIALS),
* ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING,
* WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED
* WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED
* WARRANTY OF NONINFRINGEMENT.
* HDSC SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT,
* NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT
* LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION,
* LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR
* INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA,
* SAVINGS OR PROFITS,
* EVEN IF Disclaimer HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
* INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED
* FROM, THE SOFTWARE.
*
* This software may be replicated in part or whole for the licensed use,
* with the restriction that this Disclaimer and Copyright notice must be
* included with each copy of this software, whether used in part or whole,
* at all times.
*/
/******************************************************************************/
/** \file interrupts_hc32f030.c
 **
 ** Interrupt management
 ** @link Driver Group Some description @endlink
 **
 **   - 2018-04-15  1.0  Lux     First version.
 **
 **
 ******************************************************************************/

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "ddl.h"
#include "interrupts_hc32f030.h"
__WEAKDEF void Gpio_IRQHandler(uint8_t u8Param);
__WEAKDEF void Dma_IRQHandler(uint8_t u8Param);
__WEAKDEF void Uart_IRQHandler(uint8_t u8Param);
__WEAKDEF void Spi_IRQHandler(uint8_t u8Param);
__WEAKDEF void I2c_IRQHandler(uint8_t u8Param);
__WEAKDEF void Tim_IRQHandler(uint8_t u8Param);
__WEAKDEF void Tim3_IRQHandler(uint8_t u8Param);
__WEAKDEF void Adt_IRQHandler(uint8_t u8Param);
__WEAKDEF void Pca_IRQHandler(uint8_t u8Param);
__WEAKDEF void Wdt_IRQHandler(uint8_t u8Param);
__WEAKDEF void Vc_IRQHandler(uint8_t u8Param);
__WEAKDEF void Rtc_IRQHandler(uint8_t u8Param);
__WEAKDEF void Adc_IRQHandler(uint8_t u8Param);
__WEAKDEF void Lvd_IRQHandler(uint8_t u8Param);
__WEAKDEF void EfRam_IRQHandler(uint8_t u8Param);
__WEAKDEF void ClkTrim_IRQHandler(uint8_t u8Param);

/**
 *******************************************************************************
 ** \brief NVIC ????????????
 **
 ** \param [in]  enIrq          ?????????????????????
 ** \param [in]  enLevel        ???????????????????????????
 ** \param [in]  bEn            ????????????
 ** \retval    Ok       ????????????
 **            ?????????   ????????????
 ******************************************************************************/
void EnableNvic(IRQn_Type enIrq, en_irq_level_t enLevel, boolean_t bEn)
{
    NVIC_ClearPendingIRQ(enIrq);
    NVIC_SetPriority(enIrq, enLevel);
    if (TRUE == bEn)
    {
        NVIC_EnableIRQ(enIrq);
    }
    else
    {
        NVIC_DisableIRQ(enIrq);
    }
}

/**
 *******************************************************************************
 ** \brief NVIC hardware fault ????????????
 **        ????????????????????????
 **
 ** \retval
 ******************************************************************************/
void HardFault_Handler(void)
{
    volatile int a = 0;

    while( 0 == a)
    {
        ;
    }
}

/**
 *******************************************************************************
 ** \brief GPIO PortA ??????????????????
 ** 
 ** \retval
 ******************************************************************************/
void PORTA_IRQHandler(void)
{
    Gpio_IRQHandler(0);
}

/**
 *******************************************************************************
 ** \brief GPIO PortB ??????????????????
 ** 
 ** \retval
 ******************************************************************************/
void PORTB_IRQHandler(void)
{
    Gpio_IRQHandler(1);
}

/**
 *******************************************************************************
 ** \brief GPIO PortC ??????????????????
 ** 
 ** \retval
 ******************************************************************************/
void PORTC_IRQHandler(void)
{
    Gpio_IRQHandler(2);
}

/**
 *******************************************************************************
 ** \brief GPIO PortD ??????????????????
 ** 
 ** \retval
 ******************************************************************************/
void PORTD_IRQHandler(void)
{
    Gpio_IRQHandler(3);
}

/**
 *******************************************************************************
 ** \brief DMAC  ??????????????????
 ** 
 ** \retval
 ******************************************************************************/
void DMAC_IRQHandler(void)
{
    Dma_IRQHandler(0);
}


/**
 *******************************************************************************
 ** \brief UART0 ??????0 ??????????????????
 ** 
 ** \retval
 ******************************************************************************/
void UART0_IRQHandler(void)
{
    Uart_IRQHandler(0);
}

/**
 *******************************************************************************
 ** \brief UART1 ??????1 ??????????????????
 ** 
 ** \retval
 ******************************************************************************/
void UART1_IRQHandler(void)
{
    Uart_IRQHandler(1);
}


/**
 *******************************************************************************
 ** \brief SPI0 ??????????????????
 ** 
 ** \retval
 ******************************************************************************/
void SPI0_IRQHandler(void)
{
    Spi_IRQHandler(0);
}

/**
 *******************************************************************************
 ** \brief SPI1 ??????????????????
 ** 
 ** \retval
 ******************************************************************************/
void SPI1_IRQHandler(void)
{
    Spi_IRQHandler(1);
}

/**
 *******************************************************************************
 ** \brief I2C0 ??????????????????
 ** 
 ** \retval
 ******************************************************************************/
void I2C0_IRQHandler(void)
{
    I2c_IRQHandler(0);
}

/**
 *******************************************************************************
 ** \brief I2C1 ??????????????????
 ** 
 ** \retval
 ******************************************************************************/
void I2C1_IRQHandler(void)
{
    I2c_IRQHandler(1);
}

/**
 *******************************************************************************
 ** \brief TIM0 ????????????0 ??????????????????
 ** 
 ** \retval
 ******************************************************************************/
void TIM0_IRQHandler(void)
{
    Tim_IRQHandler(0);
}

/**
 *******************************************************************************
 ** \brief TIM1 ????????????1 ??????????????????
 ** 
 ** \retval
 ******************************************************************************/
void TIM1_IRQHandler(void)
{
    Tim_IRQHandler(1);
}

/**
 *******************************************************************************
 ** \brief TIM2 ????????????2 ??????????????????
 ** 
 ** \retval
 ******************************************************************************/
void TIM2_IRQHandler(void)
{
    Tim_IRQHandler(2);
}

/**
 *******************************************************************************
 ** \brief TIM3 ????????????3 ??????????????????
 ** 
 ** \retval
 ******************************************************************************/
void TIM3_IRQHandler(void)
{
    Tim3_IRQHandler(0);
}

/**
 *******************************************************************************
 ** \brief TIM4 ????????????4 ??????????????????
 ** 
 ** \retval
 ******************************************************************************/
void TIM4_IRQHandler(void)
{
    Adt_IRQHandler(4);
}

/**
 *******************************************************************************
 ** \brief TIM5 ????????????5 ??????????????????
 ** 
 ** \retval
 ******************************************************************************/
void TIM5_IRQHandler(void)
{
    Adt_IRQHandler(5);
}

/**
 *******************************************************************************
 ** \brief TIM6 ????????????6 ??????????????????
 ** 
 ** \retval
 ******************************************************************************/
void TIM6_IRQHandler(void)
{
    Adt_IRQHandler(6);
}

/**
 *******************************************************************************
 ** \brief PCA ??????????????????
 ** 
 ** \retval
 ******************************************************************************/
void PCA_IRQHandler(void)
{
    Pca_IRQHandler(0);
}

/**
 *******************************************************************************
 ** \brief WDT ??????????????????
 ** 
 ** \retval
 ******************************************************************************/
void WDT_IRQHandler(void)
{
    Wdt_IRQHandler(0);
}


/**
 *******************************************************************************
 ** \brief ADC ??????????????????
 ** 
 ** \retval
 ******************************************************************************/
void ADC_IRQHandler(void)
{
    Adc_IRQHandler(0);
}

/**
 *******************************************************************************
 ** \brief ????????????0 ??????????????????
 ** 
 ** \retval
 ******************************************************************************/
void VC0_IRQHandler(void)
{
    Vc_IRQHandler(0);
}

/**
 *******************************************************************************
 ** \brief ????????????1 ??????????????????
 ** 
 ** \retval
 ******************************************************************************/
void VC1_IRQHandler(void)
{
    Vc_IRQHandler(1);
}

/**
 *******************************************************************************
 ** \brief ??????????????? ??????????????????
 ** 
 ** \retval
 ******************************************************************************/
void LVD_IRQHandler(void)
{
    Lvd_IRQHandler(0);
}

/**
 *******************************************************************************
 ** \brief RAM ??????????????????
 ** 
 ** \retval
 ******************************************************************************/
void EF_RAM_IRQHandler(void)
{
    EfRam_IRQHandler(0);
}

/**
 *******************************************************************************
 ** \brief ???????????? ??????????????????
 ** 
 ** \retval
 ******************************************************************************/
void CLKTRIM_IRQHandler(void)
{
    ClkTrim_IRQHandler(0);
}



/******************************************************************************/
/* EOF (not truncated)                                                        */
/******************************************************************************/
