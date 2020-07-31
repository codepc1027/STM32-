#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_key.h"

#define GPIOB_ODR_Addr (GPIOB_BASE+0x0C)
#define PBout(n) *(unsigned int*)((GPIOB_ODR_Addr & 0xF0000000) + 0X02000000 + ((GPIOB_ODR_Addr &0x00FFFFFF)<<5) + (n<<2))
//位带操作公式	
#define GPIOB_IDR_Addr (GPIOB_BASE+0x08)
#define PAin(n) *(unsigned int*)((GPIOB_IDR_Addr & 0xF0000000) + 0X02000000 + ((GPIOB_IDR_Addr &0x00FFFFFF)<<5) + (n<<2))

void Delay(uint32_t count)
{
	for(; count!=0; count--);
}

int main(void)
{
	LED_GPIO_Config();
	KEY_GPIO_Config();
#if 0
	while(1)
	{
		PBout(0) = 1;
		//LED_G(OFF);
		//GPIO_SetBits(LED_G_GPIO_PORT, LED_G_GPIO_PIN);
		Delay(0xFFFFFF);
		//LED_G(ON);
		//GPIO_ResetBits(LED_G_GPIO_PORT, LED_G_GPIO_PIN);
		PBout(0) = 0;
	}
#else	
	while(1)
	{
		/*if( Key_Scan(KEY1_G_GPIO_PORT, KEY1_G_GPIO_PIN) == KEY_ON )
		 LED_G_TOGGLE;*/
		if( PAin(0) == KEY_ON )
		{
			while(PAin(0) == KEY_ON);
			LED_G_TOGGLE;
		}
	}	
#endif
}

