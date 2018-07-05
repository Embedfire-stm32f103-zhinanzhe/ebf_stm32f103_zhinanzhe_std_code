// TIM—通用-捕获-测量脉宽 应用
#include "stm32f10x.h"
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_GeneralTim.h"  

/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{
	uint32_t time;
	
	// TIM 计数器的驱动时钟
	uint32_t TIM_PscCLK = 72000000 / (GENERAL_TIM_PSC+1);
	
	/* 串口初始化 */
	USART_Config();
	
	/* 定时器初始化 */
	GENERAL_TIM_Init();
	
	printf ( "\r\n野火 STM32 输入捕获实验\r\n" );
	printf ( "\r\n按下K1，测试K1按下的时间\r\n" );
	
	while ( 1 )
	{
		if(TIM_ICUserValueStructure.Capture_FinishFlag == 1)
		{
			// 计算高电平时间的计数器的值
			time = TIM_ICUserValueStructure.Capture_Period * (GENERAL_TIM_PERIOD+1) + 
			       (TIM_ICUserValueStructure.Capture_CcrValue+1);
			
			// 打印高电平脉宽时间
			printf ( "\r\n测得高电平脉宽时间：%d.%d s\r\n",time/TIM_PscCLK,time%TIM_PscCLK );
			
			TIM_ICUserValueStructure.Capture_FinishFlag = 0;			
		}		
	}
}
/*********************************************END OF FILE**********************/
