/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   SD卡文件系统例程
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 F103-指南者 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
#include "stm32f10x.h"
#include "./sdio/bsp_sdio_sdcard.h"
#include "./usart/bsp_usart.h"	
#include "./led/bsp_led.h"
#include "ff.h"

/**
  ******************************************************************************
  *                              定义变量
  ******************************************************************************
  */
FATFS fs;													/* FatFs文件系统对象 */
FIL fnew;													/* 文件对象 */
FRESULT res_sd;                /* 文件操作结果 */
UINT fnum;            					  /* 文件成功读写数量 */
BYTE ReadBuffer[1024]={0};        /* 读缓冲区 */
BYTE WriteBuffer[] =              /* 写缓冲区*/
"欢迎使用野火STM32 开发板 今天是个好日子，新建文件系统测试文件\r\n";  
extern  SD_CardInfo SDCardInfo;
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	/* 初始化LED */
	LED_GPIO_Config();	
	LED_BLUE;
	
	/* 初始化调试串口，一般为串口1 */
	USART_Config();	
  printf("\r\n****** 这是一个SD卡 文件系统实验 ******\r\n");
  
	//在外部SPI Flash挂载文件系统，文件系统挂载时会对SPI设备初始化
	res_sd = f_mount(&fs,"0:",1);
	
//	printf("容量=%lld",SDCardInfo.CardCapacity/1024/1024);
/*----------------------- 格式化测试 ---------------------------*/  
	/* 如果没有文件系统就格式化创建创建文件系统 */
	if(res_sd == FR_NO_FILESYSTEM)
	{
		printf("》SD卡还没有文件系统，即将进行格式化...\r\n");
    /* 格式化 */
		res_sd=f_mkfs("0:",0,0);							
		
		if(res_sd == FR_OK)
		{
			printf("》SD卡已成功格式化文件系统。\r\n");
      /* 格式化后，先取消挂载 */
			res_sd = f_mount(NULL,"0:",1);			
      /* 重新挂载	*/			
			res_sd = f_mount(&fs,"0:",1);
		}
		else
		{
			LED_RED;
			printf("《《格式化失败。》》\r\n");
			while(1);
		}
	}
  else if(res_sd!=FR_OK)
  {
    printf("！！SD卡挂载文件系统失败。(%d)\r\n",res_sd);
    printf("！！可能原因：SD卡初始化不成功。\r\n");
		while(1);
  }
  else
  {
    printf("》文件系统挂载成功，可以进行读写测试\r\n");
  }
  
/*----------------------- 文件系统测试：写测试 -----------------------------*/
	/* 打开文件，如果文件不存在则创建它 */
	printf("\r\n****** 即将进行文件写入测试... ******\r\n");	
	res_sd = f_open(&fnew, "0:FatFs读写测试文件.txt",FA_CREATE_ALWAYS | FA_WRITE );
	if ( res_sd == FR_OK )
	{
		printf("》打开/创建FatFs读写测试文件.txt文件成功，向文件写入数据。\r\n");
    /* 将指定存储区内容写入到文件内 */
		res_sd=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
    if(res_sd==FR_OK)
    {
      printf("》文件写入成功，写入字节数据：%d\n",fnum);
      printf("》向文件写入的数据为：\r\n%s\r\n",WriteBuffer);
    }
    else
    {
      printf("！！文件写入失败：(%d)\n",res_sd);
    }    
		/* 不再读写，关闭文件 */
    f_close(&fnew);
	}
	else
	{	
		LED_RED;
		printf("！！打开/创建文件失败。\r\n");
	}
	
/*------------------- 文件系统测试：读测试 ------------------------------------*/
	printf("****** 即将进行文件读取测试... ******\r\n");
	res_sd = f_open(&fnew, "0:FatFs读写测试文件.txt", FA_OPEN_EXISTING | FA_READ); 	 
	if(res_sd == FR_OK)
	{
		LED_GREEN;
		printf("》打开文件成功。\r\n");
		res_sd = f_read(&fnew, ReadBuffer, sizeof(ReadBuffer), &fnum); 
    if(res_sd==FR_OK)
    {
      printf("》文件读取成功,读到字节数据：%d\r\n",fnum);
      printf("》读取得的文件数据为：\r\n%s \r\n", ReadBuffer);	
    }
    else
    {
      printf("！！文件读取失败：(%d)\n",res_sd);
    }		
	}
	else
	{
		LED_RED;
		printf("！！打开文件失败。\r\n");
	}
	/* 不再读写，关闭文件 */
	f_close(&fnew);	
  
	/* 不再使用文件系统，取消挂载文件系统 */
	f_mount(NULL,"0:",1);
  
  /* 操作完成，停机 */
	while(1)
	{
	}
}

	  

