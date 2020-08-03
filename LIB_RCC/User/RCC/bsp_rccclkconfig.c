#include "bsp_rccclkconfig.h"

void HSE_SetSysClk( uint32_t RCC_PLLMul_x )
{
	ErrorStatus HSEStatus;
	
	//RCC��λ
	RCC_DeInit();
	// ʹ�� HSE
	RCC_HSEConfig(RCC_HSE_ON);
	// ״̬�ж�
	HSEStatus = RCC_WaitForHSEStartUp();
	
	if( HSEStatus == SUCCESS )
	{
		//ʹ��Ԥȡָ
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		// ���εȴ�
		FLASH_SetLatency(FLASH_Latency_2);
		
		//HCLK 1/   PCLK1 1/2   PCLK2 1   (72M)
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		RCC_PCLK1Config(RCC_HCLK_Div2);
		RCC_PCLK2Config(RCC_HCLK_Div1);
		
		//���໷ʱ����ԴHSE����Ƶx
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_x);
		
		//ʹ��PLL
		RCC_PLLCmd(ENABLE);
		
		//�ȴ�PLL�ȶ�
		while( RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET );
		
		//ѡ��ϵͳʱ��
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		//�ȴ�PLLΪϵͳʱ��
		while( RCC_GetSYSCLKSource() != 0x08 );
	}
	else
 {
	 /* ����ʧ�ܺ�Ĵ��� */
	}
}

