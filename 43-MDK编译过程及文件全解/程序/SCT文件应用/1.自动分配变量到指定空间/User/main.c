/**
  ******************************************************************************
  * @file    SCT文件应用
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   SCT文件应用
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
#include <stdlib.h>


void Delay(__IO u32 nCount); 




//定义变量到“指定的存储空间”
uint32_t testValue  =7 ;
//定义变量到“指定的存储空间”
uint32_t testValue2  =0;



//定义数组到“指定的存储空间”
uint8_t testGrup[100]  ={0};
//定义数组到“指定的存储空间”
uint8_t testGrup2[100] ={1,2,3};


/*本实验中的sct配置，若使用外部存储器时，堆区工作可能不正常，
  使用malloc无法得到正常的地址，不推荐在实际工程应用*/
/*另一种我们推荐的配置请参考教程中的说明*/

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	uint32_t inerTestValue =10;
	
	/* LED 端口初始化 */
	LED_GPIO_Config();	 
    
  /* 初始化串口 */
  USART_Config();
  
  printf("\r\nSCT文件应用——自动分配变量到“指定的存储空间”实验\r\n");
  
	printf("\r\n使用“	uint32_t inerTestValue =10; ”语句定义的局部变量：\r\n");
	printf("结果：它的地址为：0x%x,变量值为：%d\r\n",(uint32_t)&inerTestValue,inerTestValue);
	
  printf("\r\n使用“uint32_t testValue  =7 ;”语句定义的全局变量：\r\n");
	printf("结果：它的地址为：0x%x,变量值为：%d\r\n",(uint32_t)&testValue,testValue);
	
  printf("\r\n使用“uint32_t testValue2  =0 ; ”语句定义的全局变量：\r\n");
	printf("结果：它的地址为：0x%x,变量值为：%d\r\n",(uint32_t)&testValue2,testValue2);
	
	
	printf("\r\n使用“uint8_t testGrup[100]  ={0};”语句定义的全局数组：\r\n");
	printf("结果：它的地址为：0x%x,变量值为：%d,%d,%d\r\n",(uint32_t)&testGrup,testGrup[0],testGrup[1],testGrup[2]);
	
  printf("\r\n使用“uint8_t testGrup2[100] ={1,2,3};”语句定义的全局数组：\r\n");
	printf("结果：它的地址为：0x%x,变量值为：%d，%d,%d\r\n",(uint32_t)&testGrup2,testGrup2[0],testGrup2[1],testGrup2[2]);
	
	
/*本实验中的sct配置，若使用外部存储器时，堆区工作可能不正常，
  使用malloc无法得到正常的地址，不推荐在实际工程应用*/
  /*另一种我们推荐的配置请参考教程中的说明*/
	
	uint32_t * pointer = (uint32_t*)malloc(sizeof(uint32_t)*3); 
	if(pointer != NULL)
	{
		*(pointer)=1;
		*(++pointer)=2;
		*(++pointer)=3;	
		
		printf("\r\n使用“	uint32_t *pointer = (uint32_t*)malloc(sizeof(uint32_t)*3); ”动态分配的变量\r\n");
		printf("\r\n定义后的操作为：\r\n*(pointer++)=1;\r\n*(pointer++)=2;\r\n*pointer=3;\r\n\r\n");
		printf("结果：操作后它的地址为：0x%x,查看变量值操作：\r\n",(uint32_t)pointer); 
		printf("*(pointer--)=%d, \r\n",*(pointer--));
		printf("*(pointer--)=%d, \r\n",*(pointer--));
		printf("*(pointer)=%d, \r\n",*(pointer));
		
		free(pointer);
	}
	else
	{
		printf("\r\n使用malloc动态分配变量出错！！！\r\n");	
	}
  
  LED_BLUE; 
	

  while(1); 
 

}


void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}

/*********************************************END OF FILE**********************/
