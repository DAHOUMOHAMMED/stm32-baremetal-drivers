/*
 * stm32f446xx_gpio_driver.c
 *
 *  Created on: Nov 1, 2026
 *      Author: Mohammed Dahou
 */


#include "stm32f446xx_gpio_driver.h"



/*
 * peripheral clock setup
 */

/*********************************************************************
 * @fn              - GPIOPeriClockControl
 *
 * @brief           - this function enable or disable peripheral clock for rhe give GPIO port
 *
 * @param[in]       - base address for the gpio peripheral
 * @param[in]       - enable and disable macros
 *
 * @return          - none
 *
 * @Note            - none
 *********************************************************************/
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnorDi)
{
	if (EnorDi == ENABLE)
	{
		if (pGPIOx == GPIOA)
		{
			GPIOA_PCLK_EN();
		}
		else if (pGPIOx == GPIOB)
		{
			GPIOB_PCLK_EN();
		}
		else if (pGPIOx == GPIOC)
		{
			GPIOC_PCLK_EN();
		}
		else if (pGPIOx == GPIOD)
		{
			GPIOD_PCLK_EN();
		}
		else if (pGPIOx == GPIOE)
		{
			GPIOE_PCLK_EN();
		}
		else if (pGPIOx == GPIOF)
		{
			GPIOF_PCLK_EN();
		}
		else if (pGPIOx == GPIOG)
		{
			GPIOG_PCLK_EN();
		}
		else if (pGPIOx == GPIOH)
		{
			GPIOH_PCLK_EN();
		}
	}
	else
	{
		if (pGPIOx == GPIOA)
		{
			GPIOA_PCLK_DI();
		}
		else if (pGPIOx == GPIOB)
		{
			GPIOB_PCLK_DI();
		}
		else if (pGPIOx == GPIOC)
		{
			GPIOC_PCLK_DI();
		}
		else if (pGPIOx == GPIOD)
		{
			GPIOD_PCLK_DI();
		}
		else if (pGPIOx == GPIOE)
		{
			GPIOE_PCLK_DI();
		}
		else if (pGPIOx == GPIOF)
		{
			GPIOF_PCLK_DI();
		}
		else if (pGPIOx == GPIOG)
		{
			GPIOG_PCLK_DI();
		}
		else if (pGPIOx == GPIOH)
		{
			GPIOH_PCLK_DI();
		}
	}
}


/*
 * Init and DeInit
 */

/*********************************************************************
 * @fn      - GPIO_Init
 *
 * @brief   - Initializes a GPIO pin according to the user configuration
 *
 * @param[in]  - pGPIOHandle : pointer to the handle containing the GPIO
 *                             base address (pGPIOx) and the pin configuration
 *                             (number, mode, speed, pull-up/down, output type,
 *                             alternate function)
 *
 * @return     - none
 *
 * @Note       - Configures MODER, OSPEEDR, PUPDR, OTYPER and AFR[0]/AFR[1]
 *             - The peripheral clock must be enabled before calling this
 *               function (see GPIO_PeriClockControl)
 *             - Interrupt mode (EXTI) is not implemented yet
 *********************************************************************/
