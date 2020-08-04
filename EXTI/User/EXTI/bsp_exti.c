#include "bsp_exti.h"

void EXTI_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = KEY1_PreemptionPriority;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = KEY1_SubPriority;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}

void EXTI_Key_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct;
	
	//配置中断优先级
	EXTI_NVIC_Config();
	
	//初始化GPIO
	RCC_APB2PeriphClockCmd(KEY1_INT_GPIO_CLK, ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = KEY1_INT_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(KEY1_INT_GPIO_PORT, &GPIO_InitStruct);
	
	//初始化EXTI
	RCC_APB2PeriphClockCmd(KEY1_INT_EXTI0_CLK, ENABLE);
	GPIO_EXTILineConfig(KEY1_GPIO_PortSourceGPIOx, KEY1_GPIO_PinSourcex);//输入线
	
	EXTI_InitStruct.EXTI_Line = KEY1_INT_EXTI0_Line;
	EXTI_InitStruct.EXTI_Mode = KEY1_INT_EXTI0_Mode;
	EXTI_InitStruct.EXTI_Trigger = KEY1_INT_EXTI0_Trigger;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStruct);
}

