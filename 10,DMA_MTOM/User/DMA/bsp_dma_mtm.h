#ifndef __BSP_DMA_MTM_H
#define __BSP_DMA_MTM_H

#include "stm32f10x.h"

#define BUFFER_SIZE 		32
extern const uint32_t aSRC_Const_Buffer[BUFFER_SIZE];
extern uint32_t aDST_Buffer[BUFFER_SIZE];

#define DMA_MTM_CLK    					 RCC_AHBPeriph_DMA1
#define DMA_MTM_Channel					 DMA1_Channel6
#define DMA_MTM_ClearFlag_TC     DMA1_FLAG_TC6

void DMA_MTM_Config(void);
uint8_t Buffercmp(const uint32_t* pBuffer, 
                  uint32_t* pBuffer1, uint16_t BufferLength);

#endif /* __BSP_DMA_MTM_H */

