/**
  ******************************************************************************
  * @file    FSMC—外部SRAM(内存管理)
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   sram应用例程
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-指南者 STM32  开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
#include "stm32f10x.h"
#include "./usart/bsp_usart.h"
#include "./led/bsp_led.h"  
#include "./sram/sram.h"
#include "malloc.h"



int main(void)
 {	   
	u8 *p=0;
	u8 sramx=0;				//默认为内部sram
  

  Debug_USART_Config();		 	                         //串口初始化为115200 
 	FSMC_SRAM_Init();			  //初始化外部SRAM  
	my_mem_init(SRAMIN);		//初始化内部内存池
	my_mem_init(SRAMEX);		//初始化外部内存池

	 
	printf ( "\r\n野火内存管理\r\n" );

	 
	printf ( "\r\n分配内存\r\n" );
	 
  p = mymalloc ( sramx, 1024 *2 );//申请2K字节
	
	sprintf((char*)p,"Memory Malloc");//使用申请空间存储内容
	
	printf ( "SRAMIN USED:%d%%\r\n", my_mem_perused(SRAMIN) );//显示内部内存使用率
	printf ( "SRAMEX USED:%d%%\r\n", my_mem_perused(SRAMEX) );//显示外部内存使用率

	myfree(sramx,p);//释放内存
	p=0;		      	//指向空地址

  printf ( "\r\n释放内存\r\n" );
	printf ( "SRAMIN USED:%d%%\r\n", my_mem_perused(SRAMIN) );//显示内部内存使用率
	printf ( "SRAMEX USED:%d%%\r\n", my_mem_perused(SRAMEX) );//显示外部内存使用率
	
	
  while(1);
	
		
}



/*********************************************END OF FILE**********************/
