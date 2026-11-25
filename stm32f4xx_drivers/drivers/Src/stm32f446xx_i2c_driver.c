/*
 * stm32f446xx_i2c_driver.c
 *
 *  Created on: Nov 8, 2026
 *      Author: Med Dahou
 */

#include "stm32f446xx_i2c_driver.h"

static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx);
static void I2C_ExecuteAddresPhaseWrite(I2C_Handle_t *pI2CHandle, uint8_t SlaveAdrr);
static void I2C_ExecuteAddresPhaseRead(I2C_Handle_t *pI2CHandle, uint8_t SlaveAdrr);
//static void I2C_ClearADDRFlag(I2C_RegDef_t *pI2Cx);
static void I2C_ClearADDRFlag(I2C_Handle_t *pI2CHandle);
static void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx);


/*
 * @fn              - I2C_GenerateStartCondition
 *
 * @brief           - Generates a START condition on the I2C bus
 *
 * @param[in]       - pI2Cx: Pointer to the I2C peripheral register structure
 *
 * @return          - None
 *
 * @note            - Sets the START bit in the CR1 register to initiate
 *                    an I2C communication.
 */
static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx)
{
	pI2Cx->CR1 |= ( 1 << I2C_CR1_START);
}


/*
 * @fn              - I2C_GenerateStopCondition
 *
 * @brief           - Generates a STOP condition on the I2C bus
 *
 * @param[in]       - pI2Cx: Pointer to the I2C peripheral register structure
 *
 * @return          - None
 *
 * @note            - Sets the STOP bit in the CR1 register to terminate
 *                    the I2C communication.
 */
static void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx)
{
	pI2Cx->CR1 |= ( 1 << I2C_CR1_STOP);
}

/*
 * @fn              - I2C_ExecuteAddressPhase
 *
 * @brief           - Executes the address phase of the I2C communication
 *
 * @param[in]       - pI2CHandle: Pointer to the I2C handle structure
 * @param[in]       - SlaveAddr: 7-bit slave address
 *
 * @return          - None
 *
 * @note            - Configures the R/W bit to 0 (Write operation)
 *                    and sends the slave address through the Data Register (DR).
 */
static void I2C_ExecuteAddresPhaseWrite(I2C_Handle_t *pI2CHandle, uint8_t SlaveAdrr)
{
	SlaveAdrr = SlaveAdrr << 1;
	SlaveAdrr &= ~(1);
	pI2CHandle->pI2Cx->DR = SlaveAdrr;
}




/*
 * @fn              - I2C_ExecuteAddressPhaseRead
 *
 * @brief           - Executes the address phase of the I2C communication
 *
 * @param[in]       - pI2CHandle: Pointer to the I2C handle structure
 * @param[in]       - SlaveAddr: 7-bit slave address
 *
 * @return          - None
 *
 * @note            - Configures the R/W bit to 1 (Read operation)
 *                    and sends the slave address through the Data Register (DR).
 */
static void I2C_ExecuteAddresPhaseRead(I2C_Handle_t *pI2CHandle, uint8_t SlaveAdrr)
{
	SlaveAdrr = SlaveAdrr << 1;
	SlaveAdrr |= (1);
	pI2CHandle->pI2Cx->DR = SlaveAdrr;


}

/*
 * @fn              - I2C_ClearADDRFlag
 *
 * @brief           - Clears the ADDR flag in the I2C_SR1 register
 *
 * @param[in]       - pI2Cx: Pointer to the I2C peripheral register structure
 *
 * @return          - None
 *
 * @note            - The ADDR flag is cleared by reading SR1 followed by SR2.
 */
/*static void I2C_ClearADDRFlag(I2C_RegDef_t *pI2Cx)
{
	uint32_t dummyRead = pI2Cx->SR1;
	dummyRead = pI2Cx->SR2;

	(void) dummyRead;

}*/

