/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   触摸画板实验
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-指南者 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 
 
#include "stm32f10x.h"
#include "./usart/bsp_usart.h"	
#include "./lcd/bsp_ili9341_lcd.h"
#include "./lcd/bsp_xpt2046_lcd.h"
#include "./flash/bsp_spi_flash.h"
#include "./led/bsp_led.h" 
#include "palette.h"
#include <string.h>




int main(void)
{		
	//LCD 初始化
	ILI9341_Init();  
	
	//触摸屏初始化
	XPT2046_Init();
	//从FLASH里获取校正参数，若FLASH无参数，则使用模式3进行校正
	Calibrate_or_Get_TouchParaWithFlash(3,0);

	/* USART config */
	USART_Config();  
	LED_GPIO_Config();
	
	printf("\r\n ********** 触摸画板程序 *********** \r\n"); 
	printf("\r\n 若汉字显示不正常，请阅读工程中的readme.txt文件说明，根据要求给FLASH重刷字模数据\r\n"); 
 
	//其中0、3、5、6 模式适合从左至右显示文字，
	//不推荐使用其它模式显示文字	其它模式显示文字会有镜像效果			
	//其中 6 模式为大部分液晶例程的默认显示方向  
  ILI9341_GramScan ( 3 );	
	
	//绘制触摸画板界面
	Palette_Init(LCD_SCAN_MODE);
	
	while ( 1 )
	{
		//触摸检测函数，本函数至少10ms调用一次
			XPT2046_TouchEvenHandler();
	}
		
}





/* ------------------------------------------end of file---------------------------------------- */

