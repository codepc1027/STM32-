#include "bsp_spi_flash.h"
#include "bsp_usart.h"

static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;
static uint32_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);

static void SPI_GPIO_Config(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	FLASH_SPI_GPIOx_CLKFUN(RCC_APB2Periph_GPIOA, ENABLE);
	
	/* MISO MOSI SCK*/
  GPIO_InitStruct.GPIO_Pin = FLASH_SPI_SCK_PIN;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;	       
  GPIO_Init(FLASH_SPI_SCK_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = FLASH_SPI_MOSI_PIN;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;	       
  GPIO_Init(FLASH_SPI_MOSI_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = FLASH_SPI_MISO_PIN;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;	       
  GPIO_Init(FLASH_SPI_MISO_PORT, &GPIO_InitStruct);
	
	//初始化CS引脚，使用软件控制，所以直接设置成推挽输出	
	GPIO_InitStruct.GPIO_Pin = FLASH_SPI_CS_PIN;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;	       
  GPIO_Init(FLASH_SPI_CS_PORT, &GPIO_InitStruct);
	
	FLASH_SPI_CS_HIGH
}

static void SPI_Mode_Config(void)
{
	SPI_InitTypeDef  SPI_InitStruct;
	
	FLASH_SPI_CLKFUN(FLASH_SPI_CLK,ENABLE);
	
	//SPI 使用模式3
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStruct.SPI_CRCPolynomial = 0;//不使用CRC功能，数值随便写
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//双线全双工
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;	
	
	SPI_Init(FLASH_SPIx,&SPI_InitStruct);
	SPI_Cmd(FLASH_SPIx,ENABLE);//使能SPI
}

//初始化
void SPI_FLASH_Init(void)
{
	SPI_GPIO_Config();
	SPI_Mode_Config();
}

//发送并接收一个字节
uint8_t SPI_FLASH_Send_Byte(uint8_t data)
{
	SPITimeout = SPIT_FLAG_TIMEOUT;
	//检查并等待至TX缓冲区为空
	while(SPI_I2S_GetFlagStatus(FLASH_SPIx,SPI_I2S_FLAG_TXE) == RESET)
	{
		if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
	}
	
	//程序执行到此处，TX缓冲区已空
	SPI_I2S_SendData (FLASH_SPIx,data);
	
	
	SPITimeout = SPIT_FLAG_TIMEOUT;
	//检查并等待至RX缓冲区为非空
	while(SPI_I2S_GetFlagStatus(FLASH_SPIx,SPI_I2S_FLAG_RXNE) == RESET)
	{
		if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
	}
		
	//程序执行到此处，说明数据发送完毕，并接收到一字字节	
	return SPI_I2S_ReceiveData(FLASH_SPIx); 

}

//读取ID号
uint32_t SPI_Read_ID(void)
{
	uint32_t flash_id;
	
	//片选使能
	FLASH_SPI_CS_LOW;
	SPI_FLASH_Send_Byte(READ_JEDEC_ID);
	
	flash_id = SPI_FLASH_Send_Byte(DUMMY);

	flash_id <<= 8;
	
	flash_id |= SPI_FLASH_Send_Byte(DUMMY); 
	
	flash_id <<= 8;
	
	flash_id |= SPI_FLASH_Send_Byte(DUMMY); 
	
	FLASH_SPI_CS_HIGH;	
	
	return flash_id;
}

//write enable
//FLASH写入使能
void SPI_Write_Enable(void)
{
		//片选使能
	FLASH_SPI_CS_LOW;
	SPI_FLASH_Send_Byte(WRITE_ENABLE);	 
	FLASH_SPI_CS_HIGH;	
}
	
//等待FLASH内部时序操作完成（擦除）
void SPI_WaitForWriteEnd(void)
{
	uint8_t status_reg = 0;
	
	//片选使能
	FLASH_SPI_CS_LOW;
	
	SPI_FLASH_Send_Byte(READ_STATUS);
	
	do
	{	
	status_reg = SPI_FLASH_Send_Byte(DUMMY);
	}
	while((status_reg & 0x01) == 1);
	
	FLASH_SPI_CS_HIGH;	


}



//timeout fun
static  uint32_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
	/* Block communication and all processes */
  FLASH_ERROR("SPI 等待超时!errorCode = %d",errorCode);
  
  return 0;
}




