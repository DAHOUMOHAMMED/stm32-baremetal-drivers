/*
 * stm32f446xx_spi_driver.c
 *
 *  Created on: Nov 4, 2026
 *      Author: Med Dahou
 */


#include "stm32f446xx_spi_driver.h"

static void spi_txe_interrrupt_handle(SPI_Handle_t *pHandle);
static void spi_rxne_interrrupt_handle(SPI_Handle_t *pHandle);
static void spi_ovr_err_interrrupt_handle(SPI_Handle_t *pHandle);


/*********************************************************************
 * @fn              - SPI_PeriClockControl
 *
 * @brief           - This function enables or disables the peripheral clock for the given SPI
 *
 * @param[in]       - base address of the SPI peripheral (SPI1, SPI2, SPI3 or SPI4)
 * @param[in]       - ENABLE or DISABLE macros
 *
 * @return          - none
 *
 * @Note            - The clock must be enabled before writing to any register of the SPI
 *********************************************************************/
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
	if (EnorDi == ENABLE)
	{
		if (pSPIx == SPI1)
		{
			SPI1_PCLK_EN();
		}
		else if (pSPIx == SPI2)
		{
			SPI2_PCLK_EN();
		}
		else if (pSPIx == SPI3)
		{
			SPI3_PCLK_EN();
		}
		else if (pSPIx == SPI4)
		{
			SPI4_PCLK_EN();
		}
	}
	else
	{
		if (pSPIx == SPI1)
		{
			SPI1_PCLK_DI();
		}
		else if (pSPIx == SPI2)
		{
			SPI2_PCLK_DI();
		}
		else if (pSPIx == SPI3)
		{
			SPI3_PCLK_DI();
		}
		else if (pSPIx == SPI4)
		{
			SPI4_PCLK_DI();
		}
	}
}

/*
 * Init and DeInit
 */
void SPI_Init(SPI_Handle_t *pSPIHandle)
{
	uint32_t tempreg = 0;

	/* enable the peripheral clock */
	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);

	/* 1. configure the device mode (master/slave) */
	tempreg |= ((uint32_t)pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR);

	/* 2. configure the bus config */
	if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_Bus_Config_FD)
	{
		/* full duplex: BIDI mode cleared */
		tempreg &= ~(1U << SPI_CR1_BIDIMODE);
	}
	else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_Bus_Config_HD)
	{
		/* half duplex: BIDI mode set */
		tempreg |= (1U << SPI_CR1_BIDIMODE);
	}
	else if (pSPIHandle->SPIConfig.SPI_BusConfig == SPI_Bus_Config_SIMPLEX_RXONLY)
	{
		/* simplex receive only: BIDI cleared, RXONLY set */
		tempreg &= ~(1U << SPI_CR1_BIDIMODE);
		tempreg |= (1U << SPI_CR1_RXONLY);
	}

	/* 3. configure the serial clock speed (BR[2:0]) */
	tempreg |= ((uint32_t)pSPIHandle->SPIConfig.SPI_SclkSpeed << SPI_CR1_BR);

	/* 4. configure the data frame format */
	tempreg |= ((uint32_t)pSPIHandle->SPIConfig.SPI_DFF << SPI_CR1_DFF);

	/* 5. configure the clock polarity */
	tempreg |= ((uint32_t)pSPIHandle->SPIConfig.SPI_CPOL << SPI_CR1_CPOL);

	/* 6. configure the clock phase */
	tempreg |= ((uint32_t)pSPIHandle->SPIConfig.SPI_CPHA << SPI_CR1_CPHA);

	/* 7. configure the software slave management */
	tempreg |= ((uint32_t)pSPIHandle->SPIConfig.SPI_SSM << SPI_CR1_SSM);

	/* 8. in master mode with software NSS, SSI must be 1 (otherwise MODF error) */
		if (pSPIHandle->SPIConfig.SPI_SSM == SPI_SSM_EN &&
	    pSPIHandle->SPIConfig.SPI_DeviceMode == SPI_Device_Mode_Master)
	{
		tempreg |= (1U << SPI_CR1_SSI);
	}

	pSPIHandle->pSPIx->CR1 = tempreg;




}

