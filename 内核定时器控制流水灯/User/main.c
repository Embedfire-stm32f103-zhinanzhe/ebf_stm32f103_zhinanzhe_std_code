
#include "stm32f10x.h"
#include "bsp_led.h"
#include "./dwt_delay/core_delay.h"   

/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{	
	/* LED 端口初始化 */
	LED_GPIO_Config();

	/* 配置内核定时器 */
	CPU_TS_TmrInit();

	for(;;)
	{

		LED1( ON ); 
	    Delay_us(100000);    	// 100000 * 10us = 1000ms
		//Delay_ms(100);
		LED1( OFF );
	  
		LED2( ON );
	    Delay_us(100000);		// 100000 * 10us = 1000ms
		//Delay_ms(100);
		LED2( OFF );
	
		LED3( ON );
	    Delay_us(100000);		// 100000 * 10us = 1000ms
		//Delay_ms(100);
		LED3( OFF );
	} 
}


/*********************************************END OF FILE**********************/
