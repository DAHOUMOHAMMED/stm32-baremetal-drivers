/*
 * stm32f446xx_i2c_driver.c
 *
 *  Created on: Nov 8, 2026
 *      Author: Med Dahou
 */

#include "stm32f446xx_i2c_driver.h"






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




uint32_t RCC_GetPCLK1Value(void)
{
	uint32_t pclk1;

	uint8_t clkSRC ;

	return pclk1;
}


void I2C_Init(I2C_Handle_t *pI2CHandle)
{
	uint32 tempreg = 0;

	// enable the peripheral clock
	I2C_PeriClockControl(pI2CHandle->pI2Cx, ENABLE);


	tempreg = pI2CHandle->I2CConfig.I2C_ACKControl << I2C_CR1_ACK ;



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
