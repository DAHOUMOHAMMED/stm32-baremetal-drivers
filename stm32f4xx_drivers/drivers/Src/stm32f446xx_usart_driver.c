/*
 * stm32f446xx_usart_driver.c
 *
 *  Created on: Nov 17, 2026
 *      Author: fuji
 */

#include "stm32f446xx_usart_driver.h"


/*********************************************************************
 * @fn              - USART_PeriClockControl
 *
 * @brief           - This function enables or disables the peripheral clock for the given USART
 *
 * @param[in]       - pUSARTx : base address of the USART peripheral (USART1, USART2, USART3, UART4, UART5, USART6)
 * @param[in]       - EnorDi  : ENABLE or DISABLE macros
 *
 * @return          - none
 *
 * @Note            - The clock must be enabled before writing to any register of the USART
 *********************************************************************/
void USART_PeriClockControl(USART_RegDef_t *pUSARTx, uint8_t EnorDi)
{
	if (EnorDi == ENABLE)
	{

		if (pUSARTx == USART1)
		{
			USART1_PCLK_EN();
		}
		else if (pUSARTx == USART2)
		{
			USART2_PCLK_EN();
		}
		else if (pUSARTx == USART3)
		{
			USART3_PCLK_EN();
		}
		else if (pUSARTx == UART4)
		{
			UART4_PCLK_EN();
		}
		else if (pUSARTx == UART5)
		{
			UART5_PCLK_EN();
		}
		else if (pUSARTx == USART6)
		{
			USART6_PCLK_EN();
		}
	}
	else
	{
		if (pUSARTx == USART1)
		{
			USART1_PCLK_DI();
		}
		else if (pUSARTx == USART2)
		{
			USART2_PCLK_DI();
		}
		else if (pUSARTx == USART3)
		{
			USART3_PCLK_DI();
		}
		else if (pUSARTx == UART4)
		{
			UART4_PCLK_DI();
		}
		else if (pUSARTx == UART5)
		{
			UART5_PCLK_DI();
		}
		else if (pUSARTx == USART6)
		{
			USART6_PCLK_DI();
		}
	}
}






/*********************************************************************
 * @fn              - USART_Init
 *
 * @brief           - This function initializes the given USART peripheral
 *                    (clock, mode, word length, parity, stop bits, hardware flow control)
 *
 * @param[in]       - pUSARTHandle : pointer to the handle structure containing the
 *                    USART base address and its configuration (USART_Config_t)
 *
 * @return          - none
 *
 * @Note            - The peripheral clock is enabled inside this function.
 *                    Baud rate (BRR) configuration is still to be implemented.
 *********************************************************************/
void USART_Init(USART_Handle_t *pUSARTHandle)
{

	//Temporary variable
	uint32_t tempreg=0;

/******************************** Configuration of CR1******************************************/

	//enable the Clock for given USART peripheral
	USART_PeriClockControl(pUSARTHandle->pUSARTx,ENABLE);

	//Enable USART Tx and Rx engines according to the USART_Mode configuration item
	if ( pUSARTHandle->USARTConfig.USART_Mode == USART_MODE_ONLY_RX)
	{
		//enable the Receiver bit field
		tempreg|= (1 << USART_CR1_RE);
	}else if (pUSARTHandle->USARTConfig.USART_Mode == USART_MODE_ONLY_TX)
	{
		//enable the Transmitter bit field
		tempreg |= ( 1 << USART_CR1_TE );

	}else if (pUSARTHandle->USARTConfig.USART_Mode == USART_MODE_TXRX)
	{
		//enable the both Transmitter and Receiver bit fields
		tempreg |= ( ( 1 << USART_CR1_RE) | ( 1 << USART_CR1_TE) );
	}

    //Word length configuration item
	tempreg |= pUSARTHandle->USARTConfig.USART_WordLength << USART_CR1_M ;


    //parity control bit fields
	if ( pUSARTHandle->USARTConfig.USART_ParityControl == USART_PARITY_EN_EVEN)
	{
		//enale the parity control
		tempreg |= ( 1 << USART_CR1_PCE);

		//enable EVEN parity
		//Not required because by default EVEN parity will be selected once you enable the parity control

	}else if (pUSARTHandle->USARTConfig.USART_ParityControl == USART_PARITY_EN_ODD )
	{
		//enable the parity control
	    tempreg |= ( 1 << USART_CR1_PCE);

	    //Implement the code to enable ODD parity
	    tempreg |= ( 1 << USART_CR1_PS);

	}

   //Program the CR1 register
	pUSARTHandle->pUSARTx->USART_CR1 = tempreg;

/******************************** Configuration of CR2******************************************/

	tempreg=0;

	//number of stop bits inserted during USART frame transmission
	tempreg |= pUSARTHandle->USARTConfig.USART_NoOfStopBits << USART_CR2_STOP;

	//Program the CR2 register
	pUSARTHandle->pUSARTx->USART_CR2 = tempreg;

/******************************** Configuration of CR3******************************************/

	tempreg=0;

	//Configuration of USART hardware flow control
	if ( pUSARTHandle->USARTConfig.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS)
	{
		//enable CTS flow control
		tempreg |= ( 1 << USART_CR3_CTSE);


	}else if (pUSARTHandle->USARTConfig.USART_HWFlowControl == USART_HW_FLOW_CTRL_RTS)
	{
		//enable RTS flow control
		tempreg |= ( 1 << USART_CR3_RTSE);

	}else if (pUSARTHandle->USARTConfig.USART_HWFlowControl == USART_HW_FLOW_CTRL_CTS_RTS)
	{
		//enable both CTS and RTS Flow control
		tempreg |= (( 1 << USART_CR3_CTSE) | ( 1 << USART_CR3_RTSE));
	}


	pUSARTHandle->pUSARTx->USART_CR3 = tempreg;

/******************************** Configuration of BRR(Baudrate register)******************************************/

	//Implement the code to configure the baud rate
	//We will cover this in the lecture. No action required here

}