/*********************************************************************
 * @fn              - SPI_DeInit
 *
 * @brief           - This function resets all the registers of the given SPI peripheral
 *
 * @param[in]       - base address of the SPI peripheral (SPI1, SPI2, SPI3 or SPI4)
 *
 * @return          - none
 *
 * @Note            - The reset is done through the RCC peripheral reset registers
 *********************************************************************/
void SPI_DeInit(SPI_RegDef_t *pSPIx)
{
	if (pSPIx == SPI1)
	{
		SPI1_REG_RESET();
	}
	else if (pSPIx == SPI2)
	{
		SPI2_REG_RESET();
	}
	else if (pSPIx == SPI3)
	{
		SPI3_REG_RESET();
	}
	else if (pSPIx == SPI4)
	{
		SPI4_REG_RESET();
	}
}



/*********************************************************************
 * @fn              - SPI_GETFLAGSTATUS
 *
 * @brief           - This function returns the state of a flag of the SPI status register (SR)
 *
 * @param[in]       - base address of the SPI peripheral
 * @param[in]       - flag mask to test (SPI_TXE_FLAG, SPI_RXNE_FLAG, SPI_BUSY_FLAG...)
 *
 * @return          - FLAG_SET if the flag is set, FLAG_RESET otherwise
 *
 * @Note            - The flag is a mask with a single bit set: the bitwise AND
 *                    with SR is non-zero only if that bit is 1.
 *********************************************************************/
uint8_t SPI_GETFLAGSTATUS(SPI_RegDef_t *pSPIx, uint32_t FlagName)
{
	return (pSPIx->SR & FlagName) ? FLAG_SET : FLAG_RESET;
}

/*********************************************************************
 * @fn              - SPI_SendData
 *
 * @brief           - This function sends data through the SPI peripheral
 *
 * @param[in]       - pSPIx: Base address of the SPI peripheral
 * @param[in]       - pTxBuffer: Pointer to the data buffer to be transmitted
 * @param[in]       - Len: Length of the data to be transmitted (in bytes)
 *
 * @return          - none
 *
 * @Note            - Supports both 8-bit and 16-bit data frame formats.
 *                    In 16-bit mode, Len must be an even number.
 *********************************************************************/
void SPI_SendData(SPI_RegDef_t *pSPIx , uint8_t *pTxBuffer , uint32_t Len)
{
	while (Len > 0)
	{
		//1 wait until TXE is set
		while(SPI_GETFLAGSTATUS(pSPIx , SPI_TXE_FLAG) == FLAG_RESET);

		//2 check the DFF bit in CR1

		if(pSPIx->CR1 & (1U << SPI_CR1_DFF))
		{
			// 16 bits DFF
			// 1. load the date in DR
			pSPIx->DR = *((uint16_t*)pTxBuffer);

			Len--;
			Len--;
			pTxBuffer+=2;
		}

		else
		{
			// 8 bits DFF
			// 1. load the date in DR
			pSPIx->DR = *(pTxBuffer);

			Len--;
			pTxBuffer++;
		}



	}
}

/*********************************************************************
 * @fn              - SPI_ReceiveData
 *
 * @brief           - This function receives data from the SPI peripheral
 *
 * @param[in]       - pSPIx: Base address of the SPI peripheral
 * @param[out]      - pRxBuffer: Pointer to the buffer where received data will be stored
 * @param[in]       - Len: Length of the data to be received (in bytes)
 *
 * @return          - none
 *
 * @Note            - Supports both 8-bit and 16-bit data frame formats.
 *                    In 16-bit mode, Len must be an even number.
 *********************************************************************/
