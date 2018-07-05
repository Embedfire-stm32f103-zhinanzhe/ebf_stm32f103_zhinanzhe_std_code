/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   睡眠模式例程
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 指南者 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
#include "stm32f10x.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_usart.h"
#include "./Key/bsp_exti.h"


static void Delay(__IO u32 nCount);


/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{ 
	
	LED_GPIO_Config();	
	
  /*初始化USART1*/
  USART_Config();		
	
	/* 初始化按键为中断模式，按下中断后会进入中断服务函数	*/
	EXTI_Key_Config(); 
	
	printf("\r\n 欢迎使用野火  STM32  开发板。\r\n");
  printf("\r\n 野火STM32 睡眠模式例程\r\n");
	
	printf("\r\n 实验说明：\r\n");

	printf("\r\n 1.本程序中，绿灯表示STM32正常运行，红灯表示睡眠状态，蓝灯表示刚从睡眠状态被唤醒\r\n");
	printf("\r\n 2.程序运行一段时间后自动进入睡眠状态，在睡眠状态下，可使用KEY1或KEY2唤醒\r\n");
	printf("\r\n 3.本实验执行这样一个循环：\r\n ------》亮绿灯(正常运行)->亮红灯(睡眠模式)->按KEY1或KEY2唤醒->亮蓝灯(刚被唤醒)-----》\r\n");
	printf("\r\n 4.在睡眠状态下，DAP下载器无法给STM32下载程序，\r\n 可按KEY1、KEY2唤醒后下载，\r\n 或按复位键使芯片处于复位状态，然后在电脑上点击下载按钮，再释放复位按键，即可下载\r\n");

  while(1)
  {	
		/*********执行任务***************************/
		printf("\r\n STM32正常运行，亮绿灯\r\n");
	
		LED_GREEN;	
		Delay(0x3FFFFF);
		
		/*****任务执行完毕，进入睡眠降低功耗***********/
		
		
		printf("\r\n 进入睡眠模式，按KEY1或KEY2按键可唤醒\r\n");

		//使用红灯指示，进入睡眠状态
		LED_RED;
		//进入睡眠模式
		__WFI();	//WFI指令进入睡眠
		
		//等待中断唤醒  K1或K2按键中断	
		
		/***被唤醒，亮蓝灯指示***/
		LED_BLUE;	
		Delay(0x1FFFFF);		
			
		printf("\r\n 已退出睡眠模式\r\n");
		//继续执行while循环

  }

}

static void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}



/*********************************************END OF FILE**********************/
