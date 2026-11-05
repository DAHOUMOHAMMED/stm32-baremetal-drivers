/*
 * stm32f446xx_spi_driver.h
 *
 *  Created on: Nov 4, 2026
 *      Author: fuji
 */

#ifndef INC_STM32F446XX_SPI_DRIVER_H_
#define INC_STM32F446XX_SPI_DRIVER_H_

#include "stm32f446xx.h"


/*
 * Configuration structure for SPIx peripheral
 */
typedef struct
{
    uint8_t SPI_DeviceMode;   /* possible values from @SPI_DeviceMode */
    uint8_t SPI_BusConfig;    /* possible values from @SPI_BusConfig */
    uint8_t SPI_SclkSpeed;    /* possible values from @SPI_SclkSpeed */
    uint8_t SPI_DFF;          /* possible values from @SPI_DFF */
    uint8_t SPI_CPOL;         /* possible values from @SPI_CPOL */
    uint8_t SPI_CPHA;         /* possible values from @SPI_CPHA */
    uint8_t SPI_SSM;          /* possible values from @SPI_SSM */
} SPI_Config_t;


/*
 * Handle structure for SPIx peripheral
 */
typedef struct
{
    SPI_RegDef_t *pSPIx;      /* base address of the SPIx peripheral (SPI1, SPI2...) */
    SPI_Config_t  SPIConfig;  /* SPI configuration settings */
} SPI_Handle_t;



/*
 * @SPI_Device_Mode
 */
#define SPI_Device_Mode_Master 1
#define SPI_Device_Mode_Slave  0

/*
 * @SPI_BusConfig
 */
#define SPI_Bus_Config_FD				1
#define SPI_Bus_Config_HD				2
#define SPI_Bus_Config_SIMPLEX_RXONLY	3

/*
 * @SPI_SclkSpeed
 * SPI serial clock speed (BR[2:0] bits of SPI_CR1): fSCLK = fPCLK / divider
 */
#define SPI_SCLK_SPEED_DIV2             0
#define SPI_SCLK_SPEED_DIV4             1
#define SPI_SCLK_SPEED_DIV8             2
#define SPI_SCLK_SPEED_DIV16            3
#define SPI_SCLK_SPEED_DIV32            4
#define SPI_SCLK_SPEED_DIV64            5
#define SPI_SCLK_SPEED_DIV128           6
#define SPI_SCLK_SPEED_DIV256           7


/*
 * @SPI_DFF
 * Data frame format: number of bits per transfer (DFF bit of SPI_CR1)
 */
#define SPI_DFF_8BITS                   0
#define SPI_DFF_16BITS                  1

/*
 * @SPI_CPOL
 * Clock polarity: level of SCK when idle (CPOL bit of SPI_CR1)
 */
#define SPI_CPOL_LOW                    0
#define SPI_CPOL_HIGH                   1

/*
 * @SPI_CPHA
 * Clock phase: which clock edge samples the data (CPHA bit of SPI_CR1)
 * 0 = first edge, 1 = second edge
 */
#define SPI_CPHA_LOW                    0
#define SPI_CPHA_HIGH                   1

/*
 * @SPI_SSM
 * Software slave management (SSM bit of SPI_CR1)
 * 1 = NSS managed by software (SSI bit), 0 = NSS managed by the hardware pin
 */
#define SPI_SSM_DI                      0
#define SPI_SSM_EN                      1

/*
 * SPI related status flags definition
 */
#define SPI_RXNE_FLAG       (1U << SPI_SR_RXNE)
#define SPI_TXE_FLAG        (1U << SPI_SR_TXE)
#define SPI_CHSIDE_FLAG     (1U << SPI_SR_CHSIDE)
#define SPI_UDR_FLAG        (1U << SPI_SR_UDR)
#define SPI_CRCERR_FLAG     (1U << SPI_SR_CRCERR)
#define SPI_MODF_FLAG       (1U << SPI_SR_MODF)
#define SPI_OVR_FLAG        (1U << SPI_SR_OVR)
#define SPI_BUSY_FLAG       (1U << SPI_SR_BSY)
#define SPI_FRE_FLAG        (1U << SPI_SR_FRE)



/*******************************************************************************************
 *                     			APIs supported by this driver
 *******************************************************************************************/



/*
 * peripheral clock setup
 */
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx , uint8_t EnorDi);

/*
 * Init and DeInit
 */
void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_RegDef_t *pSPIx);

/*
 * Data Send and Receive
 */
void SPI_SendData(SPI_RegDef_t *pSPIx , uint8_t *pTxBuffer , uint32_t Len);
void SPI_ReceiveData(SPI_RegDef_t *pSPIx , uint8_t *pTxBuffer , uint32_t Len);


/*
 * IRQ Configuration and Handling
 */
void SPI_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi );
void SPI_IRQPriorityConfig(uint8_t IRQNumber, uint8_t IRQPriority);
void SPI_IRQHandling(SPI_Handle_t *pHandle);


#endif /* INC_STM32F446XX_SPI_DRIVER_H_ */
