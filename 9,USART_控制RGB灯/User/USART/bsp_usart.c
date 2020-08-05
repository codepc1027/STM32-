#include "bsp_usart.h"

//void NVIC_Config(void)
//{
//	NVIC_InitTypeDef NVIC_InitStruct;
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
//	
//	NVIC_InitStruct.NVIC_IRQChannel = DEBUG_USART_IRQ;
//	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
//	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStruct);
//}

void USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	
	//打开串口GPIO的时钟
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
	//打开串口外设的时钟
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);
	
	//初始化USART_TX的GPIO，配置为推挽复用输出模式
	GPIO_InitStruct.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStruct);
	//初始化USART_RX的GPIO，配置为浮空输入模式
	GPIO_InitStruct.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStruct);
	
	//配置串口的工作参数
	//配置波特率
	USART_InitStruct.USART_BaudRate = DEBUG_USART_BAUDRATE;
	//配置 针数据子长
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	//配置停止位
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	//配置校验位
	USART_InitStruct.USART_Parity = USART_Parity_No;
	//配置硬件流控制
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	//配置工作模式，收发一起
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//完成串口初始化配置
	USART_Init(DEBUG_USARTx, &USART_InitStruct);
	
//	//串口中断优先级配置
//	NVIC_Config();
//	
//	//使能串口接收中断
//	USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);
	
	//使能串口
	USART_Cmd(DEBUG_USARTx, ENABLE);
}

//发送一个字节
void Usart_SendByte(USART_TypeDef* pUSARTx, uint8_t data)
{
	USART_SendData(pUSARTx, data);
	while( USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET );//等待发送完成
}

//发送两个字节
void Usart_SendHalfWord(USART_TypeDef* pUSARTx, uint16_t data)
{
	uint8_t data_h, data_l;
	data_h = (data & 0xff00) >> 8;
	data_l = (data & 0xff);
	USART_SendData(pUSARTx, data_h);
	while( USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET );
	USART_SendData(pUSARTx, data_l);
	while( USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET );
}

//发送8位数据的数组
void Usart_SendArr(USART_TypeDef* pUSARTx, uint8_t *array, uint8_t num)
{
	uint8_t i;
	for( i=0; i<num; i++ )
	{
		Usart_SendByte(pUSARTx, array[i]);
	}
	while( USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET );//多缓存是TC判断
}

//发送字符串
void Usart_SendStr(USART_TypeDef* pUSARTx, uint8_t *str)
{
	uint8_t i = 0;
	do
	{
		Usart_SendByte(pUSARTx, *(str+i));
		i++;
	}while(*(str+i) != '\0');
	while( USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET );
}

//重定向C库函数printf到串口，重定向后可使用printf函数（printf需要调用fputc函数）
int fputc(int ch, FILE *f)
{
	Usart_SendByte(DEBUG_USARTx, (uint8_t) ch);//发送一个字节
	while( USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET );//等待发送完成
	
	return (ch);
}

//重定向C库函数scanf到串口，重定向后可使用scanf函数（scanf需要调用fgetc函数）
int fgetc(FILE *f)
{
	while( USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET );//等待串口输入数据
	
	return (int)USART_ReceiveData(DEBUG_USARTx);
}
