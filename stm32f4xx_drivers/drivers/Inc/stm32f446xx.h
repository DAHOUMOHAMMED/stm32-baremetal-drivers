/*
 * stm32f446xx.h
 *
 *  Created on: Oct 31, 2026
 *      Author: Mohammed Dahou
 */

#ifndef INC_STM32F446XX_H_
#define INC_STM32F446XX_H_

#include <stdint.h>

#define __vo volatile

/*
 * base addresses of Flash and SRAM memories
 */


#define FLASH_BASEADDR  					0x08000000U
#define SRAM1_BASEADDR						0x20000000U
#define SRAM2_BASEADDR						0x2001C000U
#define ROM									0x1FFF0000U
#define SRAM 								SRAM1_BASEADDR


/*
 * AHBx and APBx bus Peripheral base addresses
 */

#define PERIPH_BASEADDR  					    0x40000000U    // TIM2
#define APB1PERIFH_BASEADDR						PERIPH_BASEADDR
#define APB2PERIFH_BASEADDR						0x40010000U
#define AHB1PERIFH_BASEADDR						0x40020000U
#define AHB2PERIFH_BASEADDR						0x50000000U

/* ---------- Peripherals AHB1 ---------- */
#define GPIOA_BASEADDR          (AHB1PERIFH_BASEADDR + 0x0000U)
#define GPIOB_BASEADDR          (AHB1PERIFH_BASEADDR + 0x0400U)
#define GPIOC_BASEADDR          (AHB1PERIFH_BASEADDR + 0x0800U)
#define GPIOD_BASEADDR          (AHB1PERIFH_BASEADDR + 0x0C00U)
#define GPIOE_BASEADDR          (AHB1PERIFH_BASEADDR + 0x1000U)
#define GPIOF_BASEADDR          (AHB1PERIFH_BASEADDR + 0x1400U)
#define GPIOG_BASEADDR          (AHB1PERIFH_BASEADDR + 0x1800U)
#define GPIOH_BASEADDR          (AHB1PERIFH_BASEADDR + 0x1C00U)
#define RCC_BASEADDR            (AHB1PERIFH_BASEADDR + 0x3800U)


/* ---------- Peripherals APB1 ---------- */
#define I2C1_BASEADDR           (APB1PERIFH_BASEADDR + 0x5400U)
#define I2C2_BASEADDR           (APB1PERIFH_BASEADDR + 0x5800U)
#define I2C3_BASEADDR           (APB1PERIFH_BASEADDR + 0x5C00U)
#define SPI2_BASEADDR           (APB1PERIFH_BASEADDR + 0x3800U)
#define SPI3_BASEADDR           (APB1PERIFH_BASEADDR + 0x3C00U)
#define USART2_BASEADDR         (APB1PERIFH_BASEADDR + 0x4400U)
#define USART3_BASEADDR         (APB1PERIFH_BASEADDR + 0x4800U)
#define UART4_BASEADDR          (APB1PERIFH_BASEADDR + 0x4C00U)
#define UART5_BASEADDR          (APB1PERIFH_BASEADDR + 0x5000U)


/* ---------- Peripherals APB2 ---------- */
#define USART1_BASEADDR         (APB2PERIFH_BASEADDR + 0x1000U)
#define USART6_BASEADDR         (APB2PERIFH_BASEADDR + 0x1400U)
#define SPI1_BASEADDR           (APB2PERIFH_BASEADDR + 0x3000U)
#define SPI4_BASEADDR           (APB2PERIFH_BASEADDR + 0x3400U)
#define SYSCFG_BASEADDR         (APB2PERIFH_BASEADDR + 0x3800U)
#define EXTI_BASEADDR           (APB2PERIFH_BASEADDR + 0x3C00U)




/**********************peripheral register definition structures********************/

typedef struct
{
	__vo uint32_t MODER;            //GPIO port mode register             	  :   address offset 0x00
	__vo uint32_t OTYPER;		   //GPIO port output type register       	  :   address offset 0x40
	__vo uint32_t OSPEEDER;		   //GPIO port output speed register      	  :   address offset 0x80
	__vo uint32_t PUPDR;			//GPIO port pull-up/pull-down register 	  :   address offset 0x0c
	__vo uint32_t IDR;			   //GPIO port input data register		  	  :   address offset 0x10
	__vo uint32_t ODR;			   //GPIO port output data register			  :   address offset 0x14
	__vo uint32_t BSRR;			   //GPIO port bit set/reset register	      :   address offset 0x18
	__vo uint32_t LCKR;			   //GPIO port configuration lock register	  :   address offset 0x1C
	__vo uint32_t AFR[2];		   //address offset (AFRL => AFR[0])  0x20  and (AFRH => AFR[1])   0x24

}GPIO_RegDef_t;

