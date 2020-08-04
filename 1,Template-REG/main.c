#include "stm32f10xx.h"
#include "stm32f10xx_gpio.h"

int main(void)
{
#if 0
	//存储器映射，直接访问内存地址
	//打开APB2总线的时钟
	*(unsigned int *)0x40021018 |= (1 << 3);
	//配置IO口为输出模式
	*(unsigned int *)0x40010C00 &= ~(0x0f << (4*0));
	*(unsigned int *)0x40010C00 |= (1 << (4*0));
	//控制ODR寄存器PB0输出0
	*(unsigned int *)0x40010C00 &= ~(1 << 0);
	
#elif0
	//寄存器映射，内存地址被封装在了头文件中
	//打开APB2总线的时钟
	RCC_APB2ENR |= (1 << 3);
	//配置IO口为输出模式
	GPIOB_CRL &= ~(0x0f << (4*0));
	GPIOB_CRL |= (1 << (4*0));
	//控制ODR寄存器PB0输出0
	GPIOB_ODR &= ~(1 << 0);
	
#elif0
	/*寄存器映射要操作的寄存器太多了，继续封装成结构体，利用结构体的存储连续性。
	原来的IO口基址是32位的，只能访问4字节地址把要操作的IO口基址强制转换成
	结构体类型，即可访问整个结构体成员，即所有寄存器*/
	//打开APB2总线的时钟
	RCC->APB2ENR |= (1 << 3);
	//配置IO口为输出模式
	GPIOB->CRL &= ~(0x0f << (4*0));
	GPIOB->CRL |= (1 << (4*0));
	//控制ODR寄存器PB0输出0
	GPIOB->ODR &= ~(1 << 0);
	
#elif1
	//为了程序的可读性，把具体的操作继续封装成函数，或者调用标准库
	//打开APB2总线的时钟
	RCC->APB2ENR |= (1 << 3);
	//配置IO口为输出模式
	GPIOB->CRL &= ~(0x0f << (4*0));//清0
	GPIOB->CRL |= (1 << (4*0));
	//控制BSRR寄存器PB0置位1，即设置PB0为1，关LED
	set_BSRR(GPIOB,GPIO_Pin_0);
	//控制BRR寄存器PB0置位1，即设置PB0为0，开LED
	set_BRR(GPIOB,GPIO_Pin_0);
	
#endif

	return 0;
}

void SystemInit(void)
{
	//函数体为空，目的为了骗过编译器不报错
}