static void I2C_ClearADDRFlag(I2C_Handle_t *pI2CHandle)
{
	//check for device mode
	if(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_MSL))
	{
		//device is in master mode
		if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
		{
			if(pI2CHandle->RxSize == 1)
			{
				//first disable the  ack
				I2C_ManageAcking(pI2CHandle->pI2Cx, DISABLE);

				//clear the addr flag
				I2C_ClearADDRFlag(pI2CHandle);


			}
		}
		else
		{
			//clear the addr flag
			I2C_ClearADDRFlag(pI2CHandle);
		}
	}
	else
	{
		//device is in slave mode
		//clear the addr flag
		I2C_ClearADDRFlag(pI2CHandle);
	}

}





/*********************************************************************
 * @fn              - I2C_PeriClockControl
 *
 * @brief           - This function enables or disables the peripheral clock for the given I2C
 *
 * @param[in]       - base address of the I2C peripheral (I2C1, I2C2 or I2C3 )
 * @param[in]       - ENABLE or DISABLE macros
 *
 * @return          - none
 *
 * @Note            - The clock must be enabled before writing to any register of the I2C
 *********************************************************************/
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx , uint8_t EnorDi)
{
	if (EnorDi == ENABLE)
	{
		if (pI2Cx == I2C1)
		{
			I2C1_PCLK_EN();
		}
		else if (pI2Cx == I2C2)
		{
			I2C2_PCLK_EN();
		}
		else if (pI2Cx == I2C3)
		{
			I2C3_PCLK_EN();
		}

	}
	else
	{
		if (pI2Cx == I2C1)
		{
			I2C1_PCLK_DI();
		}
		else if (pI2Cx == I2C2)
		{
			I2C2_PCLK_DI();
		}
		else if (pI2Cx == I2C3)
		{
			I2C3_PCLK_DI();
		}

	}
}








void I2C_Init(I2C_Handle_t *pI2CHandle)
{
	uint32_t tempreg = 0;

	// enable the peripheral clock
	I2C_PeriClockControl(pI2CHandle->pI2Cx, ENABLE);

	// ack control bit
	tempreg |= pI2CHandle->I2CConfig.I2C_ACKControl << I2C_CR1_ACK ;
	pI2CHandle->pI2Cx->CR1 = tempreg;

	//configure the Freq field of CR2
	tempreg = 0;
	tempreg |= RCC_GetPCLK1Value() / 1000000U;
	pI2CHandle->pI2Cx->CR2 = (tempreg & (0x3f));

	//program the device own address
	tempreg = 0;
	tempreg |= pI2CHandle->I2CConfig.I2C_DeviceAddress << 1;
	tempreg |= (1 << 14);
	pI2CHandle->pI2Cx->OAR1 = tempreg;

	// configure the CCR register
	uint16_t ccr_value = 0;
	tempreg = 0;

	if(pI2CHandle->I2CConfig.I2C_SCLSpeed <= I2C_SCL_SPEED_SM)
	{
		// standard mode
		ccr_value = RCC_GetPCLK1Value() / (2 * pI2CHandle->I2CConfig.I2C_SCLSpeed);
		tempreg |= ccr_value & 0xFFF;

	}

	else
	{
		//fast mode
		tempreg |= (1 << 15);
		tempreg |= (pI2CHandle->I2CConfig.I2C_FMDutyCycle << 14);

		if(pI2CHandle->I2CConfig.I2C_FMDutyCycle == I2C_FM_DUTY_2)
		{
			ccr_value = RCC_GetPCLK1Value() / (3 * pI2CHandle->I2CConfig.I2C_SCLSpeed);
		}

		else
		{
			ccr_value = RCC_GetPCLK1Value() / (25 * pI2CHandle->I2CConfig.I2C_SCLSpeed);
		}
		tempreg |= ccr_value & 0xFFF;
	}
	pI2CHandle->pI2Cx->CCR = tempreg;

}



/*********************************************************************
 * @fn              - I2C_DeInit
 *
 * @brief           - This function resets all the registers of the given I2C peripheral
 *
 * @param[in]       - base address of the I2C peripheral (I2C1, I2C2 or I2C3)
 *
 * @return          - none
 *
 * @Note            - The reset is done through the RCC peripheral reset registers
 *********************************************************************/
