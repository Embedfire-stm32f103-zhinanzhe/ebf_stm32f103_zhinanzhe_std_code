
/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   停止模式唤醒实验
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
#include "./led/bsp_led.h"
#include "./key/bsp_exti.h" 
#include "./usart/bsp_usart.h"



static void Delay(__IO u32 nCount);
static void SYSCLKConfig_STOP(void);


/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */ 
int main(void)
{	

	RCC_ClocksTypeDef clock_status_wakeup,clock_status_config;
	uint8_t clock_source_wakeup,clock_source_config; 

	
	LED_GPIO_Config();	
	
  /*初始化USART1*/
  USART_Config();		
	
	/* 初始化按键中断，按下按键后会进入中断服务程序	*/
	EXTI_Key_Config(); 
	
	printf("\r\n 欢迎使用野火  STM32  开发板。\r\n");
  printf("\r\n 野火STM32 停止模式例程\r\n");
	
	printf("\r\n 实验说明：\r\n");

	printf("\r\n 1.本程序中，绿灯表示STM32正常运行，红灯表示停止状态，蓝灯表示刚从停止状态被唤醒\r\n");
	printf("\r\n 2.在停止状态下，可使用KEY1或KEY2唤醒\r\n");
	printf("\r\n 3.本实验执行这样一个循环：\r\n ------》亮绿灯(正常运行)->亮红灯(停止模式)->按KEY1或KEY2唤醒->亮蓝灯(刚被唤醒)-----》\r\n");
	printf("\r\n 4.在停止状态下，DAP下载器无法给STM32下载程序，\r\n 可按KEY1、KEY2唤醒后下载，\r\n 或按复位键使芯片处于复位状态，然后在电脑上点击下载按钮，再释放复位按键，即可下载\r\n");

  while(1)
  {	
		/*********执行任务***************************/
		printf("\r\n STM32正常运行，亮绿灯\r\n");
	
		LED_GREEN;	
		Delay(0x3FFFFF);
		
		/*****任务执行完毕，进入停止降低功耗***********/		
		
		printf("\r\n 进入停止模式，按KEY1或KEY2按键可唤醒\r\n");

		//使用红灯指示，进入停止状态
		LED_RED;
		
		/* 进入停止模式，设置电压调节器为低功耗模式，等待中断唤醒 */
		PWR_EnterSTOPMode(PWR_Regulator_LowPower,PWR_STOPEntry_WFI);	
		
	
		//等待中断唤醒  K1或K2按键中断	
		
		/*********************被唤醒***********************/
		
		//获取刚被唤醒时的时钟状态	
		//时钟源
		clock_source_wakeup = RCC_GetSYSCLKSource ();
		//时钟频率
		RCC_GetClocksFreq(&clock_status_wakeup);
		
		//从停止模式下被唤醒后使用的是HSI时钟，此处重启HSE时钟,使用PLLCLK
		SYSCLKConfig_STOP();
		
		//获取重新配置后的时钟状态	
		//时钟源
		clock_source_config = RCC_GetSYSCLKSource ();
		//时钟频率
		RCC_GetClocksFreq(&clock_status_config);
	
		//因为刚唤醒的时候使用的是HSI时钟，会影响串口波特率，输出不对，所以在重新配置时钟源后才使用串口输出。
		printf("\r\n重新配置后的时钟状态：\r\n");
		printf(" SYSCLK频率:%d,\r\n HCLK频率:%d,\r\n PCLK1频率:%d,\r\n PCLK2频率:%d,\r\n 时钟源:%d (0表示HSI，8表示PLLCLK)\n", 
			clock_status_config.SYSCLK_Frequency, 
			clock_status_config.HCLK_Frequency, 
			clock_status_config.PCLK1_Frequency, 
			clock_status_config.PCLK2_Frequency, 
			clock_source_config);
			
		printf("\r\n刚唤醒的时钟状态：\r\n");	
		printf(" SYSCLK频率:%d,\r\n HCLK频率:%d,\r\n PCLK1频率:%d,\r\n PCLK2频率:%d,\r\n 时钟源:%d (0表示HSI，8表示PLLCLK)\n", 
			clock_status_wakeup.SYSCLK_Frequency, 
			clock_status_wakeup.HCLK_Frequency, 
			clock_status_wakeup.PCLK1_Frequency, 
			clock_status_wakeup.PCLK2_Frequency, 
			clock_source_wakeup);
		
		/*指示灯*/
		LED_BLUE;	
		Delay(0x1FFFFF);		
			
		printf("\r\n 已退出停止模式\r\n");
		//继续执行while循环

  }
}


static void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}



/**
  * @brief  停机唤醒后配置系统时钟: 使能 HSE, PLL
  *         并且选择PLL作为系统时钟.
  * @param  None
  * @retval None
  */
static void SYSCLKConfig_STOP(void)
{
  /* After wake-up from STOP reconfigure the system clock */
  /* 使能 HSE */
  RCC_HSEConfig(RCC_HSE_ON);
  
  /* 等待 HSE 准备就绪 */
  while (RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
  {
  }
  
  /* 使能 PLL */ 
  RCC_PLLCmd(ENABLE);
  
  /* 等待 PLL 准备就绪 */
  while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
  {
  }
  
  /* 选择PLL作为系统时钟源 */
  RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
  
  /* 等待PLL被选择为系统时钟源 */
  while (RCC_GetSYSCLKSource() != 0x08)
  {
  }
}
/*********************************************END OF FILE**********************/
