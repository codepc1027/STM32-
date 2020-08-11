#ifndef __BSP_I2C_H
#define __BSP_I2C_H

#include "stm32f10x.h"
#include <stdio.h>

//EEPROM 设备地址（8位）
#define EEPROM_ADDR   0xA0

#define I2C_PageSize           8

/** 
* IIC EERPOM的引脚定义
*
*/
#define STM32_I2C_OWN_ADDR     0x5f 

	
// IIC
#define  EEPROM_I2C                   I2C1
#define  EEPROM_I2C_CLK               RCC_APB1Periph_I2C1
#define  EEPROM_I2C_APBxClkCmd        RCC_APB1PeriphClockCmd
#define  EEPROM_I2_BAUDRATE           400000

//IIC GPIO 引脚宏定义
#define  EEPROM_I2C_SCL_GPIO_CLK           (RCC_APB2Periph_GPIOB)
#define  EEPROM_I2C_SDA_GPIO_CLK           (RCC_APB2Periph_GPIOB)

#define  EEPROM_I2C_GPIO_APBxClkCmd    		 RCC_APB2PeriphClockCmd
    
#define  EEPROM_I2C_SCL_GPIO_PORT          GPIOB  
#define  EEPROM_I2C_SCL_GPIO_PIN           GPIO_Pin_6

#define  EEPROM_I2C_SDA_GPIO_PORT       	 GPIOB
#define  EEPROM_I2C_SDA_GPIO_PIN       	   GPIO_Pin_7
	
/*等待超时时间*/
#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define I2CT_LONG_TIMEOUT         ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))

void I2C_EEP_Config(void);
void I2C_EEP_ByteWrite(uint8_t addr,uint8_t data);
void I2C_EEP_PageWrite(uint8_t addr,uint8_t *data, uint8_t numByteToWrite);
void I2C_EEP_Read(uint8_t addr,uint8_t *data,uint8_t numByteToRead);
void I2C_EEP_BufferWrite(u8 *pBuffer, u8 WriteAddr, u16 NumByteToWrite);
void WaitEEPdata(void);

#endif /* __BSP_I2C_H */