/*********************************************************************
 * @fn              - USART_DeInit
 *
 * @brief           - This function resets all the registers of the given USART peripheral
 *
 * @param[in]       - pUSARTx : base address of the USART peripheral (USART1, USART2, USART3, UART4, UART5 or USART6)
 *
 * @return          - none
 *
 * @Note            - The reset is done through the RCC peripheral reset registers (APB1RSTR / APB2RSTR)
 *********************************************************************/
void USART_DeInit(USART_RegDef_t *pUSARTx)
{

	if (pUSARTx == USART1)
	{
		USART1_REG_RESET();
	}
	else if (pUSARTx == USART2)
	{
		USART2_REG_RESET();
	}
	else if (pUSARTx == USART3)
	{
		USART3_REG_RESET();
	}
	else if (pUSARTx == UART4)
	{
		UART4_REG_RESET();
	}
	else if (pUSARTx == UART5)
	{
		UART5_REG_RESET();
	}
	else if (pUSARTx == USART6)
	{
		USART6_REG_RESET();
	}
}

/*********************************************************************
 * @fn              - USART_GetFlagStatus
 *
 * @brief           - This function returns the state of a flag of the USART status register (SR)
 *
 * @param[in]       - pUSARTx  : base address of the USART peripheral
 * @param[in]       - FlagName : flag mask to test (USART_TXE_FLAG, USART_RXNE_FLAG, USART_TC_FLAG...)
 *
 * @return          - FLAG_SET if the flag is set, FLAG_RESET otherwise
 *
 * @Note            - The flag is a mask with a single bit set: the bitwise AND
 *                    with SR is non-zero only if that bit is 1.
 *********************************************************************/
uint8_t USART_GetFlagStatus(USART_RegDef_t *pUSARTx, uint32_t FlagName)
{
	return (pUSARTx->USART_SR & FlagName) ? FLAG_SET : FLAG_RESET;
}

/*********************************************************************
 * @fn              - USART_SendData
 *
 * @brief           - This function sends Len bytes of data over the given USART
 *                    peripheral (blocking mode)
 *
 * @param[in]       - pUSARTHandle : pointer to the handle structure of the USART
 * @param[in]       - pTxBuffer    : pointer to the data to send
 * @param[in]       - Len          : number of bytes (8-bit frames) or frames to send
 *
 * @return          - none
 *
 * @Note            - Blocking call: it waits for TXE before each frame and for TC
 *                    at the end. In 9-bit mode without parity, each frame uses
 *                    2 bytes of pTxBuffer.
 *********************************************************************/
void USART_SendData(USART_Handle_t *pUSARTHandle, uint8_t *pTxBuffer, uint32_t Len)
{

	uint16_t *pdata;
   //Loop over until "Len" number of bytes are transferred
	for(uint32_t i = 0 ; i < Len; i++)
	{
		//wait until TXE flag is set in the SR
		while(USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_TXE) == FLAG_RESET);


         //Check the USART_WordLength item for 9BIT or 8BIT in a frame
		if(pUSARTHandle->USARTConfig.USART_WordLength == USART_WORDLEN_9BITS)
		{
			//if 9BIT, load the DR with 2bytes masking the bits other than first 9 bits
			pdata = (uint16_t*) pTxBuffer;
			pUSARTHandle->pUSARTx->USART_DR = (*pdata & (uint16_t)0x01FF);

			//check for USART_ParityControl
			if(pUSARTHandle->USARTConfig.USART_ParityControl == USART_PARITY_DISABLE)
			{
				//No parity is used in this transfer. so, 9bits of user data will be sent
				//increment pTxBuffer twice
				pTxBuffer++;
				pTxBuffer++;
			}
			else
			{
				//Parity bit is used in this transfer . so , 8bits of user data will be sent
				//The 9th bit will be replaced by parity bit by the hardware
				pTxBuffer++;
			}
		}
		else
		{
			//This is 8bit data transfer
			pUSARTHandle->pUSARTx->USART_DR = (*pTxBuffer  & (uint8_t)0xFF);

			//increment the buffer address
			pTxBuffer++;
		}
	}

	//wait till TC flag is set in the SR
	while(USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_TC) == FLAG_RESET);
}