void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
	uint32_t temp = 0;
	uint8_t  pin  = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber;

    // 1 configure the mode of gpio pin

	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
	{
		temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (2*pin));
		pGPIOHandle->pGPIOx->MODER &= ~(0x3U << (2 * pin));   /* efface les 2 bits */
		pGPIOHandle->pGPIOx->MODER |= temp;
		temp = 0;
	}


	else
	{
		if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT)
		{
			//1. configure the FTSR
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			//Clear the corresponding RTSR pin
			EXTI->RTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

		}

		else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT)
		{
			//2. configure the RTSR
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			//Clear the corresponding RTSR pin
			EXTI->FTSR &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}

		else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT)
		{
			//3. configure the RFT
			EXTI->FTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			EXTI->RTSR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}

		//2. configure the GPIO port selection in SYSCFG_EXTICR


		uint8_t temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 4;
		uint8_t temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 4;
		uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);

		SYSCFG_PCLK_EN();
		SYSCFG->EXTICR[temp1] = portcode << (temp2 *4);

		//3. enable the exti interrupt delivery using IMR
		EXTI->IMR |= (1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

	}

	// 2 configure the speed

	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << (2*pin) );
	pGPIOHandle->pGPIOx->OSPEEDER &= ~(0x3U << (2 * pin));   /* efface les 2 bits */
	pGPIOHandle->pGPIOx->OSPEEDER |= temp;

	temp = 0;


	// 3 configure the pupd setting

	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl << (2*pin));
	pGPIOHandle->pGPIOx->PUPDR &= ~(0x3U << (2 * pin));   /* efface les 2 bits */
	pGPIOHandle->pGPIOx->PUPDR |= temp;

	temp = 0;

	// 4 configure the outype

	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType << pin);
	pGPIOHandle->pGPIOx->OTYPER &= ~(0x1U << pin);   /* efface les 2 bits */
	pGPIOHandle->pGPIOx->OTYPER |= temp;

	temp = 0;

	// 5 configure the alt Functionality

	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN)
	{
		uint8_t temp1 ,temp2;

		temp1 = pin / 8 ;
		temp2 = pin % 8;

		pGPIOHandle->pGPIOx->AFR[temp1] &= ~(0xFU << (4*temp2));
		pGPIOHandle->pGPIOx->AFR[temp1] |= ((uint32_t) pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << (4*temp2));

	}

	temp = 0;

}


/*********************************************************************
 * @fn      - GPIO_DeInit
 *
 * @brief   - Resets all registers of a GPIO port to their reset values
 *
 * @param[in]  - pGPIOx : base address of the GPIO peripheral (GPIOA to GPIOH)
 *
 * @return     - none
 *
 * @Note       - Uses the RCC AHB1 peripheral reset register (AHB1RSTR):
 *               the port's reset bit is set to 1 then cleared to 0
 *             - Resets the whole port (all 16 pins), not a single pin
 *********************************************************************/
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{
	if (pGPIOx == GPIOA)
		{
			GPIOA_REG_RESET();
		}
		else if (pGPIOx == GPIOB)
		{
			GPIOB_REG_RESET();
		}
		else if (pGPIOx == GPIOC)
		{
			GPIOC_REG_RESET();
		}
		else if (pGPIOx == GPIOD)
		{
			GPIOD_REG_RESET();
		}
		else if (pGPIOx == GPIOE)
		{
			GPIOE_REG_RESET();
		}
		else if (pGPIOx == GPIOF)
		{
			GPIOF_REG_RESET();
		}
		else if (pGPIOx == GPIOG)
		{
			GPIOG_REG_RESET();
		}
		else if (pGPIOx == GPIOH)
		{
			GPIOH_REG_RESET();
		}
}

/*
 * Data Read and Write
 */

/*********************************************************************
 * @fn         - GPIO_ReadFromInputPin
 *
 * @brief      - Reads the logic level of a single GPIO input pin
 *
 * @param[in]  - pGPIOx     : base address of the GPIO peripheral (GPIOA, GPIOB, ...)
 * @param[in]  - PinNumber  : pin to read (0 to 15)
 *
 * @return     - 0 or 1 (logic level of the pin)
 *
 * @Note       - Reads the IDR register (input data register)
 *********************************************************************/
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx , uint8_t PinNumber)
{
	uint8_t pinValue = (uint8_t)(((pGPIOx->IDR)>> PinNumber) & 0x00000001);

	return pinValue;
}


/*********************************************************************
 * @fn      - GPIO_ReadFromInputPort
 *
 * @brief   - Reads the logic levels of all 16 pins of a GPIO port
 *
 * @param[in]  - pGPIOx : base address of the GPIO peripheral (GPIOA, GPIOB, ...)
 *
 * @return     - 16-bit value, bit n = logic level of pin n
 *
 * @Note       - Reads the IDR register (input data register)
 *********************************************************************/
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx )
{
	uint16_t portValue = (uint16_t)((pGPIOx->IDR) & 0x0000FFFF);

	return portValue;
}


