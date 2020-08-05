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
	
	//�򿪴���GPIO��ʱ��
	DEBUG_USART_GPIO_APBxClkCmd(DEBUG_USART_GPIO_CLK, ENABLE);
	//�򿪴��������ʱ��
	DEBUG_USART_APBxClkCmd(DEBUG_USART_CLK, ENABLE);
	
	//��ʼ��USART_TX��GPIO������Ϊ���츴�����ģʽ
	GPIO_InitStruct.GPIO_Pin = DEBUG_USART_TX_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStruct);
	//��ʼ��USART_RX��GPIO������Ϊ��������ģʽ
	GPIO_InitStruct.GPIO_Pin = DEBUG_USART_RX_GPIO_PIN;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStruct);
	
	//���ô��ڵĹ�������
	//���ò�����
	USART_InitStruct.USART_BaudRate = DEBUG_USART_BAUDRATE;
	//���� �������ӳ�
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	//����ֹͣλ
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	//����У��λ
	USART_InitStruct.USART_Parity = USART_Parity_No;
	//����Ӳ��������
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	//���ù���ģʽ���շ�һ��
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//��ɴ��ڳ�ʼ������
	USART_Init(DEBUG_USARTx, &USART_InitStruct);
	
//	//�����ж����ȼ�����
//	NVIC_Config();
//	
//	//ʹ�ܴ��ڽ����ж�
//	USART_ITConfig(DEBUG_USARTx, USART_IT_RXNE, ENABLE);
	
	//ʹ�ܴ���
	USART_Cmd(DEBUG_USARTx, ENABLE);
}

//����һ���ֽ�
void Usart_SendByte(USART_TypeDef* pUSARTx, uint8_t data)
{
	USART_SendData(pUSARTx, data);
	while( USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET );//�ȴ��������
}

//���������ֽ�
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

//����8λ���ݵ�����
void Usart_SendArr(USART_TypeDef* pUSARTx, uint8_t *array, uint8_t num)
{
	uint8_t i;
	for( i=0; i<num; i++ )
	{
		Usart_SendByte(pUSARTx, array[i]);
	}
	while( USART_GetFlagStatus(pUSARTx, USART_FLAG_TC) == RESET );//�໺����TC�ж�
}

//�����ַ���
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

//�ض���C�⺯��printf�����ڣ��ض�����ʹ��printf������printf��Ҫ����fputc������
int fputc(int ch, FILE *f)
{
	Usart_SendByte(DEBUG_USARTx, (uint8_t) ch);//����һ���ֽ�
	while( USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET );//�ȴ��������
	
	return (ch);
}

//�ض���C�⺯��scanf�����ڣ��ض�����ʹ��scanf������scanf��Ҫ����fgetc������
int fgetc(FILE *f)
{
	while( USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_RXNE) == RESET );//�ȴ�������������
	
	return (int)USART_ReceiveData(DEBUG_USARTx);
}
