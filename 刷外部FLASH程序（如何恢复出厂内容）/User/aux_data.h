#ifndef __AUX_DATA_H
#define	__AUX_DATA_H

#include "stm32f10x.h"
#include "ff.h"
#include "string.h"


//SD卡及flash的根目录
#define SD_ROOT       "0:"
#define FLASH_ROOT    "1:"

typedef enum 
{
    UPDATE,
    DO_NOT_UPDATE
}Burn_Option_Typedef;

typedef struct
{
  const char*               filename;      //从SD卡拷录数据时的文件路径
  const char*               description;   //描述
  uint32_t                  start_addr;    //开始地址
  uint32_t                  length;        //长度
  Burn_Option_Typedef     burn_option;   
  
}Aux_Data_Typedef;

typedef enum 
{
    AUX_DATA_ERROR = -1,
  
    AUX_XBF_XINSONGTI, 
    AUX_HZLIB, 
		AUX_GB2312,
    AUX_UNIGBK, 
    AUX_FILE_SYSTEM,      //FLASH文件系统
  
    AUX_MAX_NUM,
} aux_data_t; 

extern  Aux_Data_Typedef  burn_data[AUX_MAX_NUM];


/*信息输出*/
#define BURN_DEBUG_ON         0
#define BURN_DEBUG_FUNC_ON    0

#define BURN_INFO(fmt,arg...)           printf("<<-BURN-INFO->> "fmt"\n",##arg)
#define BURN_ERROR(fmt,arg...)          printf("<<-BURN-ERROR->> "fmt"\n",##arg)
#define BURN_DEBUG(fmt,arg...)          do{\
                                          if(BURN_DEBUG_ON)\
                                          printf("<<-BURN-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)

#define BURN_DEBUG_FUNC()               do{\
                                         if(BURN_DEBUG_FUNC_ON)\
                                         printf("<<-BURN-FUNC->> Func:%s@Line:%d\n",__func__,__LINE__);\
                                       }while(0)


#define debug_print_assert(A,B,C,D,E,F) do {if (BURN_DEBUG_ON)\
                                                     printf("\r\nerror code = %d,[occur:%s:%s:%4d] **ASSERT** %s""\r\n",A, D, F, E, (C!=NULL) ? C : "" );\
                                                     }while(0==1)
   
                                       
                                       
 // ==== LOGGING ====
#define SHORT_FILE strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__
 
// ==== BRANCH PREDICTION & EXPRESSION EVALUATION ====
#if( !defined( unlikely ) )
    //#define unlikely( EXPRESSSION )     __builtin_expect( !!(EXPRESSSION), 0 )
   #define unlikely( EXPRESSSION )     !!(EXPRESSSION)
#endif
  

//用于检查参数，err非0就跳转到LABEL处                                                     
#if( !defined( check ) )                                       
#define require_noerr( ERR, LABEL )                                                                     \
    do                                                                                                  \
    {                                                                                                   \
        FRESULT        localErr;                                                                       \
                                                                                                        \
        localErr = (FRESULT)(ERR);                                                                     \
        if( unlikely( localErr != 0 ) )                                                                 \
        {                                                                                               \
            debug_print_assert( localErr, NULL, NULL, SHORT_FILE, __LINE__, __PRETTY_FUNCTION__ );        \
            goto LABEL;                                                                                 \
        }                                                                                               \
                                                                                                        \
    }   while( 1==0 )                                       
#endif                                      
                                       
                                       
                                       

FRESULT burn_file_sd2flash(Aux_Data_Typedef *dat,uint8_t file_num);
FRESULT copy_file_sd2flash(char *src_path,char *dst_path); 

#endif /* __BURN_DATA_H */
