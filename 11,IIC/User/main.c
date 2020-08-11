#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_i2c.h"
#include <string.h>

#define  EEP_Firstpage      0x00
uint8_t I2c_Buf_Write[256];
uint8_t I2c_Buf_Read[256];
uint8_t I2C_Test(void);

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{ 

  /* 串口初始化 */
	USART_Config();
	
	printf("\r\n 这是一个I2C外设(AT24C02)读写测试例程 \r\n");

	/* I2C 外设初(AT24C02)始化 */
	I2C_EEP_Init();
  
  while (1)
  {      
  }
}

/**
  * @brief  I2C(AT24C02)读写测试
  * @param  无
  * @retval 正常返回1，异常返回0
  */
uint8_t I2C_Test(void)
{
	uint16_t i;

	printf("写入的数据\n\r");
    
	for ( i=0; i<=255; i++ ) //填充缓冲
  {   
    I2c_Buf_Write[i] = i;

    printf("0x%02X ", I2c_Buf_Write[i]);
    if(i%16 == 15)    
        printf("\n\r");    
   }

  //将I2c_Buf_Write中顺序递增的数据写入EERPOM中 
	I2C_EEP_BufferWrite( I2c_Buf_Write, EEP_Firstpage, 256);

  //将EEPROM读出数据顺序保持到I2c_Buf_Read中
	I2C_EEP_BufferRead(I2c_Buf_Read, EEP_Firstpage, 256); 
   
  //将I2c_Buf_Read中的数据通过串口打印
	for (i=0; i<256; i++)
	{	
		if(I2c_Buf_Read[i] != I2c_Buf_Write[i])
		{
			return 0;
		}
    printf("0x%02X ", I2c_Buf_Read[i]);
    if(i%16 == 15)    
        printf("\n\r");
    
	}
  
  return 1;
}