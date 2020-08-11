#include "bsp_i2c.h"

uint32_t I2CTimeout;

void I2C_EEP_Config(void)
{
	I2C_InitTypeDef   EEP_I2C_InitStruct;
	GPIO_InitTypeDef EEP_GPIO_InitStruct;
	
	//GPIO初始化,PB6,PB7
	//打开GPIO时钟
	EEPROM_I2C_GPIO_APBxClkCmd(EEPROM_I2C_SDA_GPIO_CLK|EEPROM_I2C_SCL_GPIO_CLK, ENABLE);
	//初始化GPIO结构体成员
	//PB6 SCL
	EEP_GPIO_InitStruct.GPIO_Pin = EEPROM_I2C_SCL_GPIO_PIN;
	EEP_GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
	EEP_GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(EEPROM_I2C_SCL_GPIO_PORT, &EEP_GPIO_InitStruct);
	
  //PB7 SDA
	EEP_GPIO_InitStruct.GPIO_Pin = EEPROM_I2C_SDA_GPIO_PIN;
	EEP_GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD;
	EEP_GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(EEPROM_I2C_SDA_GPIO_PORT, &EEP_GPIO_InitStruct);
	
	//初始化EEPROM_I2C
	//打开I2C时钟
	EEPROM_I2C_APBxClkCmd(EEPROM_I2C_CLK, ENABLE);
	//初始化EEPROM_I2C结构体成员
	EEP_I2C_InitStruct.I2C_ClockSpeed = EEPROM_I2_BAUDRATE;
	EEP_I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	EEP_I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	EEP_I2C_InitStruct.I2C_OwnAddress1 = STM32_I2C_OWN_ADDR;
	EEP_I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	EEP_I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	
	I2C_Init(EEPROM_I2C, &EEP_I2C_InitStruct);
}

//发送一个字节
void I2C_EEP_ByteWrite(uint8_t addr,uint8_t data)
{
	//发送START信号
	I2C_GenerateSTART(EEPROM_I2C, ENABLE);
	//检测EV5
	I2CTimeout = I2CT_LONG_TIMEOUT;
	while( !I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT) )
	{
		if( (I2CTimeout--) == 0 )
		{
			printf("I2C_EEP_ByteWrite: waitEV5 error!");
			break;
		}
	}
	//发送EEPROM地址
	I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDR, I2C_Direction_Transmitter);
	//检测EV6
	I2CTimeout = I2CT_LONG_TIMEOUT;
	while( !I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) )
	{
		if( (I2CTimeout--) == 0 )
		{
			printf("I2C_EEP_ByteWrite: waitEV6 error!");
			break;
		}
	}
	 /* Send the EEPROM's internal address to write to */
  I2C_SendData(EEPROM_I2C, addr);
	//检测EV8
	I2CTimeout = I2CT_LONG_TIMEOUT;
	while( !I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING ) )
	{
		if( (I2CTimeout--) == 0 )
		{
			printf("I2C_EEP_ByteWrite: waitEV8 error!");
			break;
		}
	}
	/* Send the byte to be written */
  I2C_SendData(EEPROM_I2C, data); 
	//检测EV8_2
	I2CTimeout = I2CT_LONG_TIMEOUT;
	while( !I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) )
	{
		if( (I2CTimeout--) == 0 )
		{
			printf("I2C_EEP_ByteWrite: waitEV8_2 error!");
			break;
		}
	}
	//STOP信号
	I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
}

//发送多个字节，页写入
void I2C_EEP_PageWrite(uint8_t addr,uint8_t *data, uint8_t numByteToWrite)
{
	//发送START信号
	I2C_GenerateSTART(EEPROM_I2C, ENABLE);
	//检测EV5
	I2CTimeout = I2CT_LONG_TIMEOUT;
	while( !I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT) )
	{
		if( (I2CTimeout--) == 0 )
		{
			printf("I2C_EEP_ByteWrite: waitEV5 error!");
			break;
		}
	}
	//发送EEPROM地址
	I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDR, I2C_Direction_Transmitter);
	//检测EV6
	I2CTimeout = I2CT_LONG_TIMEOUT;
	while( !I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) )
	{
		if( (I2CTimeout--) == 0 )
		{
			printf("I2C_EEP_ByteWrite: waitEV6 error!");
			break;
		}
	}
	 /* Send the EEPROM's internal address to write to */
  I2C_SendData(EEPROM_I2C, addr);
	//检测EV8
	I2CTimeout = I2CT_LONG_TIMEOUT;
	while( !I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING ) )
	{
		if( (I2CTimeout--) == 0 )
		{
			printf("I2C_EEP_ByteWrite: waitEV8 error!");
			break;
		}
	}
	/* Send the byte to be written */
	while(numByteToWrite)
	{
		I2C_SendData (EEPROM_I2C,*data);
		data++;
		numByteToWrite--;
		
		I2CTimeout = I2CT_LONG_TIMEOUT;
		//检测EV8_1
		while( !I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) )
		{
			if( (I2CTimeout--) == 0 )
			{
				printf("I2C_EEP_ByteWrite: waitEV8_1 error!");
				break;
			}
		}
	}
	//STOP信号
	I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
}

