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
/*
 * PB12 => SPI2_NSS
 * PB13 => SPI2_SCLK
 * PB14 => SPI2_MISO
 * PB15 => SPI2_MOSI
 * ALT function mode : 5
 */

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
	//SPIPINS.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	//GPIO_Init(&SPIPINS);



}
void SPI2_Inits()
{
	SPI_Handle_t SPI2Handle;

	SPI2Handle.pSPIx = SPI2;
	SPI2Handle.SPIConfig.SPI_BusConfig = SPI_Bus_Config_FD;
	SPI2Handle.SPIConfig.SPI_DeviceMode = SPI_Device_Mode_Master;
	SPI2Handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV2;
	SPI2Handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2Handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2Handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2Handle.SPIConfig.SPI_SSM = SPI_SSM_EN;


	SPI_Init(&SPI2Handle);



}
int main()
{

	char user_dat[] = "Hello World";

	SPI2_GPIOInits();

	SPI2_Inits();

	SPI_PeripheralControl(SPI2 , ENABLE);

	SPI_SendData(SPI2, (uint8_t*)user_dat, strlen(user_dat));



	while(1)
	{

	}


}


