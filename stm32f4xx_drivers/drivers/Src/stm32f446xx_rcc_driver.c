/*
 * stm32f446xx_rcc_driver.c
 *
 *  Created on: Nov 18, 2026
 *      Author: Med Dahou
 */


#include "stm32f446xx_rcc_driver.h"


uint16_t AHB_PreScaler[9] = { 2,4,8,16,32,64,128,256,512};
uint16_t APB1_PreScaler[4] = { 2,4,8,16};
uint8_t  APB2_PreScaler[4] = {2, 4, 8, 16};


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

	uint8_t clkSRC , temp  , apb1p;
	uint16_t  ahbp;
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


/*********************************************************************
 * @fn              - RCC_GetPCLK2Value
 *
 * @brief           - This function calculates and returns the PCLK2
 *                    clock frequency.
 *
 * @param[in]       - none
 *
 * @return          - PCLK2 clock frequency in Hz.
 *
 * @Note            - Determines the system clock source (HSI, HSE or PLL),
 *                    then calculates the AHB and APB2 prescalers
 *                    to obtain the final PCLK2 frequency.
 *********************************************************************/
uint32_t RCC_GetPCLK2Value(void)
{
	uint32_t pclk2, SystemClk = 0;

	uint8_t  clkSRC, temp, apb2p;
	uint16_t ahbp;

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

	// for apb2
	temp = ((RCC->CFGR >> 13) & 0x07);

	if(temp < 4)
	{
		apb2p = 1;
	}

	else
	{
		apb2p = APB2_PreScaler[temp - 4];
	}

	pclk2 = (SystemClk / (ahbp * apb2p));
	return pclk2;
}
