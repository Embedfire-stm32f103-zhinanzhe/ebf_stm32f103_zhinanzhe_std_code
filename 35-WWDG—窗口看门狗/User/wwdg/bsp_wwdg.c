 
#include "bsp_wwdg.h"   



// WWDG 中断优先级初始化
static void WWDG_NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure; 
  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
  NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/* WWDG 配置函数
 * tr ：递减计时器的值， 取值范围为：0x7f~0x40
 * wr ：窗口值，取值范围为：0x7f~0x40
 * prv：预分频器值，取值可以是
 *      @arg WWDG_Prescaler_1: WWDG counter clock = (PCLK1(36MHZ)/4096)/1
 *      @arg WWDG_Prescaler_2: WWDG counter clock = (PCLK1(36mhz)/4096)/2
 *      @arg WWDG_Prescaler_4: WWDG counter clock = (PCLK1(36mhz)/4096)/4
 *      @arg WWDG_Prescaler_8: WWDG counter clock = (PCLK1(36mhz)/4096)/8
 */
void WWDG_Config(uint8_t tr, uint8_t wr, uint32_t prv)
{	
	// 开启 WWDG 时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
	
	// 设置递减计数器的值
	WWDG_SetCounter( tr );
	
	// 设置预分频器的值
	WWDG_SetPrescaler( prv );
	
	// 设置上窗口值
	WWDG_SetWindowValue( wr );
	
	// 设置计数器的值，使能WWDG
	WWDG_Enable(WWDG_CNT);	
	
	// 清除提前唤醒中断标志位
	WWDG_ClearFlag();	
	// 配置WWDG中断优先级
	WWDG_NVIC_Config();	
	// 开WWDG 中断
	WWDG_EnableIT();
}

// 喂狗
void WWDG_Feed(void)
{
	// 喂狗，刷新递减计数器的值，设置成最大WDG_CNT=0X7F
	WWDG_SetCounter( WWDG_CNT );
}


/*********************************************END OF FILE**********************/
