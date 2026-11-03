/*
 * 004_interrupt_button.c
 *
 *  Created on: Nov 3, 2026
 *      Author: Med Dahou
 */





#include "stm32f446xx.h"
#include "stm32f446xx_gpio_driver.h"

#define LOW 0


void delay(void);
void EXTI2_IRQHandler(void);

int main(void)
{
	//uint8_t pinvalue = 0;

	GPIO_Handle_t GpioLedPA8 = {0};
	GPIO_Handle_t GpioButPD2 = {0};


	GpioLedPA8.pGPIOx = GPIOA;
	GpioLedPA8.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_OUT;
	GpioLedPA8.GPIO_PinConfig.GPIO_PinNumber      = GPIO_PIN_NO_8;
	GpioLedPA8.GPIO_PinConfig.GPIO_PinOPType 	   = GPIO_OP_TYPE_PP;
	GpioLedPA8.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GpioLedPA8.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_SPEED_LOW;

	GpioButPD2.pGPIOx = GPIOD;
	GpioButPD2.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_IT_FT;
	GpioButPD2.GPIO_PinConfig.GPIO_PinNumber      = GPIO_PIN_NO_2;
	GpioButPD2.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	GpioButPD2.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_SPEED_LOW;

	GPIOA_PCLK_EN();
	GPIOD_PCLK_EN();


	GPIO_Init(&GpioLedPA8);
	GPIO_Init(&GpioButPD2);

	GPIO_IRQInterruptConfig(IRQ_NO_EXTI2, ENABLE );
	//GPIO_IRQPriorityConfig(IRQ_NO_EXTI2, NVIC_IRQ_PRIO_3);


	while(1)
	{



	}
    return 0;
}

void delay(void)
{
	for (volatile uint32_t i = 0; i < 500000; i++);
}

void EXTI2_IRQHandler(void)
{
	GPIO_IRQHandling(2);

	delay();

	GPIO_ToggleOutputPin(GPIOA , 8);
	//delay();
}








