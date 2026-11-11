/*
 * stm32f446xx_i2c_driver.h
 *
 *  Created on: Nov 8, 2026
 *      Author: fuji
 */

#ifndef INC_STM32F446XX_I2C_DRIVER_H_
#define INC_STM32F446XX_I2C_DRIVER_H_

#include "stm32f446xx.h"

/*
 * Configuration structure for I2Cx peripheral
 */
typedef struct
{
    uint32_t I2C_SCLSpeed;       /* possible values from @I2C_SCLSpeed */
    uint8_t  I2C_DeviceAddress;  /* set by the user (7-bit address, slave mode) */
    uint8_t  I2C_ACKControl;     /* possible values from @I2C_ACKControl */
    uint16_t I2C_FMDutyCycle;    /* possible values from @I2C_FMDutyCycle */
} I2C_Config_t;


/*
 * Handle structure for I2Cx peripheral
 */
typedef struct
{
	I2C_RegDef_t *pI2Cx;      /* base address of the I2Cx peripheral (I2C1, I2C...) */
    I2C_Config_t  I2CConfig;  /* I2C configuration settings */

} I2C_Handle_t;

/* @I2C_SCLSpeed */
#define I2C_SCL_SPEED_SM    100000
#define I2C_SCL_SPEED_FM4K  400000
#define I2C_SCL_SPEED_FM2K  200000

/* @I2C_ACKControl */
#define I2C_ACK_ENABLE      1
#define I2C_ACK_DISABLE     0

/* @I2C_FMDutyCycle */
#define I2C_FM_DUTY_2       0
#define I2C_FM_DUTY_16_9    1

/*
 * I2C_SR1 flags
 */
#define I2C_FLAG_SB          (1U << I2C_SR1_SB)
#define I2C_FLAG_ADDR        (1U << I2C_SR1_ADDR)
#define I2C_FLAG_BTF         (1U << I2C_SR1_BTF)
#define I2C_FLAG_ADD10       (1U << I2C_SR1_ADD10)
#define I2C_FLAG_STOPF       (1U << I2C_SR1_STOPF)
#define I2C_FLAG_RXNE        (1U << I2C_SR1_RXNE)
#define I2C_FLAG_TXE         (1U << I2C_SR1_TXE)
#define I2C_FLAG_BERR        (1U << I2C_SR1_BERR)
#define I2C_FLAG_ARLO        (1U << I2C_SR1_ARLO)
#define I2C_FLAG_AF          (1U << I2C_SR1_AF)
#define I2C_FLAG_OVR         (1U << I2C_SR1_OVR)
#define I2C_FLAG_PECERR      (1U << I2C_SR1_PECERR)
#define I2C_FLAG_TIMEOUT     (1U << I2C_SR1_TIMEOUT)
#define I2C_FLAG_SMBALERT    (1U << I2C_SR1_SMBALERT)

/*
 * I2C_SR2 flags
 */
#define I2C_FLAG_MSL         (1U << I2C_SR2_MSL)
#define I2C_FLAG_BUSY        (1U << I2C_SR2_BUSY)
#define I2C_FLAG_TRA         (1U << I2C_SR2_TRA)
#define I2C_FLAG_GENCALL     (1U << I2C_SR2_GENCALL)
#define I2C_FLAG_SMBDEFAULT  (1U << I2C_SR2_SMBDEFAULT)
#define I2C_FLAG_SMBHOST     (1U << I2C_SR2_SMBHOST)
#define I2C_FLAG_DUALF       (1U << I2C_SR2_DUALF)
#define I2C_FLAG_PEC         (0xFFU << I2C_SR2_PEC)


/*******************************************************************************************
 *                     			APIs supported by this driver
 *******************************************************************************************/



/*
 * peripheral clock setup
 */
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx , uint8_t EnorDi);

/*
 * Init and DeInit
 */
void I2C_Init(I2C_Handle_t *pI2CHandle);
void I2C_DeInit(I2C_RegDef_t *pI2Cx);

/*
 * Data Send and Receive
 */
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle , uint8_t *pTxBuffer , uint32_t Len , uint8_t SlaveAdrr);
void I2C_MasterReceiveData(I2C_Handle_t *pI2CHandle , uint8_t *pRxBuffer , uint32_t Len , uint8_t SlaveAdrr);



/*
 * IRQ Configuration and Handling
 */
void I2C_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi );
void I2C_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority);



/*
 * Other Peripheral Control APIs
 */
void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx , uint8_t EnorDi);
uint8_t I2C_GETFLAGSTATUS(I2C_RegDef_t *pI2Cx , uint32_t  FlagName);
void I2C_ManageAcking(I2C_RegDef_t *pI2Cx , uint8_t EnorDi);

/*
 * application call back
 */
void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t appEv);


#endif /* INC_STM32F446XX_I2C_DRIVER_H_ */