void I2C_DeInit(I2C_RegDef_t *pI2Cx)
{
	if (pI2Cx == I2C1)
	{
		I2C1_REG_RESET();
	}
	else if (pI2Cx == I2C2)
	{
		I2C2_REG_RESET();
	}
	else if (pI2Cx == I2C3)
	{
		I2C3_REG_RESET();
	}
}


/*********************************************************************
 * @fn              - SPI_GETFLAGSTATUS
 *
 * @brief           - This function returns the state of a flag of the SPI status register (SR)
 *
 * @param[in]       - base address of the SPI peripheral
 * @param[in]       - flag mask to test (SPI_TXE_FLAG, SPI_RXNE_FLAG, SPI_BUSY_FLAG...)
 *
 * @return          - FLAG_SET if the flag is set, FLAG_RESET otherwise
 *
 * @Note            - The flag is a mask with a single bit set: the bitwise AND
 *                    with SR is non-zero only if that bit is 1.
 *********************************************************************/
uint8_t I2C_GETFLAGSTATUS(I2C_RegDef_t *pI2Cx, uint32_t FlagName)
{
	return (pI2Cx->SR1 & FlagName) ? FLAG_SET : FLAG_RESET;
}



void I2C_MasterSendData(I2C_Handle_t *pI2CHandle , uint8_t *pTxBuffer , uint32_t Len , uint8_t SlaveAdrr , uint8_t Sr)
{
	//1. Generate the START condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	//2. confirm that start is start generation is completed by checking the SB flag in the SR1
	while(!I2C_GETFLAGSTATUS(pI2CHandle->pI2Cx, I2C_FLAG_SB));

	//3. Send the address of the slave with r/nw bit set to w(0) (total 8 bits)
	I2C_ExecuteAddresPhaseWrite(pI2CHandle,  SlaveAdrr);

	//4. confirm that addres phase is completed by checking the ADDR flag in the teh SR1
	while(!I2C_GETFLAGSTATUS(pI2CHandle->pI2Cx, I2C_FLAG_ADDR));

	//5. clear the ADDR flag according to its software sequence
	I2C_ClearADDRFlag(pI2CHandle);

	//6. send the data until len becomes 0
	while(Len > 0)
	{
		while(!I2C_GETFLAGSTATUS(pI2CHandle->pI2Cx, I2C_FLAG_TXE));

		pI2CHandle->pI2Cx->DR = *pTxBuffer;
		pTxBuffer ++;
		Len --;
	}

	//7. when Len becomes 0 wait for TXE=1 et BTF=1 before generating the stop condition
	while(!I2C_GETFLAGSTATUS(pI2CHandle->pI2Cx, I2C_FLAG_TXE));
	while(!I2C_GETFLAGSTATUS(pI2CHandle->pI2Cx, I2C_FLAG_BTF));

	//8. Generate the stop condition

	if(Sr == I2C_DISABLE_SR)
	{
		I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
	}



}

