/*
 * 007spi_cmd_handling.c
 *
 *  Created on: Nov 6, 2026
 *      Author: Med Dahou
 */



#include <string.h>
#include "stm32f446xx.h"
#include "stm32f446xx_spi_driver.h"
#include "stm32f446xx_gpio_driver.h"

//#define LOW 0
///#define BTN_PRESSED LOW

//command codes
#define COMMAND_LED_CTRL                         0X50
#define COMMAND_SENSOR_READ                      0X51
#define COMMAND_LED_READ                         0X52
#define COMMAND_PRINT                            0X53
#define COMMAND_ID_READ                          0X54

#define LED_OFF                0
#define LED_ON                 1

//ARDUINO ANALOG PINS
#define ANALOG_PIN0            0
#define ANALOG_PIN1            1
#define ANALOG_PIN2            2
#define ANALOG_PIN3            3
#define ANALOG_PIN4            4

//ARDUINO LED PIN
#define LED_PIN      9


/*
 * PB12 => SPI2_NSS
 * PB13 => SPI2_SCLK
 * PB14 => SPI2_MISO
 * PB15 => SPI2_MOSI
 * ALT function mode : 5
 */

uint8_t SPI_VerifyResponse(uint8_t ackbyte)
{
	if( ackbyte == 0xF5)
	{
		return 1;
	}

	return 0;
}

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
	SPIPINS.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	GPIO_Init(&SPIPINS);

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

	uint8_t dummy_write = 0xff;
	uint8_t dummy_read = 0;



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
		while(!GPIO_ReadFromInputPin(GPIOC , GPIO_PIN_NO_13));

		delay();

		/* enable the SPI2 peripheral (SPE = 1) */
		SPI_PeripheralControl(SPI2, ENABLE);

		//1 COMMAND_LED_CTRL  <pin no(1)>      <value(1)>

		uint8_t commandcode = COMMAND_LED_CTRL;
		uint8_t ackbyte = 0 ;
		uint8_t args[2];

		//send command
		SPI_SendData(SPI2,&commandcode , 1);

		//di dummy read to clear off the RXNE
		SPI_ReceiveData(SPI2, &dummy_read, 1);

		//send some dummy bits (1byte) to fetch the response from the slave
		SPI_SendData(SPI2,&dummy_write , 1);


		// read the ack byte received
		SPI_ReceiveData(SPI2, &ackbyte, 1);

		if(SPI_VerifyResponse(ackbyte))
		{
			// send agruments
			args[0] = LED_PIN;
			args[1] = LED_ON;

			// send led pin
			SPI_SendData(SPI2,args , 2);
		}
		// end of COMMAND_LED_CTRL


		//2 COMMAND_SENSOR_READ <analog pin number(1)>
		while(!GPIO_ReadFromInputPin(GPIOC , GPIO_PIN_NO_13));

		delay();




		commandcode = COMMAND_SENSOR_READ;
		//send command
		SPI_SendData(SPI2,&commandcode , 1);

		//do dummy read to clear off the RXNE
		SPI_ReceiveData(SPI2, &dummy_read, 1);

		//send some dummy bits (1byte) to fetch the response from the slave
		SPI_SendData(SPI2,&dummy_write , 1);


		// read the ack byte received
		SPI_ReceiveData(SPI2, &ackbyte, 1);

		if(SPI_VerifyResponse(ackbyte))
		{
			// send agruments
			args[0] = ANALOG_PIN0;


			// send led pin
			SPI_SendData(SPI2,args , 2);

			//do dummy read to clear off the RXNE
			SPI_ReceiveData(SPI2, &dummy_read, 1);


			// insert some delay so that slave can ready with thz data
			delay();

			//send some dummy bits (1byte) to fetch the response from the slave
			SPI_SendData(SPI2,&dummy_write , 1);

			uint8_t analog_read ;
			SPI_ReceiveData(SPI2, &analog_read, 1);
		}



		while (SPI_GETFLAGSTATUS(SPI2 ,SPI_BUSY_FLAG));
		SPI_PeripheralControl(SPI2, DISABLE);


	}


}




