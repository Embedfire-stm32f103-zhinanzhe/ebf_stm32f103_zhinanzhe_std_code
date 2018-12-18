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
#include "ff.h"
#include "string.h"
/**
  ******************************************************************************
  *                              定义变量
  ******************************************************************************
  */
FATFS fs;													/* FatFs文件系统对象 */
FIL fnew;													/* 文件对象 */
FRESULT res_flash;                /* 文件操作结果 */
UINT fnum;            					  /* 文件成功读写数量 */
char fpath[100];                  /* 保存当前扫描路径 */
char readbuffer[512];             
 
/**
  ******************************************************************************
  *                                任务函数
  ******************************************************************************
  */
/* FatFs多项功能测试 */
static FRESULT miscellaneous(void)
{
  DIR dir;
  FATFS *pfs;
  DWORD fre_clust, fre_sect, tot_sect;
  
  printf("\n*************** 设备信息获取 ***************\r\n");
  /* 获取设备信息和空簇大小 */
  res_flash = f_getfree("1:", &fre_clust, &pfs);

  /* 计算得到总的扇区个数和空扇区个数 */
  tot_sect = (pfs->n_fatent - 2) * pfs->csize;
  fre_sect = fre_clust * pfs->csize;

  /* 打印信息(4096 字节/扇区) */
  printf("》设备总空间：%10lu KB。\n》可用空间：  %10lu KB。\n", tot_sect *4, fre_sect *4);
  
  printf("\n******** 文件定位和格式化写入功能测试 ********\r\n");
  res_flash = f_open(&fnew, "1:FatFs读写测试文件.txt",
                            FA_OPEN_ALWAYS|FA_WRITE|FA_READ );
	if ( res_flash == FR_OK )
	{
    /*  文件定位 */
    res_flash = f_lseek(&fnew,f_size(&fnew));
    if (res_flash == FR_OK)
    {
      /* 格式化写入，参数格式类似printf函数 */
      f_printf(&fnew,"\n在原来文件新添加一行内容\n");
      f_printf(&fnew,"》设备总空间：%10lu KB。\n》可用空间：  %10lu KB。\n", tot_sect *4, fre_sect *4);
      /*  文件定位到文件起始位置 */
      res_flash = f_lseek(&fnew,0);
      /* 读取文件所有内容到缓存区 */
      res_flash = f_read(&fnew,readbuffer,f_size(&fnew),&fnum);
      if(res_flash == FR_OK)
      {
        printf("》文件内容：\n%s\n",readbuffer);
      }
    }
    f_close(&fnew);    
    
    printf("\n********** 目录创建和重命名功能测试 **********\r\n");
    /* 尝试打开目录 */
    res_flash=f_opendir(&dir,"1:TestDir");
    if(res_flash!=FR_OK)
    {
      /* 打开目录失败，就创建目录 */
      res_flash=f_mkdir("1:TestDir");
    }
    else
    {
      /* 如果目录已经存在，关闭它 */
      res_flash=f_closedir(&dir);
      /* 删除文件 */
      f_unlink("1:TestDir/testdir.txt");
    }
    if(res_flash==FR_OK)
    {
      /* 重命名并移动文件 */
      res_flash=f_rename("1:FatFs读写测试文件.txt","1:TestDir/testdir.txt");      
    } 
	}
  else
  {
    printf("!! 打开文件失败：%d\n",res_flash);
    printf("!! 或许需要再次运行“FatFs移植与读写测试”工程\n");
  }
  return res_flash;
}

  FILINFO fno;

/**
  * 文件信息获取
  */
static FRESULT file_check(void)
{
  
  /* 获取文件信息 */
  res_flash=f_stat("1:TestDir/testdir.txt",&fno);
  if(res_flash==FR_OK)
  {
    printf("“testdir.txt”文件信息：\n");
    printf("》文件大小: %ld(字节)\n", fno.fsize);
    printf("》时间戳: %u/%02u/%02u, %02u:%02u\n",
           (fno.fdate >> 9) + 1980, fno.fdate >> 5 & 15, fno.fdate & 31,fno.ftime >> 11, fno.ftime >> 5 & 63);
    printf("》属性: %c%c%c%c%c\n\n",
           (fno.fattrib & AM_DIR) ? 'D' : '-',      // 是一个目录
           (fno.fattrib & AM_RDO) ? 'R' : '-',      // 只读文件
           (fno.fattrib & AM_HID) ? 'H' : '-',      // 隐藏文件
           (fno.fattrib & AM_SYS) ? 'S' : '-',      // 系统文件
           (fno.fattrib & AM_ARC) ? 'A' : '-');     // 档案文件
  }
  return res_flash;
}

/**
  * @brief  scan_files 递归扫描FatFs内的文件
  * @param  path:初始扫描路径
  * @retval result:文件系统的返回值
  */
static FRESULT scan_files (char* path) 
{ 
  FRESULT res; 		//部分在递归过程被修改的变量，不用全局变量	
  FILINFO fno; 
  DIR dir; 
  int i;            
  char *fn;        // 文件名	
	
#if _USE_LFN 
  /* 长文件名支持 */
  /* 简体中文需要2个字节保存一个“字”*/
  static char lfn[_MAX_LFN*2 + 1]; 	
  fno.lfname = lfn; 
  fno.lfsize = sizeof(lfn); 
#endif 
  //打开目录
  res = f_opendir(&dir, path); 
  if (res == FR_OK) 
	{ 
    i = strlen(path); 
    for (;;) 
		{ 
      //读取目录下的内容，再读会自动读下一个文件
      res = f_readdir(&dir, &fno); 								
      //为空时表示所有项目读取完毕，跳出
      if (res != FR_OK || fno.fname[0] == 0) break; 	
#if _USE_LFN 
      fn = *fno.lfname ? fno.lfname : fno.fname; 
#else 
      fn = fno.fname; 
#endif 
      //点表示当前目录，跳过			
      if (*fn == '.') continue; 	
      //目录，递归读取      
      if (fno.fattrib & AM_DIR)         
			{ 			
        //合成完整目录名        
        sprintf(&path[i], "/%s", fn); 		
        //递归遍历         
        res = scan_files(path);	
        path[i] = 0;         
        //打开失败，跳出循环        
        if (res != FR_OK) 
					break; 
      } 
			else 
			{ 
				printf("%s/%s\r\n", path, fn);								//输出文件名	
        /* 可以在这里提取特定格式的文件路径 */        
      }//else
    } //for
  } 
  return res; 
}
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{    	
	/* 初始化调试串口，一般为串口1 */
	USART_Config();	
  printf("******** 这是一个SPI FLASH 文件系统实验 *******\r\n");
  
	//在外部SPI Flash挂载文件系统，文件系统挂载时会对SPI设备初始化
	res_flash = f_mount(&fs,"1:",1);
  if(res_flash!=FR_OK)
  {
    printf("！！外部Flash挂载文件系统失败。(%d)\r\n",res_flash);
    printf("！！可能原因：SPI Flash初始化不成功。\r\n");
		while(1);
  }
  else
  {
    printf("》文件系统挂载成功，可以进行测试\r\n");    
  }
  
  /* FatFs多项功能测试 */
  res_flash = miscellaneous();

  
  printf("\n*************** 文件信息获取测试 **************\r\n");
  res_flash = file_check();

  
  printf("***************** 文件扫描测试 ****************\r\n");
  strcpy(fpath,"1:");
  scan_files(fpath);
  
  
	/* 不再使用文件系统，取消挂载文件系统 */
	f_mount(NULL,"1:",1);
  
  /* 操作完成，停机 */
	while(1)
	{
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
