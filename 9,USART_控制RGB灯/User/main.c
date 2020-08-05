#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart.h"

int main(void)
{
	//uint8_t a[10] = {100,101,102,103,104,105,106,107,108,109};
	uint8_t ch;
	USART_Config();
	LED_GPIO_Config();
	//Usart_SendByte(DEBUG_USARTx, 'a');
	//Usart_SendHalfWord(DEBUG_USARTx, 0xff56);
	//Usart_SendArr(DEBUG_USARTx, a, 10);
	//Usart_SendStr(DEBUG_USARTx, "大噶好，我系\n");
	
	//printf("串口printf函数测试\n");
	
	printf("串口控制RGB灯\n");
	while(1)
	{
		ch = getchar();
		printf("ch=%c\n", ch);
		
		switch(ch)
		{
			case '1': LED_RED;
				break;
			case '2': LED_GREEN;
				break;
			case '3': LED_BLUE;
				break;
			default: LED_RGBOFF;
				break;
		}
	}
}