/*********************************************************************
 * @fn              - USART_ReceiveData
 *
 * @brief           - This function receives Len frames of data over the given USART
 *                    peripheral (blocking mode)
 *
 * @param[in]       - pUSARTHandle : pointer to the handle structure of the USART
 * @param[in]       - pRxBuffer    : pointer to the buffer where received data is stored
 * @param[in]       - Len          : number of frames to receive
 *
 * @return          - none
 *
 * @Note            - Blocking call: it waits for RXNE before reading each frame.
 *                    When parity is enabled, the parity bit is masked out of the data.
 *                    In 9-bit mode without parity, each frame uses 2 bytes of pRxBuffer.
 *********************************************************************/
void USART_ReceiveData(USART_Handle_t *pUSARTHandle, uint8_t *pRxBuffer, uint32_t Len)
{
   //Loop over until "Len" number of bytes are transferred
	for(uint32_t i = 0 ; i < Len; i++)
	{
		//wait until RXNE flag is set in the SR
		while(USART_GetFlagStatus(pUSARTHandle->pUSARTx, USART_FLAG_RXNE) == FLAG_RESET);

		//receive 9bit of data in a frame or 8 bit
		if(pUSARTHandle->USARTConfig.USART_WordLength == USART_WORDLEN_9BITS)
		{
			//We are going to receive 9bit data in a frame

			//check are we using USART_ParityControl control or not
			if(pUSARTHandle->USARTConfig.USART_ParityControl == USART_PARITY_DISABLE)
			{
				//No parity is used. so, all 9bits will be of user data

				//read only first 9 bits. so, mask the DR with 0x01FF
				*((uint16_t*) pRxBuffer) = (pUSARTHandle->pUSARTx->USART_DR  & (uint16_t)0x01FF);

				//Now increment the pRxBuffer two times
				pRxBuffer++;
				pRxBuffer++;
			}
			else
			{
				//Parity is used, so, 8bits will be of user data and 1 bit is parity
				 *pRxBuffer = (pUSARTHandle->pUSARTx->USART_DR  & (uint8_t)0xFF);

				 //Increment the pRxBuffer
				 pRxBuffer++;
			}
		}
		else
		{
			//We are going to receive 8bit data in a frame

			//check are we using USART_ParityControl control or not
			if(pUSARTHandle->USARTConfig.USART_ParityControl == USART_PARITY_DISABLE)
			{
				//No parity is used , so all 8bits will be of user data

				//read 8 bits from DR
				 *pRxBuffer = (uint8_t) (pUSARTHandle->pUSARTx->USART_DR  & (uint8_t)0xFF);
			}

			else
			{
				//Parity is used, so , 7 bits will be of user data and 1 bit is parity

				//read only 7 bits , hence mask the DR with 0X7F
				 *pRxBuffer = (uint8_t)  (pUSARTHandle->pUSARTx->USART_DR  & (uint8_t)0x7F);

			}

			//increment the pRxBuffer
			pRxBuffer++;
		}
	}

}

/*********************************************************************
 * @fn      		  - USART_SendDataWithIT
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              - Resolve all the TODOs

 */
/*
uint8_t USART_SendDataIT(USART_Handle_t *pUSARTHandle,uint8_t *pTxBuffer, uint32_t Len)
{
	uint8_t txstate = pUSARTHandle->TODO;

	if(txstate != USART_BUSY_IN_TX)
	{
		pUSARTHandle->TODO = Len;
		pUSARTHandle->pTxBuffer = TODO;
		pUSARTHandle->TxBusyState = TODO;

		//Implement the code to enable interrupt for TXE
		TODO


		//Implement the code to enable interrupt for TC
		TODO


	}

	return txstate;

}

*/
/*********************************************************************
 * @fn      		  - USART_ReceiveDataIT
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              - Resolve all the TODOs

 */
/*
uint8_t USART_ReceiveDataIT(USART_Handle_t *pUSARTHandle,uint8_t *pRxBuffer, uint32_t Len)
{
	uint8_t rxstate = pUSARTHandle->TODO;

	if(rxstate != TODO)
	{
		pUSARTHandle->RxLen = Len;
		pUSARTHandle->pRxBuffer = TODO;
		pUSARTHandle->RxBusyState = TODO;

		//Implement the code to enable interrupt for RXNE
		TODO

	}

	return rxstate;

}
*/