void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle , uint8_t *pRxBuffer , uint32_t Len , uint8_t SlaveAdrr , uint8_t Sr)
{
	//1. Generate the START condition
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	//2. confirm that start is start generation is completed by checking the SB flag in the SR1
	while(!I2C_GETFLAGSTATUS(pI2CHandle->pI2Cx, I2C_FLAG_SB));


	//3. Send the address of the slave with r/nw bit set to r(1) (total 8 bits)
	I2C_ExecuteAddresPhaseRead(pI2CHandle,  SlaveAdrr);

	//4. confirm that addres phase is completed by checking the ADDR flag in the teh SR1
	while(!I2C_GETFLAGSTATUS(pI2CHandle->pI2Cx, I2C_FLAG_ADDR));


	//procedure to read only 1byt from slave
	if(Len == 1)
	{
		//Disable Acking
		I2C_ManageAcking(pI2CHandle->pI2Cx , I2C_ACK_DISABLE);

		// clear the ADDR flag according to its software sequence
		I2C_ClearADDRFlag(pI2CHandle);

		//wait until RXNE becomes 1
		while(!I2C_GETFLAGSTATUS(pI2CHandle->pI2Cx, I2C_FLAG_RXNE));

		// Generate the stop condition
		if(Sr == I2C_DISABLE_SR)
		{
			I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
		}


		//read last byt
		*pRxBuffer =  pI2CHandle->pI2Cx->DR;


	}


	if(Len > 1)
	{
		// clear the ADDR flag according to its software sequence
		I2C_ClearADDRFlag(pI2CHandle);

		//read data until len becomes 0
		for(uint32_t i = Len ; i >0 ; i++)
		{
			//wait until RXNE becomes 1
			while(!I2C_GETFLAGSTATUS(pI2CHandle->pI2Cx, I2C_FLAG_RXNE));

			if(Len == 2)
			{
				//Disable Acking
				I2C_ManageAcking(pI2CHandle->pI2Cx , I2C_ACK_DISABLE);

				// Generate the stop condition
				if(Sr == I2C_DISABLE_SR)
				{
					I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
				}

			}

			//read the data from data register in to buffer
			*pRxBuffer =  pI2CHandle->pI2Cx->DR;

			//increment the buffer address
			pRxBuffer ++;

		}


	}

	//re-enable acking

	if(pI2CHandle->I2CConfig.I2C_ACKControl == I2C_ACK_ENABLE)
	{
		I2C_ManageAcking(pI2CHandle->pI2Cx , I2C_ACK_ENABLE);
	}

}

/*********************************************************************
 *
 * @fn                  - I2C_MasterSendDataIT
 *
 * @brief               - Sends data to an I2C slave device using
 *                        interrupt mode (non-blocking).
 *
 * @param[in]           - pI2CHandle : Pointer to the I2C handle structure
 * @param[in]           - pTxBuffer  : Pointer to the transmit buffer
 * @param[in]           - Len        : Number of bytes to transmit
 * @param[in]           - SlaveAddr  : Address of the I2C slave device
 * @param[in]           - Sr         : Repeated START condition control
 *
 * @return              - Current I2C bus state
 *
 * @Note                - The transmission is handled asynchronously
 *                        through I2C interrupts.
 *
 *********************************************************************/
