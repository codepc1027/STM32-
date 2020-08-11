#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_dma_mtm.h"

void Delay(uint32_t count)
{
	for(; count!=0; count--);
}

int main(void)
{
	uint8_t status;
	
	LED_GPIO_Config();
	
	LED_YELLOW;
	Delay(0xFFFFF);
	
	DMA_MTM_Config();
	while( DMA_GetFlagStatus(DMA_MTM_ClearFlag_TC) == RESET );//等待发送完成
	status = Buffercmp(aSRC_Const_Buffer, aDST_Buffer, BUFFER_SIZE);
	if( status == 0 )
  {
		LED_RED;
  }
  else
  {
		LED_GREEN;
	}
	while(1);
}

