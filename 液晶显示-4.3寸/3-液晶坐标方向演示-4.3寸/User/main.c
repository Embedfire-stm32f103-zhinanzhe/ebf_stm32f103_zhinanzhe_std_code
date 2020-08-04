/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2020-xx-xx
  * @brief   LCD显示英文
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
#include "./lcd/bsp_NT35510_lcd.h"
#include "./usart/bsp_usart.h" 
#include <stdio.h>

static void LCD_Test(void);	
static void Delay ( __IO uint32_t nCount );
void Printf_Charater(void)   ;
static void LCD_Direction_Show(void);	

/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main ( void )
{

	NT35510_Init ();         //LCD 初始化

	USART_Config();		
	
	printf("\r\n ********** 液晶屏显示方向说明程序*********** \r\n"); 
	printf("\r\n 本程序不支持中文，显示中文的程序请学习下一章 \r\n"); 
	
 //其中0、3、5、6 模式适合从左至右显示文字，
 //不推荐使用其它模式显示文字	其它模式显示文字会有镜像效果			
 //其中 6 模式为大部分液晶例程的默认显示方向  
  NT35510_GramScan ( 6 );
	while ( 1 )
	{
		//展示LCD的八种方向模式
		LCD_Direction_Show();
	}
	
	
}

/*用于展示LCD的八种方向模式*/
void LCD_Direction_Show(void)
{

	uint8_t i = 0;	
	char dispBuff[100];
	
	//轮流展示各个方向模式
	for(i=0;i<8;i++)
	{	
		LCD_SetFont(&Font16x32);
		LCD_SetColors(RED,BLACK);

		NT35510_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* 清屏，显示全黑 */
		
	 //其中0、3、5、6 模式适合从左至右显示文字，
	 //不推荐使用其它模式显示文字	其它模式显示文字会有镜像效果			
	 //其中 6 模式为大部分液晶例程的默认显示方向  
		NT35510_GramScan ( i );
			
		sprintf(dispBuff,"o%d. X --->",i);
		NT35510_DispStringLine_EN(LINE(0),dispBuff);//沿X方向显示文字
			
		sprintf(dispBuff,"o%d.Y|V",i);	
		NT35510_DispString_EN_YDir(0,0,dispBuff);//沿Y方向显示文字
			
		Delay(0x9FFFFF);		
		
		//显示测试
		// *	！！！其中0、3、5、6 模式适合从左至右显示文字，不推荐使用其它模式显示文字	
		//其它模式显示文字会有镜像效果
		LCD_Test();
	}

}

//*用于测试各种液晶的函数*/
void LCD_Test(void)
{
	/*演示显示变量*/
	static uint8_t testCNT = 0;	
	char dispBuff[100];
	
	testCNT++;	
	
	LCD_SetFont(&Font16x32);
	LCD_SetColors(RED,BLACK);

  NT35510_Clear(0,0,LCD_X_LENGTH,LCD_Y_LENGTH);	/* 清屏，显示全黑 */
  Delay(0x9FFFFF);
	/********显示字符串示例*******/
  NT35510_DispStringLine_EN(LINE(0),"YH 4.3 inch LCD");
  NT35510_DispStringLine_EN(LINE(2),"resolution:480x800px");
  NT35510_DispStringLine_EN(LINE(3),"LCD driver:NT35510");
  NT35510_DispStringLine_EN(LINE(4),"Touch driver:GT917S");
  
	/********显示变量示例*******/
	LCD_SetTextColor(GREEN);

	/*使用c标准库把变量转化成字符串*/
	sprintf(dispBuff,"Count : %d ",testCNT);
  NT35510_ClearLine(LINE(7));	/* 清除单行文字 */
	
	/*然后显示该字符串即可，其它变量也是这样处理*/
	NT35510_DispStringLine_EN(LINE(7),dispBuff);

	/*******显示图形示例******/
  /* 画直线 */
  
  NT35510_ClearLine(LINE(7));/* 清除单行文字 */
	LCD_SetTextColor(BLUE);

  NT35510_DispStringLine_EN(LINE(7),"Draw line:");
  
	LCD_SetTextColor(RED);
  NT35510_DrawLine(50,270,420,275);  
  NT35510_DrawLine(50,300,420,375);
  
	LCD_SetTextColor(GREEN);
  NT35510_DrawLine(50,370,420,475);  
  NT35510_DrawLine(50,400,420,475);
	
	LCD_SetTextColor(BLUE);
  NT35510_DrawLine(50,420,420,325);  
  NT35510_DrawLine(50,450,420,395);
  
  Delay(0x9FFFFF);
  
  NT35510_Clear(0,32*7,LCD_X_LENGTH,LCD_Y_LENGTH-32*7);	/* 清屏，显示全黑 */
  
  
  /*画矩形*/

  NT35510_ClearLine(LINE(7));	/* 清除单行文字 */
	LCD_SetTextColor(BLUE);

  NT35510_DispStringLine_EN(LINE(7),"Draw Rect:");

	LCD_SetTextColor(RED);
  NT35510_DrawRectangle(50,300,200,100,1);
	
	LCD_SetTextColor(GREEN);
  NT35510_DrawRectangle(100,300,200,120,0);
	
	LCD_SetTextColor(BLUE);
  NT35510_DrawRectangle(250,300,200,150,1);
  
  
  Delay(0x9FFFFF);
	
	NT35510_Clear(0,32*7,LCD_X_LENGTH,LCD_Y_LENGTH-32*7);	/* 清屏，显示全黑 */

  /* 画圆 */
  NT35510_ClearLine(LINE(7));	/* 清除单行文字 */
	LCD_SetTextColor(BLUE);
	
  NT35510_DispStringLine_EN(LINE(7),"Draw Cir:");

	LCD_SetTextColor(RED);
	NT35510_DrawCircle(150,400,60,1);

	LCD_SetTextColor(GREEN);
	NT35510_DrawCircle(250,400,60,0);

	LCD_SetTextColor(BLUE);
	NT35510_DrawCircle(350,400,60,1);

  Delay(0x9FFFFF);
  
  NT35510_Clear(0,32*7,LCD_X_LENGTH,LCD_Y_LENGTH-32*7);	/* 清屏，显示全黑 */

}

/**
  * @brief  简单延时函数
  * @param  nCount ：延时计数值
  * @retval 无
  */	
static void Delay ( __IO uint32_t nCount )
{
  for ( ; nCount != 0; nCount -- );
	
}

/* ------------------------------------------end of file---------------------------------------- */

