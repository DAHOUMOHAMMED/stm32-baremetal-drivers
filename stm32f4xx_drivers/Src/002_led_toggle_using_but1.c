/*
 * 002_led_toggle_using_but1.c
 *
 *  Created on: Nov 2, 2026
 *      Author: Med Dahou
 */


#include "stm32f446xx.h"

#define LOW 0
#define BTN_PRESSED LOW

void delay(void);

int main(void)
{
	uint8_t pinvalue = 0;

	GPIO_Handle_t GpioLed = {0};
	GPIO_Handle_t GpioBut1 = {0};


	GpioLed.pGPIOx = GPIOA;
	GpioLed.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinNumber      = GPIO_PIN_NO_5;
	GpioLed.GPIO_PinConfig.GPIO_PinOPType 	   = GPIO_OP_TYPE_PP;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_SPEED_LOW;

	GpioBut1.pGPIOx = GPIOC;
	GpioBut1.GPIO_PinConfig.GPIO_PinMode       = GPIO_MODE_IN;
	GpioBut1.GPIO_PinConfig.GPIO_PinNumber     = GPIO_PIN_NO_13;
	GpioBut1.GPIO_PinConfig.GPIO_PinPuPdControl= GPIO_NO_PUPD;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_SPEED_LOW;

	GPIOA_PCLK_EN();
	GPIOC_PCLK_EN();


	GPIO_Init(&GpioLed);
	GPIO_Init(&GpioBut1);


	while(1)
	{
		pinvalue = GPIO_ReadFromInputPin(GPIOC , GPIO_PIN_NO_13);

		if(pinvalue == BTN_PRESSED)
		{
			delay();
			GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_5);
		}


	}
    return 0;
}

void delay(void)
{
	for (volatile uint32_t i = 0; i < 500000; i++);
}




