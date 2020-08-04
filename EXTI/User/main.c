#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_exti.h"

void Delay(uint32_t count)
{
	for(; count!=0; count--);
}

int main(void)
{
	LED_GPIO_Config();
	EXTI_Key_Config();
	
	while(1)
	{
		/*
		LED_G(OFF);
		//GPIO_SetBits(LED_G_GPIO_PORT, LED_G_GPIO_PIN);
		Delay(0xFFFFFF);
		LED_G(ON);
		//GPIO_ResetBits(LED_G_GPIO_PORT, LED_G_GPIO_PIN);
		*/
	}
}

