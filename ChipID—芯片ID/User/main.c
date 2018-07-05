/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   获取芯片的ID
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-指南者 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 

// 通过ADC1通道16获取芯片内部温度，并通过串口打印到电脑串口调试助手显示

#include "stm32f10x.h"
#include "./usart/bsp_usart.h"
#include "./chipid/bsp_chipid.h"

extern uint32_t ChipUniqueID[3];

/*
 * 函数名：main
 * 描述  ：主函数
 * 输入  ：无
 * 输出  ：无
 */
int main(void)
{	
	/* USART1 config 115200 8-N-1 */
	USART_Config();

	/* to get the chipid and put it in ChipUniqueID[3] */
	Get_ChipID();	
	
	/* printf the chipid */
	printf("\r\n芯片的唯一ID为: 0x%08X-%08X-%08X\r\n",
	        ChipUniqueID[0],ChipUniqueID[1],ChipUniqueID[2]);
	
	/* printf the flash memory amount */
	printf("\r\n芯片flash的容量为: %dK \r\n", *(__IO u16 *)(0X1FFFF7E0));

	for(;;)
	{
		
	}
}
