/*
 * 005_spi_tx_testing.c
 *
 *  Created on: Nov 5, 2026
 *      Author: Med Dahou
 */

#include <string.h>
#include "stm32f446xx.h"
#include "stm32f446xx_spi_driver.h"
#include "stm32f446xx_gpio_driver.h"

#define LOW 0
#define BTN_PRESSED LOW
/*
 * PB12 => SPI2_NSS
 * PB13 => SPI2_SCLK
 * PB14 => SPI2_MISO
 * PB15 => SPI2_MOSI
 * ALT function mode : 5
 */

void delay(void)
{
	for (volatile uint32_t i = 0; i < 500000; i++);
}

void SPI2_GPIOInits()
{
	GPIO_Handle_t SPIPINS = {0};

	SPIPINS.pGPIOx = GPIOB;
	SPIPINS.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPINS.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIPINS.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPINS.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPINS.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	//SCLK
	SPIPINS.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&SPIPINS);

	//MOSI
	SPIPINS.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPINS);

	//MISO
	//SPIPINS.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	//GPIO_Init(&SPIPINS);

	//NSS
	SPIPINS.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_Init(&SPIPINS);



}
void SPI2_Inits()
{
	SPI_Handle_t SPI2Handle;

	SPI2Handle.pSPIx = SPI2;
	SPI2Handle.SPIConfig.SPI_BusConfig = SPI_Bus_Config_FD;
	SPI2Handle.SPIConfig.SPI_DeviceMode = SPI_Device_Mode_Master;
	SPI2Handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV8;
	SPI2Handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2Handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2Handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2Handle.SPIConfig.SPI_SSM = SPI_SSM_DI;


	SPI_Init(&SPI2Handle);



}
int main()
{

	char user_dat[] = "Hello World";

	uint8_t pinvalue = 0;

	/*******************Button configuration*************************/
	GPIO_Handle_t GpioBut1 = {0};


	GpioBut1.pGPIOx = GPIOC;
	GpioBut1.GPIO_PinConfig.GPIO_PinMode        = GPIO_MODE_IN;
	GpioBut1.GPIO_PinConfig.GPIO_PinNumber      = GPIO_PIN_NO_13;
	GpioBut1.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GpioBut1.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_SPEED_LOW;

	GPIOC_PCLK_EN();
	GPIO_Init(&GpioBut1);


	/*******************SPI2 configuration*************************/

	/* message to send: 11 characters, the '\0' terminator is not sent */


	/* 1. configure the GPIO pins used by SPI2 (SCK, MOSI...) in alternate function mode */
	SPI2_GPIOInits();

	/* 2. configure the SPI2 peripheral (mode, speed, CPOL/CPHA, DFF, SSM) */
	SPI2_Inits();

	/* enable the SS output (SSOE = 1): in master mode with hardware NSS management
	 * (SSM = 0), the NSS pin is driven low automatically when SPE = 1 and released
	 * when SPE = 0, so the slave is selected without any GPIO code */
	SPI_SSOEConfig(SPI2, ENABLE);





	while(1)
	{
		pinvalue = GPIO_ReadFromInputPin(GPIOC , GPIO_PIN_NO_13);

		if(pinvalue == BTN_PRESSED)
		{
			delay();

			/* enable the SPI2 peripheral (SPE = 1) */
			SPI_PeripheralControl(SPI2, ENABLE);

			/* first send length information*/
			uint8_t dataLen = strlen(user_dat);
			SPI_SendData(SPI2,&dataLen , 1);

			/* send the data: pointer to the buffer and number of bytes */
			SPI_SendData(SPI2, (uint8_t*)user_dat, strlen(user_dat));

			while(SPI_GETFLAGSTATUS(SPI2 , SPI_BUSY_FLAG));

			/* enable the SPI2 peripheral (SPE = 1) */
			SPI_PeripheralControl(SPI2, DISABLE);

		}
	}


}