//读取
void I2C_EEP_Read(uint8_t addr,uint8_t *data,uint8_t numByteToRead)
{
	//判断总线是否忙碌
	I2CTimeout = I2CT_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_BUSY))
  {
		if( (I2CTimeout--) == 0 )
		{
			printf("I2C_EEP_ByteWrite: waitEV5 error!");
			break;
		}
   }
	
	//发送START信号
	I2C_GenerateSTART(EEPROM_I2C, ENABLE);
	//检测EV5
	I2CTimeout = I2CT_LONG_TIMEOUT;
	while( !I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT) )
	{
		if( (I2CTimeout--) == 0 )
		{
			printf("I2C_EEP_ByteWrite: waitEV5 error!");
			break;
		}
	}
	
	//发送EEPROM地址
	I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDR, I2C_Direction_Transmitter);
	//检测EV6
	I2CTimeout = I2CT_LONG_TIMEOUT;
	while( !I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) )
	{
		if( (I2CTimeout--) == 0 )
		{
			printf("I2C_EEP_ByteWrite: waitEV6 error!");
			break;
		}
	}
	
	 /* Send the EEPROM's internal address to write to */
  I2C_SendData(EEPROM_I2C, addr);
	//检测EV8
	I2CTimeout = I2CT_LONG_TIMEOUT;
	while( !I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING ) )
	{
		if( (I2CTimeout--) == 0 )
		{
			printf("I2C_EEP_ByteWrite: waitEV8 error!");
			break;
		}
	}
	
	//再次发送START信号
	I2C_GenerateSTART(EEPROM_I2C, ENABLE);
	//检测EV5
	I2CTimeout = I2CT_LONG_TIMEOUT;
	while( !I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT) )
	{
		if( (I2CTimeout--) == 0 )
		{
			printf("I2C_EEP_ByteWrite: waitEV5 error!");
			break;
		}
	}
	
	//发送EEPROM地址
	I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDR, I2C_Direction_Transmitter);
	//检测EV6
	I2CTimeout = I2CT_LONG_TIMEOUT;
	while( !I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED ) )
	{
		if( (I2CTimeout--) == 0 )
		{
			printf("I2C_EEP_ByteWrite: waitEV6 error!");
			break;
		}
	}
	//read
	while(numByteToRead)
	{
		if(numByteToRead == 1)//如果为最后一个字节
		{		
			I2C_AcknowledgeConfig (EEPROM_I2C,DISABLE);//产生NA非应答信号
		}		
		//即数据寄存器有新的有效数据	
		*data = I2C_ReceiveData(EEPROM_I2C);
		data++;
		numByteToRead--;
		//EV7事件被检测到	
		while( !I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED ) )
		{
			if( (I2CTimeout--) == 0 )
			{
				printf("I2C_EEP_ByteWrite: waitEV7 error!");
				break;
			}
		}
	}
	
	//STOP信号
	I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
}

/**
  * @brief   将缓冲区中的数据写到I2C EEPROM中
  * @param   
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址
  *   @arg NumByteToWrite:写的字节数
  * @retval  无
  */
void I2C_EEP_BufferWrite(u8 *pBuffer, u8 WriteAddr, u16 NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;

  Addr = WriteAddr % I2C_PageSize;
  count = I2C_PageSize - Addr;
  NumOfPage =  NumByteToWrite / I2C_PageSize;
  NumOfSingle = NumByteToWrite % I2C_PageSize;
 
  /* If WriteAddr is I2C_PageSize aligned  */
  if(Addr == 0) 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfPage == 0) 
    {
      I2C_EEP_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      WaitEEPdata();
    }
    /* If NumByteToWrite > I2C_PageSize */
    else  
    {
      while(NumOfPage--)
      {
        I2C_EEP_PageWrite(pBuffer, WriteAddr, I2C_PageSize); 
    	  WaitEEPdata();
        WriteAddr +=  I2C_PageSize;
        pBuffer += I2C_PageSize;
      }

      if(NumOfSingle!=0)
      {
        I2C_EEP_PageWrite(pBuffer, WriteAddr, NumOfSingle);
        WaitEEPdata();
      }
    }
  }
  /* If WriteAddr is not I2C_PageSize aligned  */
  else 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfPage== 0) 
    {
      I2C_EEP_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      WaitEEPdata();
    }
    /* If NumByteToWrite > I2C_PageSize */
    else
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / I2C_PageSize;
      NumOfSingle = NumByteToWrite % I2C_PageSize;	
      
      if(count != 0)
      {  
        I2C_EEP_PageWrite(pBuffer, WriteAddr, count);
        WaitEEPdata();
        WriteAddr += count;
        pBuffer += count;
      } 
      
      while(NumOfPage--)
      {
        I2C_EEP_PageWrite(pBuffer, WriteAddr, I2C_PageSize);
        WaitEEPdata();
        WriteAddr +=  I2C_PageSize;
        pBuffer += I2C_PageSize;  
      }
      if(NumOfSingle != 0)
      {
        I2C_EEP_PageWrite(pBuffer, WriteAddr, NumOfSingle); 
        WaitEEPdata();
      }
    }
  }  
}

//等待EEP_I2C数据传输完成函数
void WaitEEPdata(void)
{
	I2CTimeout = I2CT_LONG_TIMEOUT;
	do
	{
		I2CTimeout--;
		//发送START信号
		I2C_GenerateSTART(EEPROM_I2C, ENABLE);
		//等待检测SB位，EV5
		I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_SB);
		//发送EEPROM address
		I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDR, I2C_Direction_Transmitter);
		//等待检测ADDR位，EV6
	}while( !(I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_ADDR)==SET) && (I2CTimeout>0) );
	
	//清除应答失败标志位
	I2C_ClearFlag(EEPROM_I2C, I2C_FLAG_AF);
	//STOP信号
	I2C_GenerateSTOP(EEPROM_I2C, ENABLE); 
}

