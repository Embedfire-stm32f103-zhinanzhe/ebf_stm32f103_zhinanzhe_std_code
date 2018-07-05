/**
  ******************************************************************************
  * @file    bsp_chipid.c
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   stm32 chipid
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-指南者 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 

//获取CPU的ID函数，每个芯片都有唯一的 96_bit unique ID         

#include "./chipid/bsp_chipid.h"

uint32_t ChipUniqueID[3];

/*
 * 函数名：Get_ChipID
 * 描述  ：获取芯片ID
 * 输入  ：无
 * 输出  ：无
 */
void Get_ChipID(void)
{
	ChipUniqueID[0] = *(__IO u32 *)(0X1FFFF7F0); // 高字节
	ChipUniqueID[1] = *(__IO u32 *)(0X1FFFF7EC); // 
	ChipUniqueID[2] = *(__IO u32 *)(0X1FFFF7E8); // 低字节
}

