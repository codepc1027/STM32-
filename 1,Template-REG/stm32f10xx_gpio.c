#include "stm32f10xx_gpio.h"

void set_BSRR(GPIO_Reg *GPIOx,uint16_t GPIO_Pin)
{
	GPIOx->BSRR |= GPIO_Pin;
}

void set_BRR(GPIO_Reg *GPIOx,uint16_t GPIO_Pin)
{
	GPIOx->BRR |= GPIO_Pin;
}

