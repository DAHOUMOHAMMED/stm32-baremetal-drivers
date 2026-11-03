/*
 * 003_led_toggle_exteral_but_and_led.c
 *
 *  Created on: Nov 2, 2026
 *      Author: Med Dahou
 */



#include "stm32f446xx.h"
#include "stm32f446xx_gpio_driver.h"

#define LOW 0
#define BTN_PRESSED LOW

void delay(void);

int main(void)
{
	uint8_t pinvalue = 0;

	GPIO_Handle_t GpioLedPA8 = {0};
	GPIO_Handle_t GpioButPB12 = {0};


	GpioLedPA8.pGPIOx = GPIOA;
	GpioLedPA8.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_OUT;
	GpioLedPA8.GPIO_PinConfig.GPIO_PinNumber      = GPIO_PIN_NO_8;
	GpioLedPA8.GPIO_PinConfig.GPIO_PinOPType 	   = GPIO_OP_TYPE_PP;
	GpioLedPA8.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GpioLedPA8.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_SPEED_LOW;

	GpioButPB12.pGPIOx = GPIOB;
	GpioButPB12.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_IN;
	GpioButPB12.GPIO_PinConfig.GPIO_PinNumber      = GPIO_PIN_NO_12;
	GpioButPB12.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	GpioButPB12.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_SPEED_LOW;

	GPIOA_PCLK_EN();
	GPIOB_PCLK_EN();


	GPIO_Init(&GpioLedPA8);
	GPIO_Init(&GpioButPB12);


	while(1)
	{
		pinvalue = GPIO_ReadFromInputPin(GPIOB , GPIO_PIN_NO_12);

		if(pinvalue == BTN_PRESSED)
		{
			delay();
			GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_8);
		}


	}
    return 0;
}

void delay(void)
{
	for (volatile uint32_t i = 0; i < 500000; i++);
}






