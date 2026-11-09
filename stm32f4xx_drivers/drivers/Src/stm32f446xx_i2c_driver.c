/*
 * stm32f446xx_i2c_driver.c
 *
 *  Created on: Nov 8, 2026
 *      Author: Med Dahou
 */

#include "stm32f446xx_i2c_driver.h"

static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx);
static void I2C_ExecuteAddresPhase(I2C_Handle_t *pI2CHandle, uint8_t SlaveAdrr);
static void I2C_ClearADDRFlag(I2C_RegDef_t *pI2Cx);
static void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx);


/*
 * @fn              - I2C_GenerateStartCondition
 *
 * @brief           - Generates a START condition on the I2C bus
 *
 * @param[in]       - pI2Cx: Pointer to the I2C peripheral register structure
 *
 * @return          - None
 *
 * @note            - Sets the START bit in the CR1 register to initiate
 *                    an I2C communication.
 */
static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx)
{
	pI2Cx->CR1 |= ( 1 << I2C_CR1_START);
}


/*
 * @fn              - I2C_GenerateStopCondition
 *
 * @brief           - Generates a STOP condition on the I2C bus
 *
 * @param[in]       - pI2Cx: Pointer to the I2C peripheral register structure
 *
 * @return          - None
 *
 * @note            - Sets the STOP bit in the CR1 register to terminate
 *                    the I2C communication.
 */
static void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx)
{
	pI2Cx->CR1 |= ( 1 << I2C_CR1_STOP);
}

/*
 * @fn              - I2C_ExecuteAddressPhase
 *
 * @brief           - Executes the address phase of the I2C communication
 *
 * @param[in]       - pI2CHandle: Pointer to the I2C handle structure
 * @param[in]       - SlaveAddr: 7-bit slave address
 *
 * @return          - None
 *
 * @note            - Configures the R/W bit to 0 (Write operation)
 *                    and sends the slave address through the Data Register (DR).
 */
static void I2C_ExecuteAddresPhase(I2C_Handle_t *pI2CHandle, uint8_t SlaveAdrr)
{
	SlaveAdrr = SlaveAdrr << 1;
	SlaveAdrr &= ~(1);
	pI2CHandle->pI2Cx->DR = SlaveAdrr;
}


/*
 * @fn              - I2C_ClearADDRFlag
 *
 * @brief           - Clears the ADDR flag in the I2C_SR1 register
 *
 * @param[in]       - pI2Cx: Pointer to the I2C peripheral register structure
 *
 * @return          - None
 *
 * @note            - The ADDR flag is cleared by reading SR1 followed by SR2.
 */
static void I2C_ClearADDRFlag(I2C_RegDef_t *pI2Cx)
{
	uint32_t dummyRead = pI2Cx->SR1;
	dummyRead = pI2Cx->SR2;

	(void) dummyRead;

}

uint16_t AHB_PreScaler[9] = { 2,4,8,16,32,64,128,256,512};
uint16_t APB1_PreScaler[4] = { 2,4,8,16};



/*********************************************************************
 * @fn              - I2C_PeriClockControl
 *
 * @brief           - This function enables or disables the peripheral clock for the given I2C
 *
 * @param[in]       - base address of the I2C peripheral (I2C1, I2C2 or I2C3 )
 * @param[in]       - ENABLE or DISABLE macros
 *
 * @return          - none
 *
 * @Note            - The clock must be enabled before writing to any register of the I2C
 *********************************************************************/
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx , uint8_t EnorDi)
{
	if (EnorDi == ENABLE)
	{
		if (pI2Cx == I2C1)
		{
			I2C1_PCLK_EN();
		}
		else if (pI2Cx == I2C2)
		{
			I2C2_PCLK_EN();
		}
		else if (pI2Cx == I2C3)
		{
			I2C3_PCLK_EN();
		}

	}
	else
	{
		if (pI2Cx == I2C1)
		{
			I2C1_PCLK_DI();
		}
		else if (pI2Cx == I2C2)
		{
			I2C2_PCLK_DI();
		}
		else if (pI2Cx == I2C3)
		{
			I2C3_PCLK_DI();
		}

	}
}


uint32_t RCC_GetPLLOutputClock()
{
	return 0;
}



/*********************************************************************
 * @fn              - RCC_GetPCLK1Value
 *
 * @brief           - This function calculates and returns the PCLK1
 *                    clock frequency.
 *
 * @param[in]       - none
 *
 * @return          - PCLK1 clock frequency in Hz.
 *
 * @Note            - Determines the system clock source (HSI, HSE or PLL),
 *                    then calculates the AHB and APB1 prescalers
 *                    to obtain the final PCLK1 frequency.
 *********************************************************************/
