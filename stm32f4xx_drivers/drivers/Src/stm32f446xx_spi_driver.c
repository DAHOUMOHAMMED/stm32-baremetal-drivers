/*
 * stm32f446xx_spi_driver.c
 *
 *  Created on: Nov 4, 2026
 *      Author: Med Dahou
 */


#include "stm32f446xx_spi_driver.h"


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



uint8_t SPI_GETFLAGSTATUS(SPI_RegDef_t *pSPIx , uint32_t  FlagName)
{
	return (pSPIx->SR & FlagName) ? FLAG_SET : FLAG_RESET ;
}

/*********************************************************************
 * @fn              - SPI_GetFlagStatus
 *
 * @brief           - This function returns the state of a flag of the SPI status register
 *
 * @param[in]       - base address of the SPI peripheral
 * @param[in]       - flag mask (SPI_TXE_FLAG, SPI_RXNE_FLAG, SPI_BUSY_FLAG...)
 *
 * @return          - FLAG_SET or FLAG_RESET
 *
 * @Note            - none
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


void SPI_PeripheralControl(SPI_RegDef_t *pSPIx , uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		//SPE: SPI enable
		pSPIx->CR1 |= (1 << SPI_CR1_SPE);
	}

	else
	{
		//SPE: SPI desable
		pSPIx->CR1 &= ~(1 << SPI_CR1_SPE);
	}
}

void SPI_ReceiveData(SPI_RegDef_t *pSPIx , uint8_t *pTxBuffer , uint32_t Len);


/*
 * IRQ Configuration and Handling
 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi );
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority);
void SPI_IRQHandling(SPI_Handle_t *pHandle);



