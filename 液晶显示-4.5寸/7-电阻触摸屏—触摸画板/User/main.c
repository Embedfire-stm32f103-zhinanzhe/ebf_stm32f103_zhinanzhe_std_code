/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   电容触摸屏-触摸画板
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F407 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "stm32f10x.h"
#include "./usart/bsp_usart.h"
#include "./led/bsp_led.h"  
#include "./lcd/bsp_ili9806g_lcd.h"
#include "./touch/gt5xx.h"
#include "./dwt_delay/core_delay.h"   
#include "./touch/palette.h"



/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main ( void )
{
	LED_GPIO_Config();
	ILI9806G_Init ();         //LCD 初始化

	USART_Config();	

  /* 初始化内核定时器 */
  CPU_TS_TmrInit();
	
	//其中0、3、5、6 模式适合从左至右显示文字，
	//不推荐使用其它模式显示文字	其它模式显示文字会有镜像效果			
	//其中 6 模式为大部分液晶例程的默认显示方向  
  ILI9806G_GramScan ( 3 );	

	/* 设定好液晶扫描方向后，再初始化触摸屏，触摸屏会根据液晶的扫描方向输出匹配的触摸坐标 */
	/* 每次修改液晶扫描方向后，应重新调用一次GTP_Init_Panel函数更新触摸配置 */
	GTP_Init_Panel(); 
    
	printf("\r\n ********** 触摸画板程序 *********** \r\n"); 
	printf("\r\n 若汉字显示不正常，请阅读工程中的readme.txt文件说明，根据要求给FLASH重刷字模数据\r\n"); 
 
	
	//绘制触摸画板界面
	Palette_Init(LCD_SCAN_MODE);
    
	while ( 1 )
	{
	}
	
	
}





/*********************************************END OF FILE**********************/