void SPI_ReceiveData(SPI_RegDef_t *pSPIx , uint8_t *pRxBuffer , uint32_t Len)
{


	while (Len > 0)
	{
		//1 wait until RXNE is set
		while(SPI_GETFLAGSTATUS(pSPIx , SPI_RXNE_FLAG) == FLAG_RESET);

		//2 check the DFF bit in CR1

		if(pSPIx->CR1 & (1U << SPI_CR1_DFF))
		{
			// 16 bits DFF
			// 1. load the date from DR to RxBuffer
			 *((uint16_t*)pRxBuffer) = pSPIx->DR;

			Len--;
			Len--;
			pRxBuffer+=2;
		}

		else
		{
			// 8 bits DFF
			// 1. load the date from DR to RxBuffer
			 *(pRxBuffer) = pSPIx->DR;

			Len--;
			pRxBuffer++;
		}

	}
}

/*********************************************************************
 * @fn              - SPI_PeripheralControl
 *
 * @brief           - This function enables or disables the SPI peripheral (SPE bit of SPI_CR1)
 *
 * @param[in]       - base address of the SPI peripheral
 * @param[in]       - ENABLE or DISABLE macros
 *
 * @return          - none
 *
 * @Note            - Configure the SPI (SPI_Init) before enabling it. When
 *                    disabling, wait for the BSY flag to be cleared first,
 *                    otherwise the last byte being sent can be cut.
 *********************************************************************/
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
	if (EnorDi == ENABLE)
	{
		/* SPE: SPI enable */
		pSPIx->CR1 |= (1U << SPI_CR1_SPE);
	}
	else
	{
		/* SPE: SPI disable */
		pSPIx->CR1 &= ~(1U << SPI_CR1_SPE);
	}
}

/*********************************************************************
 * @fn              - SPI_SSOEConfig
 *
 * @brief           - This function enables or disables the SS output (SSOE bit of SPI_CR2)
 *
 * @param[in]       - base address of the SPI peripheral
 * @param[in]       - ENABLE or DISABLE macros
 *
 * @return          - none
 *
 * @Note            - SSOE only has an effect in master mode with hardware slave
 *                    management (SSM = 0). When enabled, the NSS pin is driven low
 *                    automatically as soon as the SPI is enabled (SPE = 1), and
 *                    released when SPE = 0. When disabled, NSS can be used
 *                    in a multi-master configuration.
 *********************************************************************/
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx, uint8_t EnorDi)
{
	if (EnorDi == ENABLE)
	{
		pSPIx->CR2 |= (1U << SPI_CR2_SSOE);
	}
	else
	{
		pSPIx->CR2 &= ~(1U << SPI_CR2_SSOE);
	}
}

void SPI_ReceiveData(SPI_RegDef_t *pSPIx , uint8_t *pTxBuffer , uint32_t Len);




/*
 * IRQ Configuration and Handling
 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi )
{
	uint32_t temp = IRQNumber%32;

	if(EnorDi == ENABLE)
	{
		if(IRQNumber <= 31)
		{
			(*NVIC_ISER0) = (1U << temp);
		}

		else if(IRQNumber > 31 && IRQNumber < 64)
		{
			(*NVIC_ISER1) = (1U << temp);
		}

		else if(IRQNumber >= 64 && IRQNumber < 96)
		{
			(*NVIC_ISER2) = (1U << temp);
		}
	}

	else
	{
		if(IRQNumber <= 31)
		{
			(*NVIC_ICER0) = (1U << temp);
		}

		else if(IRQNumber > 31 && IRQNumber < 64)
		{
			(*NVIC_ICER1) = (1U << temp);
		}

		else if(IRQNumber >= 64 && IRQNumber < 96)
		{
			(*NVIC_ICER2) = (1U << temp);
		}
	}
}
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority)
{
	uint8_t iprx = IRQNumber/4;
	uint8_t iprx_section = IRQNumber%4;

	uint8_t shift_amount = (8*iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);

	*(NVIC_PR_BASE_ADDR + iprx) &= ~(0xFU << shift_amount);
	*(NVIC_PR_BASE_ADDR + iprx) |= ((uint32_t)IRQPriority << shift_amount);
}




/*********************************************************************
 * @fn              - SPI_SendDataIT
 *
 * @brief           - This function sends data through the SPI peripheral
 *                    using interrupt mode (non-blocking).
 *
 * @param[in]       - pSPIHandle: Pointer to the SPI handle structure
 * @param[in]       - pTxBuffer: Pointer to the data buffer to be transmitted
 * @param[in]       - Len: Length of the data to be transmitted (in bytes)
 *
 * @return          - Returns the previous transmission state.
 *
 * @Note            - The function saves the Tx buffer address and length,
 *                    marks the SPI as busy in transmission, and enables
 *                    the TXE interrupt to start the transmission.
 *********************************************************************/
uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle , uint8_t *pTxBuffer , uint32_t Len)
{
	uint8_t  state =  pSPIHandle->TxState;

	if(state != SPI_BUSY_IN_TX)
	{
		//1. save the Tx buffer address  and Lin to some global variables
		pSPIHandle->pTxBuffer = pTxBuffer;
		pSPIHandle->TxLen     = Len;

		//2.Mark the spi state as busy in transmission
		pSPIHandle->TxState = SPI_BUSY_IN_TX;

		//3 Enable the TXEIE control bit to get interrupt whenever TXE flag is set in SR
		pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_TXEIE);
	}

	return state;

}




/*********************************************************************
 * @fn              - SPI_ReceiveDataIT
 *
 * @brief           - This function receives data from the SPI peripheral
 *                    using interrupt mode (non-blocking).
 *
 * @param[in]       - pSPIHandle: Pointer to the SPI handle structure
 * @param[out]      - pRxBuffer: Pointer to the buffer where received data
 *                    will be stored
 * @param[in]       - Len: Length of the data to be received (in bytes)
 *
 * @return          - Returns the previous reception state.
 *
 * @Note            - The function saves the Rx buffer address and length,
 *                    marks the SPI as busy in reception, and enables
 *                    the RXNE interrupt to handle incoming data.
 *********************************************************************/
uint8_t SPI_ReceiveDataIT(SPI_Handle_t *pSPIHandle , uint8_t *pRxBuffer , uint32_t Len)
{
	uint8_t  state =  pSPIHandle->RxState;

	if(state != SPI_BUSY_IN_RX)
	{
		//1. save the Rx buffer address  and Lin to some global variables
		pSPIHandle->pRxBuffer = pRxBuffer;
		pSPIHandle->RxLen     = Len;

		//2.Mark the spi state as busy in reception
		pSPIHandle->RxState = SPI_BUSY_IN_RX;

		//3 Enable the RXNEIE control bit to get interrupt whenever TXE flag is set in SR
		pSPIHandle->pSPIx->CR2 |= (1 << SPI_CR2_RXNEIE);
	}

	return state;
}



void SPI_IRQHandling(SPI_Handle_t *pSPIHandle)
{
	uint8_t temp1 , temp2;

	// first lets check for TXE
	temp1 = SPI_GETFLAGSTATUS(pSPIHandle->pSPIx,SPI_TXE_FLAG);
	temp2 = pSPIHandle->pSPIx->CR2 & (1 << SPI_CR2_TXEIE);

	if(temp1 & temp2)
	{
		//handle TXE

		spi_txe_interrrupt_handle(pSPIHandle);
	}

	// Second lets check for RXNE
	temp1 = SPI_GETFLAGSTATUS(pSPIHandle->pSPIx,SPI_RXNE_FLAG);
	temp2 = pSPIHandle->pSPIx->CR2 & (1 << SPI_CR2_RXNEIE);

	if(temp1 & temp2)
	{
		//handle RXE
		spi_rxne_interrrupt_handle(pSPIHandle);

	}

	// check for ovr flag
	temp1 = SPI_GETFLAGSTATUS(pSPIHandle->pSPIx,SPI_OVR_FLAG);
	temp2 = pSPIHandle->pSPIx->CR2 & (1 << SPI_CR2_ERRIE);

	if(temp1 & temp2)
	{
		//handle OVR_Err
		spi_ovr_err_interrrupt_handle(pSPIHandle);

	}

}

// some helper function implementation

