#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include "stm32f10x.h"

#define KEY1_G_GPIO_PIN     GPIO_Pin_0 
#define KEY1_G_GPIO_PORT  	GPIOA
#define KEY1_G_GPIO_CLK     RCC_APB2Periph_GPIOA

#define KEY_ON      1
#define KEY_OFF     0

uint8_t Key_Scan(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void KEY_GPIO_Config(void);

#endif /* __BSP_KEY_H */

