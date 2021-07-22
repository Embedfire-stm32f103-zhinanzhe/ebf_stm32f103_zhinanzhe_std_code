/**
  ******************************************************************************
  * @file    aux_data.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   用于从SD卡烧录FLASH出厂数据
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 指南者 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */  


#include "aux_data.h"
#include "ff.h"
#include "./flash/bsp_spi_flash.h"
#include "./led/bsp_led.h" 


/***************************************************************************************************************/



//SD卡源数据路径！！
char src_dir[255]="0:/srcdata";

//要拷贝到的FLASH路径
char dst_dir[255]= FLASH_ROOT;

char flash_scan_dir[255]= FLASH_ROOT;
char sd_scan_dir[255]= SD_ROOT;


//要写入的数据的信息
Aux_Data_Typedef  burn_data[] =  
{
  [AUX_XBF_XINSONGTI] =
  {
    .filename           =   "0:/srcdata/songti.xbf",
    .description        =   "XBF字库文件（emWin使用,新宋体.xbf）",
    .start_addr         =   60*4096 ,
    .length             =   317*4096, 
    .burn_option      =  UPDATE,
  },
  
  [AUX_HZLIB] =
  {
    .filename           =   "0:/srcdata/HZLIB.bin",
    .description        =   "裸机中文字库（旧版，为兼容而保留）",
    .start_addr         =   1*4096 ,
    .length             =   53*4096, 
    .burn_option      =  UPDATE,
  },
	
	 [AUX_GB2312] = 
  {
    .filename           =   "0:/srcdata/GB2312_H1616.FON",
    .description        =   "GB2312字库（修复HZLIB不支持标点的问题）",
    .start_addr         =   387*4096 ,
    .length             =   64*4096, 
    .burn_option      =  UPDATE,
  },
  
  [AUX_UNIGBK] =
  {
    .filename           =   "0:/srcdata/UNIGBK.BIN",
    .description        =   "文件系统中文支持字库(emWin使用,UNIGBK.BIN)",
    .start_addr         =   465*4096 ,
    .length             =   43*4096,
    .burn_option      =  UPDATE,      
  },      
  
  [AUX_FILE_SYSTEM] =
  {
    .filename           =   "",
    .description        =   "FATFS文件系统",
    .start_addr         =   512*4096 ,
    .length             =   1536*4096, 
    .burn_option      =  DO_NOT_UPDATE,
  }

};



static int copy_dir(char *src_path,char *dst_path);
static int copy_file(char *src_path,char *dst_path);
static FRESULT scan_files (char* path) ; 
static FRESULT copy_all (char* src_path,char* dst_path); 


/*flash及sd卡的文件系统句柄*/
FATFS flash_fs;
FATFS sd_fs;													/* Work area (file system object) for logical drives */



/*sd使用的文件系统变量*/
static FIL fnew;													/* file objects */
 
/**
  * @brief  向FLASH写入文件(写到非文件系统部分)
  * @param  dat：要写入的文件数据的信息
  * @param  file_num：要写入的文件个数
  * @retval 正常返回FR_OK
  */
