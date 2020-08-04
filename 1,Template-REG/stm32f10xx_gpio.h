#ifndef __STM32F10XX_GPIO_H
#define __STM32F10XX_GPIO_H

#include "stm32f10xx.h"

/*typedef unsigned short uint16_t;
typedef unsigned short uint32_t;
typedef struct
{
	uint32_t CRL;
	uint32_t CRH;
	uint32_t IDR;
	uint32_t ODR;
	uint32_t BSRR;
	uint32_t BRR;
	uint32_t LCKR;
}GPIO_Reg;*/

#define GPIO_Pin_0 		((uint16_t)0x0001)		//(0x0000 0001)b
#define GPIO_Pin_1 		((uint16_t)0x0002)		//(0x0000 0010)b
#define GPIO_Pin_2 		((uint16_t)0x0004)	  //(0x0000 0100)b
#define GPIO_Pin_3 		((uint16_t)0x0008)		//
#define GPIO_Pin_4 		((uint16_t)0x0010)		//
#define GPIO_Pin_5 		((uint16_t)0x0020)		//
#define GPIO_Pin_6 		((uint16_t)0x0040)  	//
#define GPIO_Pin_7 		((uint16_t)0x0080)		//
#define GPIO_Pin_8 		((uint16_t)0x0100)    //
#define GPIO_Pin_9 		((uint16_t)0x0200)		//
#define GPIO_Pin_10	  ((uint16_t)0x0400)		//	
#define GPIO_Pin_11 	((uint16_t)0x0800)		//
#define GPIO_Pin_12   ((uint16_t)0x1000)		//
#define GPIO_Pin_13 	((uint16_t)0x2000)		//
#define GPIO_Pin_14 	((uint16_t)0x4000)		//
#define GPIO_Pin_15 	((uint16_t)0x8000)		//
#define GPIO_Pin_ALL 	((uint16_t)0xFFFF)		//

void set_BSRR(GPIO_Reg *GPIOx,uint16_t GPIO_Pin);
void set_BRR(GPIO_Reg *GPIOx,uint16_t GPIO_Pin);
#endif /* __STM32F10XX_GPIO_H */

