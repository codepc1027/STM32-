#include "bsp_i2c.h"

uint32_t I2CTimeout;

void I2C_EEP_Config(void)
{
	I2C_InitTypeDef   EEP_I2C_InitStruct;
	GPIO_InitTypeDef EEP_GPIO_InitStruct;
	
	//GPIO��ʼ��,PB6,PB7
	//��GPIOʱ��
	EEPROM_I2C_GPIO_APBxClkCmd(EEPROM_I2C_SDA_GPIO_CLK|EEPROM_I2C_SCL_GPIO_CLK, ENABLE);
	//��ʼ��GPIO�ṹ���Ա
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
	
	//��ʼ��EEPROM_I2C
	//��I2Cʱ��
	EEPROM_I2C_APBxClkCmd(EEPROM_I2C_CLK, ENABLE);
	//��ʼ��EEPROM_I2C�ṹ���Ա
	EEP_I2C_InitStruct.I2C_ClockSpeed = EEPROM_I2_BAUDRATE;
	EEP_I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	EEP_I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
	EEP_I2C_InitStruct.I2C_OwnAddress1 = STM32_I2C_OWN_ADDR;
	EEP_I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	EEP_I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	
	I2C_Init(EEPROM_I2C, &EEP_I2C_InitStruct);
}

//����һ���ֽ�
void I2C_EEP_ByteWrite(uint8_t addr,uint8_t data)
{
	//����START�ź�
	I2C_GenerateSTART(EEPROM_I2C, ENABLE);
	//���EV5
	I2CTimeout = I2CT_LONG_TIMEOUT;
	while( !I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT) )
	{
		if( (I2CTimeout--) == 0 )
		{
			printf("I2C_EEP_ByteWrite: waitEV5 error!");
			break;
		}
	}
	//����EEPROM��ַ
	I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDR, I2C_Direction_Transmitter);
	//���EV6
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
	//���EV8
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
	//���EV8_2
	I2CTimeout = I2CT_LONG_TIMEOUT;
	while( !I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) )
	{
		if( (I2CTimeout--) == 0 )
		{
			printf("I2C_EEP_ByteWrite: waitEV8_2 error!");
			break;
		}
	}
	//STOP�ź�
	I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
}

//���Ͷ���ֽڣ�ҳд��
void I2C_EEP_PageWrite(uint8_t addr,uint8_t *data, uint8_t numByteToWrite)
{
	//����START�ź�
	I2C_GenerateSTART(EEPROM_I2C, ENABLE);
	//���EV5
	I2CTimeout = I2CT_LONG_TIMEOUT;
	while( !I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT) )
	{
		if( (I2CTimeout--) == 0 )
		{
			printf("I2C_EEP_ByteWrite: waitEV5 error!");
			break;
		}
	}
	//����EEPROM��ַ
	I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDR, I2C_Direction_Transmitter);
	//���EV6
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
	//���EV8
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
		//���EV8_1
		while( !I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTED ) )
		{
			if( (I2CTimeout--) == 0 )
			{
				printf("I2C_EEP_ByteWrite: waitEV8_1 error!");
				break;
			}
		}
	}
	//STOP�ź�
	I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
}

//��ȡ
void I2C_EEP_Read(uint8_t addr,uint8_t *data,uint8_t numByteToRead)
{
	//�ж������Ƿ�æµ
	I2CTimeout = I2CT_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_BUSY))
  {
		if( (I2CTimeout--) == 0 )
		{
			printf("I2C_EEP_ByteWrite: waitEV5 error!");
			break;
		}
   }
	
	//����START�ź�
	I2C_GenerateSTART(EEPROM_I2C, ENABLE);
	//���EV5
	I2CTimeout = I2CT_LONG_TIMEOUT;
	while( !I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT) )
	{
		if( (I2CTimeout--) == 0 )
		{
			printf("I2C_EEP_ByteWrite: waitEV5 error!");
			break;
		}
	}
	
	//����EEPROM��ַ
	I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDR, I2C_Direction_Transmitter);
	//���EV6
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
	//���EV8
	I2CTimeout = I2CT_LONG_TIMEOUT;
	while( !I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING ) )
	{
		if( (I2CTimeout--) == 0 )
		{
			printf("I2C_EEP_ByteWrite: waitEV8 error!");
			break;
		}
	}
	
	//�ٴη���START�ź�
	I2C_GenerateSTART(EEPROM_I2C, ENABLE);
	//���EV5
	I2CTimeout = I2CT_LONG_TIMEOUT;
	while( !I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_MODE_SELECT) )
	{
		if( (I2CTimeout--) == 0 )
		{
			printf("I2C_EEP_ByteWrite: waitEV5 error!");
			break;
		}
	}
	
	//����EEPROM��ַ
	I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDR, I2C_Direction_Transmitter);
	//���EV6
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
		if(numByteToRead == 1)//���Ϊ���һ���ֽ�
		{		
			I2C_AcknowledgeConfig (EEPROM_I2C,DISABLE);//����NA��Ӧ���ź�
		}		
		//�����ݼĴ������µ���Ч����	
		*data = I2C_ReceiveData(EEPROM_I2C);
		data++;
		numByteToRead--;
		//EV7�¼�����⵽	
		while( !I2C_CheckEvent(EEPROM_I2C, I2C_EVENT_MASTER_BYTE_RECEIVED ) )
		{
			if( (I2CTimeout--) == 0 )
			{
				printf("I2C_EEP_ByteWrite: waitEV7 error!");
				break;
			}
		}
	}
	
	//STOP�ź�
	I2C_GenerateSTOP(EEPROM_I2C, ENABLE);
}

/**
  * @brief   ���������е�����д��I2C EEPROM��
  * @param   
  *		@arg pBuffer:������ָ��
  *		@arg WriteAddr:д��ַ
  *   @arg NumByteToWrite:д���ֽ���
  * @retval  ��
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

//�ȴ�EEP_I2C���ݴ�����ɺ���
void WaitEEPdata(void)
{
	I2CTimeout = I2CT_LONG_TIMEOUT;
	do
	{
		I2CTimeout--;
		//����START�ź�
		I2C_GenerateSTART(EEPROM_I2C, ENABLE);
		//�ȴ����SBλ��EV5
		I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_SB);
		//����EEPROM address
		I2C_Send7bitAddress(EEPROM_I2C, EEPROM_ADDR, I2C_Direction_Transmitter);
		//�ȴ����ADDRλ��EV6
	}while( !(I2C_GetFlagStatus(EEPROM_I2C, I2C_FLAG_ADDR)==SET) && (I2CTimeout>0) );
	
	//���Ӧ��ʧ�ܱ�־λ
	I2C_ClearFlag(EEPROM_I2C, I2C_FLAG_AF);
	//STOP�ź�
	I2C_GenerateSTOP(EEPROM_I2C, ENABLE); 
}