FRESULT burn_file_sd2flash(Aux_Data_Typedef *dat,uint8_t file_num) 
{
    uint8_t i;
  
    FRESULT result; 
    UINT  bw;            					    /* File R/W count */


    uint32_t write_addr=0,j=0;
    uint8_t tempbuf[256],flash_buf[256];
    
//    result = f_mount(&sd_fs,SD_ROOT,1);
//    
//    //如果文件系统挂载失败就退出
//    if(result != FR_OK)
//    {
//      BURN_ERROR("f_mount ERROR!");
//      LED_RED;
//      return result;
//    }
    
    for(i=0;i<file_num;i++)
    {
       if (dat[i].burn_option == DO_NOT_UPDATE)
          continue;
       
       BURN_INFO("-------------------------------------"); 
       BURN_INFO("准备烧录内容：%s",dat[i].description);
       LED_BLUE;
       
       result = f_open(&fnew,dat[i].filename,FA_OPEN_EXISTING | FA_READ);
        if(result != FR_OK)
        {
            BURN_ERROR("打开文件失败！");
            LED_RED;
            return result;
        }
        
      BURN_INFO("正在擦除要使用的FLASH空间...");
  
      write_addr = dat[i].start_addr;
        
      for(j=0;j < dat[i].length/4096 ;j++)//擦除扇区，起始位置710*4096共2116KB
      {
        SPI_FLASH_SectorErase(write_addr+j*4096);
      }
      
      BURN_INFO("正在向FLASH写入内容...");
      
      write_addr = dat[i].start_addr;
      while(result == FR_OK) 
      {
        result = f_read( &fnew, tempbuf, 256, &bw);//读取数据	 
        if(result!=FR_OK)			 //执行错误
        {
          BURN_ERROR("读取文件失败！");
          LED_RED;
          return result;
        }      
        SPI_FLASH_PageWrite(tempbuf,write_addr,256);  //拷贝数据到外部flash上    
        write_addr+=256;				
        if(bw !=256)break;
      }

        BURN_INFO("内容写入完毕,开始校验数据...");

        //校验数据
      write_addr = dat[i].start_addr;
     
      f_lseek(&fnew,0);
      
      while(result == FR_OK) 
      {
        result = f_read( &fnew, tempbuf, 256, &bw);//读取数据	 
        if(result!=FR_OK)			 //执行错误
        {
          BURN_ERROR("读取文件失败！");
          LED_RED;
          return result;
        }      
        SPI_FLASH_BufferRead(flash_buf,write_addr,bw);  //从FLASH中读取数据
        write_addr+=bw;		
        
        for(j=0;j<bw;j++)
        {
          if(tempbuf[i] != flash_buf[i])
          {
            BURN_ERROR("数据校验失败！");
            LED_RED;
            return FR_INT_ERR;
          }
         }  
     
        if(bw !=256)break;//到了文件尾
      }
      

      BURN_INFO("数据校验成功！");
      BURN_INFO("文件：%s 写入完成",dat[i].filename);
      BURN_INFO("-------------------------------------");
      LED_BLUE;

      f_close(&fnew); 
    }
    
    
    BURN_INFO("************************************");
    BURN_INFO("所有文件均已烧录成功！（非文件系统部分）");
    return FR_OK;


}



/**
  * @brief  复制文件(若目录下有文件夹，会递归复制)
  * @param  src_path:要复制的文件(或文件夹)
  * @param  dst_path:要复制到哪个文件夹
  * @retval result:文件系统的返回值
  */
FRESULT copy_file_sd2flash(char *src_path,char *dst_path) 
{
  FRESULT result; 

  BURN_INFO("-------------------------------------"); 
  //复制文件到flash的文件系统区域    
  BURN_INFO("即将拷贝文件到FLASH的文件系统区域...");


  //挂载flash
  result = f_mount(&flash_fs,FLASH_ROOT,1);
  
  BURN_INFO("正在格式化FLASH..."); 

  //格式化FLASH
  result = f_mkfs(FLASH_ROOT,0,0);							

  //重新挂载flash
  result = f_mount(NULL,FLASH_ROOT,1);
  result = f_mount(&flash_fs,FLASH_ROOT,1);
  
  //如果文件系统挂载失败就退出
  if(result != FR_OK)
  {
    BURN_ERROR("FLASH文件系统挂载失败，请复位重试！");
    LED_RED;
    return result;
  } 
  
  BURN_INFO("*****************************************"); 
  BURN_INFO("\r\n 即将要复制的SD卡文件： \r\n"); 
  scan_files(sd_scan_dir);
  
  BURN_INFO("*****************************************"); 
  
  BURN_INFO("\r\n 开始复制： \r\n"); 
  result = copy_all(src_dir,dst_dir);   
  if(result != FR_OK)
  {
    BURN_ERROR("复制文件到FLASH失败，错误码：%d",result);
    LED_RED;
    return result;
  }   

  BURN_INFO("*****************************************"); 
  BURN_INFO("\r\n 复制后的FLASH文件(文件系统部分)： \r\n"); 
  scan_files(flash_scan_dir);

  BURN_INFO("*****************************************"); 
  BURN_INFO("所有文件均已复制成功！（文件系统部分）");
  LED_BLUE;
  
  return result;

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
        BURN_INFO("文件夹：%s",path);
        //递归遍历         
        res = scan_files(path);	
        path[i] = 0;         
        //打开失败，跳出循环        
        if (res != FR_OK) 
					break; 
      } 
			else 
			{ 
				BURN_INFO("%s/%s", path, fn);								//输出文件名	
        /* 可以在这里提取特定格式的文件路径 */        
      }//else
    } //for
  } 
  return res; 
}


#define COPY_UNIT 4096

