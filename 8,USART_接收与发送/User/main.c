#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart.h"

int main(void)
{
	//uint8_t a[10] = {100,101,102,103,104,105,106,107,108,109};
	USART_Config();
	//Usart_SendByte(DEBUG_USARTx, 'a');
	//Usart_SendHalfWord(DEBUG_USARTx, 0xff56);
	//Usart_SendArr(DEBUG_USARTx, a, 10);
	//Usart_SendStr(DEBUG_USARTx, "����ã���ϵ\n");
	
	printf("����printf��������\n");
	
	while(1)
	{

	}
}

