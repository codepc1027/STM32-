#ifndef __BSP_EXTI_H
#define __BSP_EXTI_H

#include "stm32f10x.h"
	
#define KEY1_INT_GPIO_PIN     	 GPIO_Pin_0 
#define KEY1_INT_GPIO_PORT 			 GPIOA
#define KEY1_INT_GPIO_CLK  			 RCC_APB2Periph_GPIOA

#define KEY1_PreemptionPriority  	 1
#define KEY1_SubPriority         	 1

#define KEY1_INT_EXTI0_CLK  					 RCC_APB2Periph_AFIO
#define KEY1_GPIO_PortSourceGPIOx  		 GPIO_PortSourceGPIOA
#define KEY1_GPIO_PinSourcex  		     GPIO_PinSource0

#define KEY1_INT_EXTI0_Line 		EXTI_Line0;
#define KEY1_INT_EXTI0_Mode 		EXTI_Mode_Interrupt;
#define KEY1_INT_EXTI0_Trigger  EXTI_Trigger_Rising;

void EXTI_NVIC_Config(void);
void EXTI_Key_Config(void);

#endif /* __BSP_EXTI_H */

