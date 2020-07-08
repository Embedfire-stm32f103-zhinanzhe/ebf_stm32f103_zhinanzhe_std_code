 /**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   华邦 8M串行flash测试，并将测试信息通过串口1在电脑的超级终端中打印出来
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
#include "./led/bsp_led.h"
#include "./internal_flash/bsp_internal_flash.h"   


// 函数原型声明
void Delay(__IO uint32_t nCount);
/*
 * 函数名：main
 * 描述  ：主函数
 * 输入  ：无
 * 输出  ：无
 */
int main(void)
{ 	
	  /*初始化USART，配置模式为 115200 8-N-1*/
  USART_Config();
	LED_GPIO_Config();
 
	LED_BLUE;
	printf("\r\n 欢迎使用野火  STM32  开发板。\r\n");	
	printf("正在进行读写内部FLASH实验，请耐心等待\r\n");
	
	if(InternalFlash_Test()== PASSED)
	{
		LED_GREEN;
		printf("读写内部FLASH测试成功\r\n");

	}
	else
	{
		printf("读写内部FLASH测试失败\r\n");
		LED_RED;
	}
	
	
  while(1)
	{	} 
}


void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/