static void spi_txe_interrrupt_handle(SPI_Handle_t *pSPIHandle)
{
	//1 check the DFF bit in CR1

	if(pSPIHandle->pSPIx->CR1 & ( 1U << SPI_CR1_DFF))
	{
		// 16 bits DFF
		// 1. load the date in DR
		pSPIHandle->pSPIx->DR = *((uint16_t*)pSPIHandle->pTxBuffer);

		pSPIHandle->TxLen--;
		pSPIHandle->TxLen--;
		pSPIHandle->pTxBuffer+=2;
	}

	else
	{
		// 8 bits DFF
		// 1. load the date in DR
		pSPIHandle->pSPIx->DR = *(pSPIHandle->pTxBuffer);

		pSPIHandle->TxLen--;
		pSPIHandle->pTxBuffer++;
	}

	if(! pSPIHandle->TxLen)
	{
		//TxLen is zero , so close the spi transmission  and inform the application
		// TX is over

		SPI_CloseTransmission(pSPIHandle);
		SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_TX_CMPLT);


	}

}


/*********************************************************************
 * @fn              - spi_rxne_interrupt_handle
 *
 * @brief           - This function handles SPI reception using the
 *                    RXNE interrupt.
 *
 * @param[in]       - pSPIHandle: Pointer to the SPI handle structure
 *
 * @return          - none
 *
 * @Note            - Reads received data from the SPI data register
 *                    and stores it in the Rx buffer.
 *                    Supports both 8-bit and 16-bit data frame formats.
 *                    When reception is complete, it disables the RXNE
 *                    interrupt and notifies the application.
 *********************************************************************/
static void spi_rxne_interrrupt_handle(SPI_Handle_t *pSPIHandle)
{


	if(pSPIHandle->pSPIx->CR1 & ( 1U << SPI_CR1_DFF))
	{
		// 16 bits DFF

		*((uint16_t*)pSPIHandle->pRxBuffer) = pSPIHandle->pSPIx->DR  ;

		pSPIHandle->RxLen--;
		pSPIHandle->RxLen--;
		pSPIHandle->pRxBuffer+=2;
	}

	else
	{
		// 8 bits DFF
		// 1. write data in DR
		*(pSPIHandle->pRxBuffer) = pSPIHandle->pSPIx->DR  ;

		pSPIHandle->RxLen--;
		pSPIHandle->pRxBuffer++;
	}

	if(! pSPIHandle->RxLen)
	{
		//RxLen is zero , so close the spi reception  and inform the application
		// RX is over

		SPI_CloseReception(pSPIHandle);
		SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_RX_CMPLT);


	}
}


static void spi_ovr_err_interrrupt_handle(SPI_Handle_t *pSPIHandle)
{
	uint8_t temp;

	//1clear the over flag
	if(pSPIHandle->TxState != SPI_BUSY_IN_TX)
	{
		temp = pSPIHandle->pSPIx->DR;
		temp = pSPIHandle->pSPIx->SR;
		(void)temp;
	}

	//2 inform the application
	SPI_ApplicationEventCallback(pSPIHandle, SPI_EVENT_OVR_CMPLT);

}


void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle)
{
	pSPIHandle->pSPIx->CR2 &= ~( 1U << SPI_CR2_TXEIE);
	pSPIHandle->pTxBuffer = NULL;
	pSPIHandle->TxLen = 0;
	pSPIHandle->TxState = SPI_READY;
}


void SPI_CloseReception(SPI_Handle_t *pSPIHandle)
{
	pSPIHandle->pSPIx->CR2 &= ~( 1U << SPI_CR2_RXNEIE);
	pSPIHandle->pRxBuffer = NULL;
	pSPIHandle->RxLen = 0;
	pSPIHandle->RxState = SPI_READY;
}

void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx)
{
	uint8_t temp ;
	temp = pSPIx->DR;
	temp = pSPIx->SR;

	(void)temp;
}

__we void SPI_ApplicationEventCallback(SPI_Handle_t *pSPIHandle, uint8_t appEv)
{

}
