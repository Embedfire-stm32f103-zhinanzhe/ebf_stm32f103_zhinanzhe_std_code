#include "stm32f10x.h"

// 这里只定义了 GPIO ODR和IDR这两个寄存器的位带别名区地址，其他寄存器的没有定义

//SRAM 位带区:    0X2000 0000~0X2010 0000
//SRAM 位带别名区:0X2200 0000~0X23FF FFFF

//外设 位带区:    0X4000 0000~0X4010 0000
//外设 位带别名区:0X4200 0000~0X43FF FFFF

// 把“位带地址+位序号”转换成别名地址的宏
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x02000000+((addr & 0x00FFFFFF)<<5)+(bitnum<<2)) 
/*
 *addr & 0xF0000000，取地址的高4位，看看是2还是4，用于区分SRAM和外设地址，
 *如果是2，+0x02000000则=0X2200 0000，即是SRAM，如果是4，+0x02000000则=0X4200 0000，即是外设
 *
 *addr & 0x000FFFFFF，屏蔽掉高两位，相当于-0X2000 0000或者-0X4000 0000，结果表示偏移位带区多少个字节
 *<<5  等于*8*4，因为位带区一个地址表示一个字节，一个字节有8个bit，一个bit可以膨胀成一个字，即4个字节
 *<<2 等于*4，因为一个位可以膨胀成一个字，即4个字节
 *
 *分解成两条公式应该就是这样：
 *SRAM位带别名地址
 *AliasAddr= 0x22000000+((A-0x20000000)*8+n)*4 =0x22000000+ (A-0x20000000)*8*4 +n*4
 *外设位带别名地址
 *AliasAddr= 0x22000000+((A-0x20000000)*8+n)*4 =0x22000000+ (A-0x20000000)*8*4 +n*4
 */


// 把一个地址转换成一个指针
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 

// 把位带别名区地址转换成指针
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))   

// GPIO ODR 和 IDR 寄存器地址映射 
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C   
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C   
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C   
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C   
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C   
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C      
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C      
  
#define GPIOA_IDR_Addr    (GPIOA_BASE+8)  //0x40010808   
#define GPIOB_IDR_Addr    (GPIOB_BASE+8)  //0x40010C08   
#define GPIOC_IDR_Addr    (GPIOC_BASE+8)  //0x40011008   
#define GPIOD_IDR_Addr    (GPIOD_BASE+8)  //0x40011408   
#define GPIOE_IDR_Addr    (GPIOE_BASE+8)  //0x40011808   
#define GPIOF_IDR_Addr    (GPIOF_BASE+8)  //0x40011A08   
#define GPIOG_IDR_Addr    (GPIOG_BASE+8)  //0x40011E08 


// 单独操作 GPIO的某一个IO口，n(0,1,2...16),n表示具体是哪一个IO口
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出   
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入   
  
#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出   
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入   
  
#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出   
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入   
  
#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出   
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入   
  
#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出   
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入  
  
#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出   
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入  
  
#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出   
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入  

void SOFT_Delay(__IO uint32_t nCount);	
void LED_GPIO_Config(void);

int main(void)
{	
	// 程序来到main函数之前，启动文件：statup_stm32f10x_hd.s已经调用
	// SystemInit()函数把系统时钟初始化成72MHZ
	// SystemInit()在system_stm32f10x.c中定义
	// 如果用户想修改系统时钟，可自行编写程序修改
	
	LED_GPIO_Config();
	
	while( 1 )
	{
		// PB0 = 0,点亮LED
		PBout(0)= 0;		
		SOFT_Delay(0x0FFFFF);
		
		// PB0 = 1,熄灭LED		
		PBout(0)= 1;
		SOFT_Delay(0x0FFFFF);		
    
    // PB1 = 0,点亮LED
		PBout(1)= 0;		
		SOFT_Delay(0x0FFFFF);
		
		// PB1 = 1,熄灭LED		
		PBout(1)= 1;
		SOFT_Delay(0x0FFFFF);	
    
    // PB5 = 0,点亮LED
		PBout(5)= 0;		
		SOFT_Delay(0x0FFFFF);
		
		// PB5 = 1,熄灭LED		
		PBout(5)= 1;
		SOFT_Delay(0x0FFFFF);	
	}
}

	

void LED_GPIO_Config(void)
{		
		// 定义一个GPIO_InitTypeDef类型的结构体
		GPIO_InitTypeDef GPIO_InitStructure;

		// 开启GPIOB的时钟
		RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 

		// 选择要控制的IO口													   
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_5|GPIO_Pin_1;	

		// 设置引脚为推挽输出
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   

		// 设置引脚速率为50MHz
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 

		/*调用库函数，初始化GPIOB0*/
		GPIO_Init(GPIOB, &GPIO_InitStructure);		  

		// 关闭LED
		GPIO_SetBits(GPIOB, GPIO_Pin_0); 
    GPIO_SetBits(GPIOB, GPIO_Pin_5); 
    GPIO_SetBits(GPIOB, GPIO_Pin_1); 
}

// 简陋的软件延时函数
void SOFT_Delay(__IO uint32_t nCount)	
{
	for(; nCount != 0; nCount--);
}
