/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   SPI FLASH���ԣ�������Ϣͨ��USART1��ӡ�ڵ��Եĳ����ն�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:���� F103-�Ե� STM32 ������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :http://firestm32.taobao.com
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
  /* ���ڳ�ʼ�� */
	USART_Config();
	
	printf("\r\n ����һ��SPI-FLASH��д�������� \r\n");
	

	
  while (1)
  {      
  }
}

/*********************************************END OF FILE**********************/
