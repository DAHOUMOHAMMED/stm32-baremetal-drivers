#include <stdint.h>

#define ADC_BASE_ADD         0x40012000UL
#define RCC_BASE_ADD    	 0x40023800UL

#define ADC_CR1_Reg_Offset   0x04UL
#define RCC_APB2_ENR_Offset   0x44UL

#define ADC_CR1_Reg_ADDR     (ADC_BASE_ADD + ADC_CR1_Reg_Offset)
#define RCC_APB2_Clock_Enb_ADDR     (RCC_BASE_ADD + RCC_APB2_ENR_Offset)


int main(void)
{

	uint32_t *pRccAPB2Reg = (uint32_t*) RCC_APB2_Clock_Enb_ADDR;
	uint32_t *pAdrCr1Reg = (uint32_t*) ADC_CR1_Reg_ADDR;

	*pRccAPB2Reg |= (1<<8);
	*pAdrCr1Reg |= (1<<8);

	for(;;);
}
