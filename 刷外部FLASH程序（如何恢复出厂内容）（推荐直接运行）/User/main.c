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
#include "./led/bsp_led.h" 
#include "./usart/bsp_usart.h"
#include "./spi_flash/bsp_flash_spi.h"
#include "./key/bsp_key.h" 
//#include "./delay/core_delay.h"  
/* FatFs includes component */
#include "ff.h"
#include "./res_mgr/RES_MGR.h"

/**
  ******************************************************************************
  *                              定义变量
  ******************************************************************************
  */
DIR dir; 
FIL fnew;													/* 文件对象 */
UINT fnum;            			  /* 文件成功读写数量 */
BYTE ReadBuffer[1024]={0};        /* 读缓冲区 */
BYTE WriteBuffer[] =              /* 写缓冲区*/
"欢迎使用野火STM32 F103开发板 今天是个好日子，新建文件系统测试文件\r\n";  
char SDPath[4]; /* SD逻辑驱动器路径 */
extern FATFS sd_fs;	
FRESULT res_sd;                /* 文件操作结果 */
extern char src_dir[];
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
uint8_t state = 0;
int main(void)
{
    /* 系统时钟初始化成400MHz */
  
    LED_GPIO_Config();
    LED_BLUE;	
    /* 初始化USART1 配置模式为 115200 8-N-1 */
    USART_Config();	
  
    Key_GPIO_Config();
    SPI_FLASH_Init();
  
    //在外部SD卡挂载文件系统，文件系统挂载时会对SD卡初始化
    res_sd = f_mount(&sd_fs,"0:",1);  
  
    if(res_sd != FR_OK)
    {
      printf("f_mount ERROR!请给开发板插入SD卡然后重新复位开发板!");
      LED_RED;
      while(1);
    }
  
#if 1    
    printf("\r\n 按一次KEY1开始烧写字库并复制文件到FLASH。 \r\n"); 
    printf("\r\n 注意该操作会把FLASH的原内容会被删除！！ \r\n"); 

    while(Key_Scan(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == KEY_OFF);
    printf("\r\n 正在进行整片擦除，时间很长，请耐心等候...\r\n"); 
    
    SPI_FLASH_BulkErase();
    /* 第一次执行Make_Catalog函数时删除旧的烧录信息文件 */
    res_sd = f_unlink(BURN_INFO_NAME_FULL);
    if ( res_sd == FR_OK )
    {
      printf("！！删除文件成功：(%d)\n",res_sd);
    }
    else
    {
      printf("！！删除文件失败：(%d)\n",res_sd);
    }
    
    /* 生成烧录目录信息文件 */
    Make_Catalog(src_dir,0);

    /* 烧录 目录信息至FLASH*/
    Burn_Catalog();
     /* 根据 目录 烧录内容至FLASH*/
    Burn_Content();
    /* 校验烧录的内容 */
    Check_Resource(); 
#endif    
    /* 操作完成，停机 */
    while(1)
    {
    }
}
	  