uint8_t  I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle,uint8_t *pTxBuffer, uint32_t Len,uint8_t SlaveAddr,uint8_t Sr)
{

	uint8_t busystate = pI2CHandle->TxRxState;

	if( (busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
	{
		pI2CHandle->pTxBuffer = pTxBuffer;
		pI2CHandle->TxLen = Len;
		pI2CHandle->TxRxState = I2C_BUSY_IN_TX;
		pI2CHandle->DevAddr = SlaveAddr;
		pI2CHandle->Sr = Sr;

		//Implement code to Generate START Condition
		I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

		//Implement the code to enable ITBUFEN Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITBUFEN);

		//Implement the code to enable ITEVFEN Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITEVTEN);

		//Implement the code to enable ITERREN Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITERREN);

	}

	return busystate;

}


/* @fn                  - I2C_MasterReceiveDataIT
*
* @brief               - Receives data from an I2C slave device using
*                        interrupt mode (non-blocking).
*
* @param[in]           - pI2CHandle : Pointer to the I2C handle structure
* @param[in]           - pRxBuffer  : Pointer to the receive buffer
* @param[in]           - Len        : Number of bytes to receive
* @param[in]           - SlaveAddr  : Address of the I2C slave device
* @param[in]           - Sr         : Repeated START condition control
*
* @return              - Current bus state before starting reception
*
* @Note                - The reception is handled asynchronously through
*                        I2C interrupts. The application can continue
*                        executing while data is being received.*/
uint8_t I2C_MasterReceiveDataIT(I2C_Handle_t *pI2CHandle,uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t Sr)
{

	uint8_t busystate = pI2CHandle->TxRxState;

	if( (busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
	{
		pI2CHandle->pRxBuffer = pRxBuffer;
		pI2CHandle->RxLen = Len;
		pI2CHandle->TxRxState = I2C_BUSY_IN_RX;
		pI2CHandle->RxSize = Len; //Rxsize is used in the ISR code to manage the data reception
		pI2CHandle->DevAddr = SlaveAddr;
		pI2CHandle->Sr = Sr;

		//Implement code to Generate START Condition
		I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

		//Implement the code to enable ITBUFEN Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITBUFEN);

		//Implement the code to enable ITEVFEN Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITEVTEN);

		//Implement the code to enable ITERREN Control Bit
		pI2CHandle->pI2Cx->CR2 |= ( 1 << I2C_CR2_ITERREN);


	}

	return busystate;
}

void I2C_ManageAcking(I2C_RegDef_t *pI2Cx , uint8_t EnorDi)
{
	if(EnorDi == I2C_ACK_ENABLE)
	{
		//enable the ack
		pI2Cx->CR1 |= (1 << I2C_CR1_ACK);
	}
	else
	{
		//desable the ack
		pI2Cx->CR1 &= ~(1 << I2C_CR1_ACK);
	}
}

void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi );
void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority);

void I2C_MasterHandleTXEInterrupt(I2C_Handle_t *pI2CHandle)
{
	if(pI2CHandle->TxLen > 1)
	{
		//1. load the data in to DR
		pI2CHandle->pI2Cx->DR = *pI2CHandle->pTxBuffer;

		//2. decrement the TxLen
		pI2CHandle->TxLen--;

		//3. Increment the buffer address
		pI2CHandle->pTxBuffer++;

	}
}

void I2C_CloseReceiveData(I2C_Handle_t *pI2CHandle)
{
	//Implement the code to disable ITBUFEN control bit
	pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITBUFEN);

	//Implement the code to disable ITEVFEN control bit
	pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITEVTEN);

	pI2CHandle->TxRxState = I2C_READY;
	pI2CHandle->pRxBuffer = NULL;
	pI2CHandle->RxLen = 0;
	pI2CHandle->RxSize=0;

	if(pI2CHandle->I2CConfig.I2C_ACKControl == I2C_ACK_ENABLE)
	{
		I2C_ManageAcking(pI2CHandle->pI2Cx, ENABLE);
	}

}

void I2C_CloseSendData(I2C_Handle_t *pI2CHandle)
{
	//Implement the code to disable ITBUFEN control bit
	pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITBUFEN);

	//Implement the code to disable ITEVFEN control bit
	pI2CHandle->pI2Cx->CR2 &= ~(1 << I2C_CR2_ITEVTEN);

	pI2CHandle->TxRxState = I2C_READY;
	pI2CHandle->pTxBuffer = NULL;
	pI2CHandle->TxLen = 0;
}

void I2C_MasterHandleRXNEInterrupt(I2C_Handle_t *pI2CHandle)
{
	//RXNE flag is set
	if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
	{
		//we have to do data reception
		if(pI2CHandle->RxSize == 1)
		{
			*pI2CHandle->pRxBuffer = pI2CHandle->pI2Cx->DR;
			pI2CHandle->RxLen--;
		}

		if(pI2CHandle->RxSize > 1)
		{
			if(pI2CHandle->RxLen == 2)
			{
				//clear the ack bit
				I2C_ManageAcking(pI2CHandle->pI2Cx, DISABLE);
			}

			*pI2CHandle->pRxBuffer = pI2CHandle->pI2Cx->DR;
			pI2CHandle->pRxBuffer++;
			pI2CHandle->RxLen--;

		}

		if(pI2CHandle->RxLen == 0)
		{
			//close the i2c data reception and notify the application

			//1.generate the stop condition
			if(pI2CHandle->Sr == I2C_DISABLE_SR)
				I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

			//2.close the i2c rx
			I2C_CloseReceiveData(pI2CHandle);

			//3.notify the application
			I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_RX_CMPLT);
		}
	}
}