/*********************************************************************
 * @fn         - GPIO_WriteToOutputPin
 *
 * @brief      - Sets or clears a single GPIO output pin
 *
 * @param[in]  - pGPIOx     : base address of the GPIO peripheral (GPIOA, GPIOB, ...)
 * @param[in]  - PinNumber  : pin to write (0 to 15)
 * @param[in]  - Value      : SET (1) or RESET (0)
 *
 * @return     - none
 *
 * @Note       - Read-modify-write on the ODR register, other pins are unchanged
 *********************************************************************/
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx , uint8_t PinNumber , uint8_t Value)
{
	if(Value == GPIO_PIN_SET)
	{
		pGPIOx->ODR |= (1U << PinNumber);
	}

	else
	{
		pGPIOx->ODR &= ~(1U << PinNumber);
	}
}

/*********************************************************************
 * @fn         - GPIO_WriteToOutputPort
 *
 * @brief      - Writes a 16-bit value to all pins of a GPIO port
 *
 * @param[in]  - pGPIOx : base address of the GPIO peripheral (GPIOA, GPIOB, ...)
 * @param[in]  - Value  : 16-bit value, bit n = level of pin n
 *
 * @return     - none
 *
 * @Note       - Overwrites the whole ODR register (all 16 pins)
 *********************************************************************/
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value)
{

	pGPIOx->ODR = Value;

}


/*********************************************************************
 * @fn         - GPIO_ToggleOutputPin
 *
 * @brief      - Toggles the logic level of a single GPIO output pin
 *
 * @param[in]  - pGPIOx     : base address of the GPIO peripheral (GPIOA, GPIOB, ...)
 * @param[in]  - PinNumber  : pin to toggle (0 to 15)
 *
 * @return     - none
 *
 * @Note       - Read-modify-write on the ODR register, other pins are unchanged
 *********************************************************************/
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx , uint8_t PinNumber)
{
	pGPIOx->ODR ^= (1 << PinNumber);
}

/*
 * IRQ Configuration and Handling
 */
void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi )
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



/*********************************************************************
 * @fn              - GPIO_IRQPriorityConfig
 *
 * @brief           - This function sets the priority of an IRQ line in the NVIC
 *
 * @param[in]       - IRQ number (position in the vector table, e.g. IRQ_NO_EXTI15_10)
 * @param[in]       - IRQ priority, from 0 (highest) to 15 (lowest)
 *
 * @return          - none
 *
 * @Note            - Each IPR register (32 bits) holds the priority of 4 IRQ lines
 *                    (8 bits each). On the STM32F4, only the upper 4 bits of each
 *                    8-bit field are implemented, so the priority is shifted
 *                    by (8 - NO_PR_BITS_IMPLEMENTED) bits before being written.
 *********************************************************************/
void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority)
{
	uint8_t iprx = IRQNumber/4;
	uint8_t iprx_section = IRQNumber%4;

	uint8_t shift_amount = (8*iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);

	*(NVIC_PR_BASE_ADDR + iprx) &= ~(0xFU << shift_amount);
	*(NVIC_PR_BASE_ADDR + iprx) |= ((uint32_t)IRQPriority << shift_amount);

}


/*********************************************************************
 * @fn              - GPIO_IRQHandling
 *
 * @brief           - This function clears the EXTI pending flag of the given pin
 *
 * @param[in]       - pin number (0 to 15) that triggered the interrupt
 *
 * @return          - none
 *
 * @Note            - Must be called from the IRQ handler (e.g. EXTI15_10_IRQHandler).
 *                    The pending flag is cleared by writing 1 to it: use "=" and not "|=",
 *                    otherwise the other pending flags, which are read as 1, would be
 *                    cleared too.
 *********************************************************************/
void GPIO_IRQHandling(uint8_t PinNumber)
{
	/* if the pending flag of this EXTI line is set */
	if (EXTI->PR & (1U << PinNumber))
	{
		/* clear it by writing 1 (writing 0 has no effect) */
		EXTI->PR = (1U << PinNumber);
	}
}
