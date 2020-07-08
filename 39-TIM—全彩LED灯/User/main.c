/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   全彩LED灯例程
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 STM32 指南者 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :http://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f10x.h"
#include "./color_led/bsp_color_led.h"


#define SOFT_DELAY() Delay(0xFFFFFF);

void Delay(__IO u32 nCount); 

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{			
	/* 初始化LED灯 */
	COLOR_TIMx_LED_Init();

	while(1)
	{
		//显示各种颜色
		SetRGBColor(0x8080ff);
		SOFT_DELAY();
		
		SetRGBColor(0xff8000);
		SOFT_DELAY();
		
		SetRGBColor(0xffc90e);
		SOFT_DELAY();
		
		SetColorValue(181,230,29);
		SOFT_DELAY();

		SetColorValue(255,128,64);
		SOFT_DELAY();		

	}		
}


void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/