void I2C_EV_IRQHandling(I2C_Handle_t *pI2CHandle)
{
	//Interrupt handling for both master and slave mode of a device
	uint32_t temp1, temp2, temp3;

	temp1 = pI2CHandle->pI2Cx->CR2 & ( 1 << I2C_CR2_ITEVTEN);
	temp2 = pI2CHandle->pI2Cx->CR2 & ( 1 << I2C_CR2_ITBUFEN);
	temp3 = pI2CHandle->pI2Cx->SR1 & ( 1 << I2C_SR1_SB);

	//1. Handle For interrupt generated by SB event
	//	Note : SB flag is only applicable in Master mode
	if(temp1 && temp3)
	{
		if( pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
		{
			I2C_ExecuteAddresPhaseWrite(pI2CHandle, pI2CHandle->DevAddr);
		}

		else if ( pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
		{
			I2C_ExecuteAddresPhaseRead(pI2CHandle, pI2CHandle->DevAddr);
		}
	}

	temp3 = pI2CHandle->pI2Cx->SR1 & ( 1 << I2C_SR1_ADDR);
	//2. Handle For interrupt generated by ADDR event
	//Note : When master mode : Address is sent
	//		 When Slave mode   : Address matched with own address
	if(temp1 && temp3)
	{
		I2C_ClearADDRFlag(pI2CHandle);
	}

	temp3 = pI2CHandle->pI2Cx->SR1 & ( 1 << I2C_SR1_BTF);
	//3. Handle For interrupt generated by BTF(Byte Transfer Finished) event
	if(temp1 && temp3)
	{
		//BTF flag is set
		if( pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
		{
			//make sure that TXE is also set.
			if(pI2CHandle->pI2Cx->SR1 & ( 1 << I2C_SR1_TXE))
			{

				if(pI2CHandle->TxLen == 0)
				{
					//1. generate the stop condition
					if(pI2CHandle->Sr == I2C_DISABLE_SR)
						I2C_GenerateStopCondition(pI2CHandle->pI2Cx);

					//2. reset all the member elements of the handle structure
					I2C_CloseSendData(pI2CHandle);

					//3. notify the application about transmission complete
					I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_TX_CMPLT);
				}

			}
		}

		else if ( pI2CHandle->TxRxState == I2C_BUSY_IN_RX)
		{
			;
		}
	}

	temp3 = pI2CHandle->pI2Cx->SR1 & ( 1 << I2C_SR1_STOPF);
	//4. Handle For interrupt generated by STOPF event
	// Note : Stop detection flag is applicable only slave mode . For master this flag will never be set
	if(temp1 && temp3)
	{
		//STOPF flag is set
		//Clear the STOPF (read SR1 & write to CR1)
		pI2CHandle->pI2Cx->CR1 |= 0x0000;

		//notify application that stop detected
		I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_STOP);
	}

	temp3 = pI2CHandle->pI2Cx->SR1 & ( 1 << I2C_SR1_TXE);
	//5. Handle For interrupt generated by TXE event
	if(temp1 && temp2 && temp3)
	{
		if(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_MSL))
		{
			//TXE flag is set
			//we have to do data transmission
			if( pI2CHandle->TxRxState == I2C_BUSY_IN_TX)
			{
				I2C_MasterHandleTXEInterrupt(pI2CHandle);
			}
		}


	}

	temp3 = pI2CHandle->pI2Cx->SR1 & ( 1 << I2C_SR1_RXNE);
	//6. Handle For interrupt generated by RXNE event
	if(temp1 &&  temp2 && temp3)
	{
		//check device mode
		if(pI2CHandle->pI2Cx->SR2 & (1 << I2C_SR2_MSL))
		{
			I2C_MasterHandleRXNEInterrupt(pI2CHandle);
		}

	}
}
void I2C_ER_IRQHandling(I2C_Handle_t *pI2CHandle);

