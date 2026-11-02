/*
 * stm32f446xx_gpio_driver.h
 *
 *  Created on: Nov 1, 2026
 *      Author: Mohammed Dahou
 */

#ifndef INC_STM32F446XX_GPIO_DRIVER_H_
#define INC_STM32F446XX_GPIO_DRIVER_H_

#include "stm32f446xx.h"


/*
 * this is a configuration structure for GPIO pin
 */


typedef struct
{
    uint8_t GPIO_PinNumber;       /* possible values from @GPIO_PIN_NUMBERS */
    uint8_t GPIO_PinMode;         /* possible values from @GPIO_PIN_MODES */
    uint8_t GPIO_PinSpeed;        /* possible values from @GPIO_PIN_SPEED */
    uint8_t GPIO_PinPuPdControl;  /* possible values from @GPIO_PIN_PUPD */
    uint8_t GPIO_PinOPType;       /* possible values from @GPIO_PIN_OP_TYPES */
    uint8_t GPIO_PinAltFunMode;   /* alternate function number, 0 to 15 (AF0 to AF15) */
} GPIO_PinConfig_t;


/*
 * this is a handle structure for a GPIO pin
 */
typedef struct
{
    GPIO_RegDef_t     *pGPIOx;        /* base address of the GPIO port the pin belongs to */
    GPIO_PinConfig_t  GPIO_PinConfig; /* pin configuration settings */
} GPIO_Handle_t;


/*
 * @GPIO_PIN_NUMBERS
 * GPIO pin numbers
 */
/*
 * @GPIO_PIN_NUMBERS
 * GPIO pin numbers
 */
#define GPIO_PIN_NO_0       0
#define GPIO_PIN_NO_1       1
#define GPIO_PIN_NO_2       2
#define GPIO_PIN_NO_3       3
#define GPIO_PIN_NO_4       4
#define GPIO_PIN_NO_5       5
#define GPIO_PIN_NO_6       6
#define GPIO_PIN_NO_7       7
#define GPIO_PIN_NO_8       8
#define GPIO_PIN_NO_9       9
#define GPIO_PIN_NO_10      10
#define GPIO_PIN_NO_11      11
#define GPIO_PIN_NO_12      12
#define GPIO_PIN_NO_13      13
#define GPIO_PIN_NO_14      14
#define GPIO_PIN_NO_15      15


/*
 * @GPIO_PIN_MODES
 * GPIO pin possible modes
 */
#define GPIO_MODE_IN        0
#define GPIO_MODE_OUT       1
#define GPIO_MODE_ALTFN     2
#define GPIO_MODE_ANALOG    3
#define GPIO_MODE_IT_FT     4
#define GPIO_MODE_IT_RT     5
#define GPIO_MODE_IT_RFT    6


/*
 * @GPIO_PIN_OP_TYPES
 * GPIO pin possible output types
 */
#define GPIO_OP_TYPE_PP     0     /* push-pull */
#define GPIO_OP_TYPE_OD     1     /* open-drain */


/*
 * @GPIO_PIN_SPEED
 * GPIO pin possible output speeds
 */
#define GPIO_SPEED_LOW      0
#define GPIO_SPEED_MEDIUM   1
#define GPIO_SPEED_FAST     2
#define GPIO_SPEED_HIGH     3


/*
 * @GPIO_PIN_PUPD
 * GPIO pin pull-up and pull-down configuration macros
 */
#define GPIO_NO_PUPD        0
#define GPIO_PIN_PU         1
#define GPIO_PIN_PD         2


/*******************************************************************************************
 *                     			APIs supported by this driver
 *******************************************************************************************/



/*
 * peripheral clock setup
 */
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx , uint8_t EnorDi);

/*
 * Init and DeInit
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);

/*
 * Data Read and Write
 */
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx , uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx );
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx , uint8_t PinNumber , uint8_t Value);
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value);
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx , uint8_t PinNumber);

/*
 * IRQ Configuration and Handling
 */
void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnorDi );
void GPIO_IRQHandling(uint8_t PinNumber);


#endif /* INC_STM32F446XX_GPIO_DRIVER_H_ */