static FIL fsrc,fdst;													/* 文件对象 */
static BYTE read_buf[COPY_UNIT]={0};        /* 读缓冲区 */
static UINT real_read_num;            					  /* 文件成功读写数量 */
static UINT real_write_num;            					  /* 文件成功读写数量 */

char newfn[255];

/**
  * @brief  复制文件
  * @param  src_path:源文件路径
  * @param  dst_path:文件将要复制到的目录(不含文件名)
  * @retval result:文件系统的返回值
  */
static int copy_file(char *src_path,char *dst_path)
{
    FRESULT res = FR_OK; 	    
    char *sub_dir = NULL;  
  
    res = f_open(&fsrc,src_path,FA_READ); 
    require_noerr(res,exit);
  
    //获取子文件路径
    sub_dir =  strrchr(src_path,'/'); 
    require_noerr(!sub_dir,exit);  
  
    //拼接成新路径
    sprintf(newfn,"%s%s",dst_path,sub_dir);
  
    BURN_INFO("正在复制文件 %s ...",newfn);
    res = f_open(&fdst,newfn,FA_CREATE_ALWAYS|FA_WRITE|FA_READ);
    require_noerr(res,exit);    
  
    //拷贝文件
    while(res == FR_OK)
    {
      res = f_read(&fsrc,&read_buf,COPY_UNIT,&real_read_num);  
      require_noerr(res,exit);
      
      res = f_write(&fdst,&read_buf,real_read_num,&real_write_num);
      require_noerr(res,exit);
      
      if(real_read_num != COPY_UNIT)
        break;
    }
    
    f_close(&fsrc);
    f_close(&fdst);
    
    BURN_INFO("复制成功！");

exit:
    return res;
}


/**
  * @brief  创建文件夹
  * @param  src_path:源文件夹路径
  * @param  dst_path:要在哪里创建该文件夹
  * @retval result:文件系统的返回值
  */
static int copy_dir(char *src_path,char *dst_path)
{
    FRESULT res; 	    
    char *sub_dir = NULL;
    
    //获取子文件夹路径
    sub_dir =  strrchr(src_path,'/'); 
    require_noerr(!sub_dir,exit);    
    
    BURN_DEBUG("将要创建的路径：%s",dst_path);
  
    //创建路径
    res = f_mkdir(dst_path); 
  
    //文件夹本来就存在,正常
    if(res == FR_EXIST)
      res = FR_OK;
  
    //检查
    require_noerr(res,exit);  

exit:
    return res;
}


//用于缓存源文件的名字
static char fntemp[_MAX_LFN*2 + 1];

/**
  * @brief  复制文件(若目录下有文件夹，会递归复制)
  * @param  src_path:要复制的文件(或文件夹)
  * @param  dst_path:要复制到哪个文件夹
  * @retval result:文件系统的返回值
  */
static FRESULT copy_all (char* src_path,char* dst_path)  
{ 
  FRESULT res; 		//部分在递归过程被修改的变量，不用全局变量	
  FILINFO fno; 
  DIR dir; 
  int i,j;            
  char *fn;        // 文件名	
	
#if _USE_LFN 
  /* 长文件名支持 */
  /* 简体中文需要2个字节保存一个“字”*/
  static char lfn[_MAX_LFN*2 + 1]; 	
  fno.lfname = lfn; 
  fno.lfsize = sizeof(lfn); 
#endif 
   
  //打开目录
  res = f_opendir(&dir, src_path); 
  if (res == FR_OK) 
	{ 
    i = strlen(src_path);//源文件路径
    j = strlen(dst_path);//目标路径
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
        sprintf(&src_path[i], "/%s", fn); 
        BURN_DEBUG("src dir=%s",src_path);
        
        sprintf(&dst_path[j], "/%s", fn);
        BURN_DEBUG("dst dir=%s",dst_path);        
        copy_dir(src_path,dst_path);

        //递归遍历         
        res = copy_all(src_path,dst_path);	
        src_path[i] = 0;   
        dst_path[j] = 0;           
        //打开失败，跳出循环        
        if (res != FR_OK) 
					break; 
      } 
			else 
			{ 
				BURN_DEBUG("%s/%s", src_path, fn);								//输出文件名	
        sprintf(fntemp,"%s/%s",src_path,fn);
        BURN_DEBUG("%s",fntemp);
        BURN_DEBUG("dst_path = %s",dst_path);
        /* 可以在这里提取特定格式的文件路径 */        
        copy_file(fntemp,dst_path);
      }//else
    } //for
  } 
  return res; 
}








/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
