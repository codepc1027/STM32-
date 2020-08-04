#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_systick.h"

void Delay(uint32_t count)
{
	for(; count!=0; count--);
}

int main(void)
{
	LED_GPIO_Config();
	
	while(1)
	{
		LED_G(OFF);
		//GPIO_SetBits(LED_G_GPIO_PORT, LED_G_GPIO_PIN);
		SysTick_Delay_ms(1000);
		LED_G(ON);
		//GPIO_ResetBits(LED_G_GPIO_PORT, LED_G_GPIO_PIN);
	}
}

