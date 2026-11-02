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
	if (EnorDi == ENABALE)
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
		//interupt mode
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
	pGPIOHandle->pGPIOx->OTYPER &= ~(0x3U << pin);   /* efface les 2 bits */
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
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);

/*
 * Data Read and Write
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx , uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx );
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx , uint8_t PinNumber , uint8_t Value);
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value);
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx , uint8_t PinNumber);

/*
 * IRQ Configuration and Handling
 */
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnorDi );
void GPIO_IRQHandling(uint8_t PinNumber);