uint32_t RCC_GetPCLK1Value(void)
{
	uint32_t pclk1, SystemClk;

	uint8_t clkSRC , temp , ahbp , apb1p;

	clkSRC = ((RCC->CFGR >> 2) & 0x03);

	if(clkSRC == 0)
	{
		SystemClk = 16000000;
	}

	else if (clkSRC == 1)
	{
		SystemClk = 8000000;
	}

	else if (clkSRC == 2)
	{
		SystemClk = RCC_GetPLLOutputClock();
	}


	// for ahb
	temp = ((RCC->CFGR >> 4) & 0x0F);

	if(temp < 8)
	{
		ahbp = 1;
	}

	else
	{
		ahbp = AHB_PreScaler[temp - 8];
	}

	// for apb1
	temp = ((RCC->CFGR >> 10) & 0x07);

	if(temp < 4)
	{
		apb1p = 1;
	}

	else
	{
		apb1p = APB1_PreScaler[temp - 4];
	}

	pclk1 = (SystemClk / (ahbp * apb1p));
	return pclk1;
}


void I2C_Init(I2C_Handle_t *pI2CHandle)
{
	uint32_t tempreg = 0;

	// enable the peripheral clock
	I2C_PeriClockControl(pI2CHandle->pI2Cx, ENABLE);

	// ack control bit
	tempreg |= pI2CHandle->I2CConfig.I2C_ACKControl << I2C_CR1_ACK ;
	pI2CHandle->pI2Cx->CR1 = tempreg;

	//configure the Freq field of CR2
	tempreg = 0;
	tempreg |= RCC_GetPCLK1Value() / 1000000U;
	pI2CHandle->pI2Cx->CR2 = (tempreg & (0x3f));

	//program the device own address
	tempreg = 0;
	tempreg |= pI2CHandle->I2CConfig.I2C_DeviceAddress << 1;
	tempreg |= (1 << 14);
	pI2CHandle->pI2Cx->OAR1 = tempreg;

	// configure the CCR register
	uint16_t ccr_value = 0;
	tempreg = 0;

	if(pI2CHandle->I2CConfig.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
	{
		// standard mode
		ccr_value = RCC_GetPCLK1Value() / (2 * pI2CHandle->I2CConfig.I2C_SCLSpeed);
		tempreg |= ccr_value & 0xFFF;

	}

	else
	{
		//fast mode
		tempreg |= (1 << 15);
		tempreg |= (pI2CHandle->I2CConfig.I2C_FMDutyCycle << 14);

		if(pI2CHandle->I2CConfig.I2C_FMDutyCycle == I2C_FM_DUTY_2)
		{
			ccr_value = RCC_GetPCLK1Value() / (3 * pI2CHandle->I2CConfig.I2C_SCLSpeed);
		}

		else
		{
			ccr_value = RCC_GetPCLK1Value() / (25 * pI2CHandle->I2CConfig.I2C_SCLSpeed);
		}
		tempreg |= ccr_value & 0xFFF;
	}
	pI2CHandle->pI2Cx->CCR = tempreg;

}



/*********************************************************************
 * @fn              - I2C_DeInit
 *
 * @brief           - This function resets all the registers of the given I2C peripheral
 *
 * @param[in]       - base address of the I2C peripheral (I2C1, I2C2 or I2C3)
 *
 * @return          - none
 *
 * @Note            - The reset is done through the RCC peripheral reset registers
 *********************************************************************/
void I2C_DeInit(I2C_RegDef_t *pI2Cx)
{
	if (pI2Cx == I2C1)
	{
		I2C1_REG_RESET();
	}
	else if (pI2Cx == I2C2)
	{
		I2C2_REG_RESET();
	}
	else if (pI2Cx == I2C3)
	{
		I2C3_REG_RESET();
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
uint8_t I2C_GETFLAGSTATUS(I2C_RegDef_t *pI2Cx, uint32_t FlagName)
{
	return (pI2Cx->SR1 & FlagName) ? FLAG_SET : FLAG_RESET;
}



void I2C_MasterSendData(I2C_Handle_t *pI2CHandle , uint8_t *pTxBuffer , uint32_t Len , uint8_t SlaveAdrr)
{
	//1. Generate the START condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	//2. confirm that start is start generation is completed by checking the SB flag in the SR1
	while(!I2C_GETFLAGSTATUS(pI2CHandle->pI2Cx, I2C_FLAG_SB));

	//3. Send the address of the slave with r/nw bit set to w(0) (total 8 bits)
	I2C_ExecuteAddresPhase(pI2CHandle,  SlaveAdrr);

	//4. confirm that addres phase is completed by checking the ADDR flag in the teh SR1
	while(!I2C_GETFLAGSTATUS(pI2CHandle->pI2Cx, I2C_FLAG_ADDR));

	//5. clear the ADDR flag according to its software sequence
	I2C_ClearADDRFlag(pI2CHandle->pI2Cx);

	//6. send the data until len becomes 0
	while(Len > 0)
	{
		while(!I2C_GETFLAGSTATUS(pI2CHandle->pI2Cx, I2C_FLAG_TXE));

		pI2CHandle->pI2Cx->DR = *pTxBuffer;
		pTxBuffer ++;
		Len --;
	}

	//7. when Len becomes 0 wait for TXE=1 et BTF=1 before generating the stop condition
	while(!I2C_GETFLAGSTATUS(pI2CHandle->pI2Cx, I2C_FLAG_TXE));
	while(!I2C_GETFLAGSTATUS(pI2CHandle->pI2Cx, I2C_FLAG_BTF));

	//8. Generate the stop condition
	I2C_GenerateStopCondition(pI2CHandle->pI2Cx);


}