typedef struct
{
	__vo uint32_t CR;            /* 0x00 */
	__vo uint32_t PLLCFGR;       /* 0x04 */
	__vo uint32_t CFGR;          /* 0x08 */
	__vo uint32_t CIR;           /* 0x0C */
	__vo uint32_t AHB1RSTR;      /* 0x10 */
	__vo uint32_t AHB2RSTR;      /* 0x14 */
	__vo uint32_t AHB3RSTR;      /* 0x18 */
	uint32_t      RESERVED0;     /* 0x1C */
	__vo uint32_t APB1RSTR;      /* 0x20 */
	__vo uint32_t APB2RSTR;      /* 0x24 */
	uint32_t      RESERVED1[2];  /* 0x28, 0x2C */
	__vo uint32_t AHB1ENR;       /* 0x30 */
	__vo uint32_t AHB2ENR;       /* 0x34 */
	__vo uint32_t AHB3ENR;       /* 0x38 */
	uint32_t      RESERVED2;     /* 0x3C */
	__vo uint32_t APB1ENR;       /* 0x40 */
	__vo uint32_t APB2ENR;       /* 0x44 */
	uint32_t      RESERVED3[2];  /* 0x48, 0x4C */
	__vo uint32_t AHB1LPENR;     /* 0x50 */
	__vo uint32_t AHB2LPENR;     /* 0x54 */
	__vo uint32_t AHB3LPENR;     /* 0x58 */
	uint32_t      RESERVED4;     /* 0x5C */
	__vo uint32_t APB1LPENR;     /* 0x60 */
	__vo uint32_t APB2LPENR;     /* 0x64 */
	uint32_t      RESERVED5[2];  /* 0x68, 0x6C */
	__vo uint32_t BDCR;          /* 0x70 */
	__vo uint32_t CSR;           /* 0x74 */
	uint32_t      RESERVED6[2];  /* 0x78, 0x7C */
	__vo uint32_t SSCGR;         /* 0x80 */
	__vo uint32_t PLLI2SCFGR;    /* 0x84 */
	__vo uint32_t PLLSAICFGR;    /* 0x88 */
	__vo uint32_t DCKCFGR;       /* 0x8C */
	__vo uint32_t CKGATENR;      /* 0x90 */
	__vo uint32_t DCKCFGR2;      /* 0x94 */
} RCC_RegDef_t;





#define GPIOA   ((GPIO_RegDef_t*)GPIOA_BASEADDR)
#define GPIOB   ((GPIO_RegDef_t*)GPIOB_BASEADDR)
#define GPIOC   ((GPIO_RegDef_t*)GPIOC_BASEADDR)
#define GPIOD   ((GPIO_RegDef_t*)GPIOD_BASEADDR)
#define GPIOE   ((GPIO_RegDef_t*)GPIOE_BASEADDR)
#define GPIOF   ((GPIO_RegDef_t*)GPIOF_BASEADDR)
#define GPIOG   ((GPIO_RegDef_t*)GPIOG_BASEADDR)
#define GPIOH   ((GPIO_RegDef_t*)GPIOH_BASEADDR)

#define RCC     ((RCC_RegDef_t*)RCC_BASEADDR)

/*
 * Reset macros for GPIOx peripherals
 */

#define GPIOA_REG_RESET()   do{ (RCC->AHB1RSTR |= (1 << 0)); (RCC->AHB1RSTR &= ~(1 << 0)); }while(0)
#define GPIOB_REG_RESET()   do{ (RCC->AHB1RSTR |= (1 << 1)); (RCC->AHB1RSTR &= ~(1 << 1)); }while(0)
#define GPIOC_REG_RESET()   do{ (RCC->AHB1RSTR |= (1 << 2)); (RCC->AHB1RSTR &= ~(1 << 2)); }while(0)
#define GPIOD_REG_RESET()   do{ (RCC->AHB1RSTR |= (1 << 3)); (RCC->AHB1RSTR &= ~(1 << 3)); }while(0)
#define GPIOE_REG_RESET()   do{ (RCC->AHB1RSTR |= (1 << 4)); (RCC->AHB1RSTR &= ~(1 << 4)); }while(0)
#define GPIOF_REG_RESET()   do{ (RCC->AHB1RSTR |= (1 << 5)); (RCC->AHB1RSTR &= ~(1 << 5)); }while(0)
#define GPIOG_REG_RESET()   do{ (RCC->AHB1RSTR |= (1 << 6)); (RCC->AHB1RSTR &= ~(1 << 6)); }while(0)
#define GPIOH_REG_RESET()   do{ (RCC->AHB1RSTR |= (1 << 7)); (RCC->AHB1RSTR &= ~(1 << 7)); }while(0)

/*
 * clock enable macros for GPIOx peripherals
 */

