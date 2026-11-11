/*
 * 009i2c_master_rx_testing.c
 *
 *  Created on: Nov 11, 2026
 *      Author: Med Dahou
 */



#include <string.h>
#include "stm32f446xx.h"


#define Slave_Addr 0x68
#define MY_ADDRESS 0x61

//rcv buffer
uint8_t rcv_buffer[32] ;

uint8_t active_command = 0X51;
uint8_t Len = 0;

I2C_Handle_t I2CHandle;

/*
 * PB6 => I2C1_SCL
 * PB7 => I2C1_SDA
 * ALT function mode : 4
 */

void delay(void)
{
	for (volatile uint32_t i = 0; i < 500000; i++);
}

void I2C_GPIOInits()
{
	GPIO_Handle_t I2CPINS = {0};

	I2CPINS.pGPIOx = GPIOB;
	I2CPINS.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	I2CPINS.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
	I2CPINS.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	I2CPINS.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	I2CPINS.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	//SCL
	I2CPINS.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
	GPIO_Init(&I2CPINS);

	//SDA
	I2CPINS.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;
	GPIO_Init(&I2CPINS);
}

void I2C_Inits()
{


	I2CHandle.pI2Cx = I2C1;
	I2CHandle.I2CConfig.I2C_ACKControl = I2C_ACK_ENABLE;
	I2CHandle.I2CConfig.I2C_DeviceAddress = MY_ADDRESS;
	I2CHandle.I2CConfig.I2C_FMDutyCycle = I2C_FM_DUTY_2;
	I2CHandle.I2CConfig.I2C_SCLSpeed = I2C_SCL_SPEED_SM;



	I2C_Init(&I2CHandle);



}

int main()
{
	/*******************Button configuration*************************/
	GPIO_Handle_t GpioBut1 = {0};

	GpioBut1.pGPIOx = GPIOC;
	GpioBut1.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GpioBut1.GPIO_PinConfig.GPIO_PinNumber = 13;
	GpioBut1.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GpioBut1.GPIO_PinConfig.GPIO_PinSpeed       = GPIO_SPEED_LOW;

	GPIOC_PCLK_EN();
	GPIO_Init(&GpioBut1);




	I2C_GPIOInits();
	I2C_Inits();

	// enable the peripheral clock
	I2C_PeriClockControl(I2C1, ENABLE);

	//ack bit is made 1 after PE=1
	I2C_ManageAcking(I2C1 , I2C_ACK_ENABLE);

	while(1)
	{
		while(GPIO_ReadFromInputPin(GPIOC, 13));

		delay();

        //Master sends command code 0x51 to read the length(1 byte) of the data from the slave
		I2C_MasterSendData(&I2CHandle , &active_command , 1 ,  Slave_Addr);


		//Master receive length info from slave
		I2C_MasterReceiveData(&I2CHandle , &Len , 1 , Slave_Addr);

		//Master sends command code 0x52 to read the complete data from the slave
		active_command = 0X52;
		I2C_MasterSendData(&I2CHandle , &active_command , 1 ,  Slave_Addr);

		//Master receive complete data from the slave
		I2C_MasterReceiveData(&I2CHandle , rcv_buffer , Len , Slave_Addr);

	}
}







