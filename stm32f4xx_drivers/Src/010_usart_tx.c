
/*
 * 010_usart_tx.c
 *
 *  Created on: Nov 18, 2026
 *      Author: Med Dahou
 */

#include "stm32f446xx.h"
#include <string.h>

USART_Handle_t USART1Handle = {0};

char msg[1012] = "Inchalah\n\r";

void delay(void)
{
    for (volatile uint32_t i = 0; i < 500000; i++);
}

void USART1_GPIOInits(void)
{
    GPIO_Handle_t USARTPINS = {0};

    USARTPINS.pGPIOx = GPIOB;
    USARTPINS.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
    USARTPINS.GPIO_PinConfig.GPIO_PinAltFunMode = 7;
    USARTPINS.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
    USARTPINS.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
    USARTPINS.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

    // TX => PB6
    USARTPINS.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
    GPIO_Init(&USARTPINS);

    // RX => PB7
    USARTPINS.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;
    GPIO_Init(&USARTPINS);
}

void USART1_Inits(void)
{
    USART1Handle.pUSARTx = USART1;

    USART1Handle.USARTConfig.USART_Baud = 115200;
    USART1Handle.USARTConfig.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
    USART1Handle.USARTConfig.USART_Mode = USART_MODE_ONLY_TX;
    USART1Handle.USARTConfig.USART_NoOfStopBits = USART_STOPBITS_1;
    USART1Handle.USARTConfig.USART_ParityControl = USART_PARITY_DISABLE;
    USART1Handle.USARTConfig.USART_WordLength = USART_WORDLEN_8BITS;

    USART_Init(&USART1Handle);
}

int main(void)
{
    /*************** Button configuration ***************/

    GPIO_Handle_t GpioBut1 = {0};

    GpioBut1.pGPIOx = GPIOC;
    GpioBut1.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
    GpioBut1.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
    GpioBut1.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
    GpioBut1.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_LOW;

    GPIOC_PCLK_EN();
    GPIO_Init(&GpioBut1);

    /*************** USART1 configuration ***************/

    USART1_GPIOInits();
    USART1_Inits();

    USART_PeripheralControl(USART1, ENABLE);

    while (1)
    {
        while (GPIO_ReadFromInputPin(GPIOC, 13));

        delay();

        USART_SendData(&USART1Handle, (uint8_t*)msg, strlen(msg));
    }

    return 0;
}
