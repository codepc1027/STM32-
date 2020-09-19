/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   SPI FLASH测试，测试信息通过USART1打印在电脑的超级终端
  ******************************************************************************
  * @attention
  *
  * 实验平台:秉火 F103-霸道 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f10x.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_usart.h"
#include "./flash/bsp_spi_flash.h"
#include <string.h>

int main(void)
{ 
  LED_GPIO_Config();
  
  LED_BLUE;
  /* 串口初始化 */
	USART_Config();
	
	printf("\r\n 这是一个SPI-FLASH读写测试例程 \r\n");
	

	
  while (1)
  {      
  }
}

/*********************************************END OF FILE**********************/
