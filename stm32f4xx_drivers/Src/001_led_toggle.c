/*
 * 001_led_toggle.c
 *
 *  Created on: Nov 2, 2026
 *      Author: Med Dahou
 */

#include "stm32f446xx.h"
//#include "stm32f446xx_gpio_driver.h"

void delay(void);

int main(void)
{
	GPIO_Handle_t GpioLed = {0};

	GpioLed.pGPIOx = GPIOA;
	GpioLed.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber      = GPIO_PIN_NO_5;
	GpioLed.GPIO_PinConfig.GPIO_PinOPType 	   = GPIO_OP_TYPE_PP;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_SPEED_LOW;

	GPIOA_PCLK_EN();
	GPIO_Init(&GpioLed);


	while(1)
	{
		GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_5);
		delay();
	}
    return 0;
}

void delay(void)
{
	for (volatile uint32_t i = 0; i < 500000; i++);
}

