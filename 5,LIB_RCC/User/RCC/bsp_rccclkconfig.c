#include "bsp_rccclkconfig.h"

void HSE_SetSysClk( uint32_t RCC_PLLMul_x )
{
	ErrorStatus HSEStatus;
	
	//RCC复位
	RCC_DeInit();
	// 使能 HSE
	RCC_HSEConfig(RCC_HSE_ON);
	// 状态判断
	HSEStatus = RCC_WaitForHSEStartUp();
	
	if( HSEStatus == SUCCESS )
	{
		//使能预取指
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		// 两次等待
		FLASH_SetLatency(FLASH_Latency_2);
		
		//HCLK 1/   PCLK1 1/2   PCLK2 1   (72M)
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		RCC_PCLK1Config(RCC_HCLK_Div2);
		RCC_PCLK2Config(RCC_HCLK_Div1);
		
		//锁相环时钟来源HSE，倍频x
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_x);
		
		//使能PLL
		RCC_PLLCmd(ENABLE);
		
		//等待PLL稳定
		while( RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET );
		
		//选择系统时钟
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		//等待PLL为系统时钟
		while( RCC_GetSYSCLKSource() != 0x08 );
	}
	else
 {
	 /* 启动失败后的处理 */
	}
}

