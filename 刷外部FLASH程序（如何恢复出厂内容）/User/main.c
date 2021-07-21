/**
  ******************************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   SPI FLASH文件系统常用功能演示
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
#include "./usart/bsp_usart.h"
#include "./led/bsp_led.h"
#include "./key/bsp_key.h"
#include "./flash/bsp_spi_flash.h"
#include "ff.h"
#include "string.h"
#include "aux_data.h"

/**
  ******************************************************************************
  *                              定义变量
  ******************************************************************************
  */
extern FATFS sd_fs;													/* Work area (file system object) for logical drives */
extern FATFS flash_fs;

//要复制的文件路径，到aux_data.c修改
extern char src_dir[];
extern char dst_dir[];

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{    	
	
  FRESULT res = FR_OK;
  
	/* 初始化LED */
  LED_GPIO_Config();
  Key_GPIO_Config();

  /* 初始化调试串口，一般为串口1 */
  USART_Config();
  TM_FATFS_FLASH_SPI_disk_initialize();
  
  res = FR_DISK_ERR;
       
  //挂载SD卡
  res = f_mount(&sd_fs,SD_ROOT,1);

  //如果文件系统挂载失败就退出
  if(res != FR_OK)
  {
    BURN_ERROR("f_mount ERROR!请给开发板插入SD卡然后重新复位开发板!");
    LED_RED;
    while(1);
  }    
    
  printf("\r\n 按一次KEY1开始烧写字库并复制文件到FLASH。 \r\n"); 
  printf("\r\n 注意该操作会把FLASH的原内容会被删除！！ \r\n"); 

  while(Key_Scan(KEY1_GPIO_PORT,KEY1_GPIO_PIN) != KEY_ON);

  printf("\r\n 正在进行整片擦除，时间很长，请耐心等候...\r\n");
  SPI_FLASH_BulkErase();   
  //烧录数据到flash的非文件系统区域    
  res = burn_file_sd2flash(burn_data,AUX_MAX_NUM); 
  
  if(res == FR_OK)
  {
    printf("\r\n\r\n\r\n"); 

    //复制文件到FLASH的文件系统区域
    copy_file_sd2flash(src_dir,dst_dir);
    if(res == FR_OK)
    {
      printf("\r\n 所有数据已成功复制到FLASH！！！ \r\n");  
      LED_GREEN;
    }
    else
    {
      printf("\r\n 复制文件到FLASH失败(文件系统部分)，请复位重试！！ \r\n"); 
    }
  }
  else
  {
    printf("\r\n 拷贝数据到FLASH失败(非文件系统部分)，请复位重试！！ \r\n"); 
  }
  
  
  while(1);

}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