#define GPIOA_PCLK_EN()    (RCC->AHB1ENR |= (1 << 0))
#define GPIOB_PCLK_EN()    (RCC->AHB1ENR |= (1 << 1))
#define GPIOC_PCLK_EN()    (RCC->AHB1ENR |= (1 << 2))
#define GPIOD_PCLK_EN()    (RCC->AHB1ENR |= (1 << 3))
#define GPIOE_PCLK_EN()    (RCC->AHB1ENR |= (1 << 4))
#define GPIOF_PCLK_EN()    (RCC->AHB1ENR |= (1 << 5))
#define GPIOG_PCLK_EN()    (RCC->AHB1ENR |= (1 << 6))
#define GPIOH_PCLK_EN()    (RCC->AHB1ENR |= (1 << 7))


/*
 * clock enable macros for I2Cx peripherals
 */

#define I2C1_PCLK_EN()	   (RCC->APB1ENR |= (1 << 21))
#define I2C2_PCLK_EN()	   (RCC->APB1ENR |= (1 << 22))
#define I2C3_PCLK_EN()	   (RCC->APB1ENR |= (1 << 23))


/*
 * clock enable macros for SPIx peripherals
 */

#define SPI1_PCLK_EN()	   (RCC->APB2ENR |= (1 << 12))
#define SPI2_PCLK_EN()	   (RCC->APB1ENR |= (1 << 14))
#define SPI3_PCLK_EN()	   (RCC->APB1ENR |= (1 << 15))
#define SPI4_PCLK_EN()	   (RCC->APB2ENR |= (1 << 13))

/*
 * clock enable macros for USARTx / UARTx peripherals
 */

#define USART1_PCLK_EN()   (RCC->APB2ENR |= (1 << 4))
#define USART2_PCLK_EN()   (RCC->APB1ENR |= (1 << 17))
#define USART3_PCLK_EN()   (RCC->APB1ENR |= (1 << 18))
#define UART4_PCLK_EN()    (RCC->APB1ENR |= (1 << 19))
#define UART5_PCLK_EN()    (RCC->APB1ENR |= (1 << 20))
#define USART6_PCLK_EN()   (RCC->APB2ENR |= (1 << 5))


/*
 * clock enable macro for SYSCFG peripheral
 */

#define SYSCFG_PCLK_EN()   (RCC->APB2ENR |= (1 << 14))

/*
 * clock disable macros for GPIOx peripherals
 */

#define GPIOA_PCLK_DI()    (RCC->AHB1ENR &= ~(1 << 0))
#define GPIOB_PCLK_DI()    (RCC->AHB1ENR &= ~(1 << 1))
#define GPIOC_PCLK_DI()    (RCC->AHB1ENR &= ~(1 << 2))
#define GPIOD_PCLK_DI()    (RCC->AHB1ENR &= ~(1 << 3))
#define GPIOE_PCLK_DI()    (RCC->AHB1ENR &= ~(1 << 4))
#define GPIOF_PCLK_DI()    (RCC->AHB1ENR &= ~(1 << 5))
#define GPIOG_PCLK_DI()    (RCC->AHB1ENR &= ~(1 << 6))
#define GPIOH_PCLK_DI()    (RCC->AHB1ENR &= ~(1 << 7))


/*
 * clock disable macros for I2Cx peripherals
 */

#define I2C1_PCLK_DI()     (RCC->APB1ENR &= ~(1 << 21))
#define I2C2_PCLK_DI()     (RCC->APB1ENR &= ~(1 << 22))
#define I2C3_PCLK_DI()     (RCC->APB1ENR &= ~(1 << 23))


/*
 * clock disable macros for SPIx peripherals
 */

#define SPI1_PCLK_DI()     (RCC->APB2ENR &= ~(1 << 12))
#define SPI2_PCLK_DI()     (RCC->APB1ENR &= ~(1 << 14))
#define SPI3_PCLK_DI()     (RCC->APB1ENR &= ~(1 << 15))
#define SPI4_PCLK_DI()     (RCC->APB2ENR &= ~(1 << 13))


/*
 * clock disable macros for USARTx / UARTx peripherals
 */

#define USART1_PCLK_DI()   (RCC->APB2ENR &= ~(1 << 4))
#define USART2_PCLK_DI()   (RCC->APB1ENR &= ~(1 << 17))
#define USART3_PCLK_DI()   (RCC->APB1ENR &= ~(1 << 18))
#define UART4_PCLK_DI()    (RCC->APB1ENR &= ~(1 << 19))
#define UART5_PCLK_DI()    (RCC->APB1ENR &= ~(1 << 20))
#define USART6_PCLK_DI()   (RCC->APB2ENR &= ~(1 << 5))


/*
 * clock disable macro for SYSCFG peripheral
 */

#define SYSCFG_PCLK_DI()   (RCC->APB2ENR &= ~(1 << 14))



//some generic macros

#define ENABALE				    1
#define DISABLE 				0
#define SET 					ENABALE
#define RESET 					DISABLE
#define GPIO_PIN_SET  			SET
#define GPIO_PIN_RESET  		RESET




#endif /* INC_STM32F446XX_H_ */
