
#if 0

#include <rge52.h>

sbit LED  = P0^0

void main(void)
{
	P0 = 0XFE;     // 总线操作
	
	LED = 0;       // 位操作
}

#endif



#include "stm32f10x.h"

int main (void)
{
	// 打开 GPIOB 端口的时钟
	*( unsigned int * )0x40021018 |=  ( (1) << 3 );	

	// 配置IO口为输出
	*( unsigned int * )0x40010C00 |=  ( (1) << (4*0) );
	
	// 控制 ODR 寄存器
	*( unsigned int * )0x40010C0C &= ~(1<<0);
}


// 置位   |=   ，清0 &=~
//



// 1、点亮其他的两个LED灯

// 2、写一个简单的延时函数，让灯闪烁


// 72M，HSI = 8M；

void SystemInit(void)
{
	// 函数体为空，目的是为了骗过编译器不报错
}

