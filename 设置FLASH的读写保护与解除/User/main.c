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
#include "./key/bsp_key.h"  
#include "./led/bsp_led.h"
#include "./protect/bsp_readWriteProtect.h"   


// 函数原型声明
void Delay(__IO uint32_t nCount);





//【 ！！】注意事项：
//1.当芯片处于读写保护状态时，均无法下载新的程序，需要先解除保护状态后再下载
//2.本工程包含两个版本，可在MDK的“Load”下载按钮旁边的下拉框选择：
//	FLASH版本：	接上串口调试助手后，直接点击MDK的“Load”按钮把程序下载到STM32的FLASH中，
//				复位运行，串口会输出当前芯片的保护状态，可使用KEY1和KEY2切换。切换写保护
//				状态时，芯片会自动复位，程序重新执行；切换读保护状态时，按键后需要重新给
//				开发板上电复位，配置才会有效（断电时，串口与电脑的连接会断开，所以上电后
//				注意重新打开串口调试助手），若是执行解除读保护过程，运行后芯片FLASH中自身
//				的代码都会消失，所以要重新给开发板下载程序。
//	RAM版本  ：	若无SRAM调试程序的经验，请先学习前面的《SRAM调试》章节。接上串口调试助手后，
//				只能使用MDK的“Debug”按钮把程序下载到STM32的内部SRAM中，然后点击全速运行，
//				可在串口查看调试输出。由于SRAM调试状态下，复位会使芯片程序乱飞，所以每次切
//				换状态后，都要重新点击“Debug”按钮下载SRAM程序，再全速运行查看输出。


//3.若自己修改程序导致使芯片处于读写保护状态而无法下载，
//  且 FALSH程序自身又不包含自解除状态的程序，可以使用本工程的“RAM版本”解除，解除即可重新下载。


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
	Key_GPIO_Config();

	LED_BLUE;
	
	//芯片自动复位后，串口可能有小部分异常输出，如输出一个“？”号
	printf("\r\n欢迎使用野火  STM32  开发板。\r\n");	
	printf("这是读写保护测试实验\r\n");
	
	/* 获取写保护寄存器的值进行判断，寄存器位为0表示有保护，为1表示无保护 */
	/*  若不等于0xFFFFFFFF，则说明有部分页被写保护了 */
	if(FLASH_GetWriteProtectionOptionByte() !=0xFFFFFFFF )
	{
		printf("\r\n目前芯片处于写保护状态，按Key1键解除保护\r\n");
		printf("写保护寄存器的值：WRPR=0x%x\r\n",FLASH_GetWriteProtectionOptionByte());
	}
	else //无写保护
	{
		printf("\r\n目前芯片无 写 保护，按 Key1 键可设置成 写 保护\r\n");
		printf("写保护寄存器的值：WRPR=0x%x\r\n",FLASH_GetWriteProtectionOptionByte());
	}
	
	
	/*  若等于SET，说明处于读保护状态 */
	if(FLASH_GetReadOutProtectionStatus () == SET )
	{
		printf("\r\n目前芯片处于读保护状态，按Key2键解除保护\r\n");
	}
	else
	{
		printf("\r\n目前芯片无 读 保护，按 Key2 键可设置成 读 保护\r\n");
	}
	
	
	while(1)                            
	{	   
		if( Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) == KEY_ON  )
		{
			LED1_TOGGLE;
			WriteProtect_Toggle();
		} 

		if( Key_Scan(KEY2_GPIO_PORT,KEY2_GPIO_PIN) == KEY_ON  )
		{
			LED2_TOGGLE;
			ReadProtect_Toggle();			
		}		
	}	
 
}


void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/
