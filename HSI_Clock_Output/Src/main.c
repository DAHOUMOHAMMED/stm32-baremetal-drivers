#include <stdint.h>

#define RCC_BASE_ADD 			0x40023800UL
#define GPOIA_BASE_ADDR 		0x40020000UL
#define RCC_CFGR_reg_offset 	0x08
#define RCC_CFGR_reg_ADDR	    (RCC_BASE_ADD + RCC_CFGR_reg_offset)




int main(void)
{
	uint32_t *pRccCfrgReg = (uint32_t *) RCC_CFGR_reg_ADDR;

	//1Configure the RCC_CFGR MCO1 bit to select HSI as clock source
	*pRccCfrgReg &= ~(3<<21);

	//2Configure PA8 to AF0 mode to behave as mco1 signal

	//a enable the peripheral clock for GPIOA peripheral

	uint32_t *pRccAhb1Enb = (uint32_t *) (RCC_BASE_ADD + 0x30);
	*pRccAhb1Enb |= (1<<0);

	//b configure the mode of GPIO pin 8 as alternate function mode

	uint32_t *pGPIOAModeReg = (uint32_t *) (GPOIA_BASE_ADDR + 0x00);
	*pGPIOAModeReg &= ~(3<<16); //clear
	*pGPIOAModeReg |= (1<<17); //set

	//c configure the alternation function register to set the 0  for PA8

	uint32_t *pGPIOAAlFunHighReg = (uint32_t *) (GPOIA_BASE_ADDR + 0x24);
	*pGPIOAAlFunHighReg &= ~(0x0f<<0);

    /* Loop forever */
	for(;;);
}
