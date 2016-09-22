#ifndef __BSP_LED_H
#define	__BSP_LED_H


#include "stm32f10x.h"


/* 定义LED连接的GPIO端口, 用户只需要修改下面的代码即可改变控制的LED引脚 */
#define LED1_GPIO_PORT    	GPIOB			              /* GPIO端口 */
#define LED1_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define LED1_GPIO_PIN		    GPIO_Pin_5			          /* 连接到SCL时钟线的GPIO */

#define LED2_GPIO_PORT    	GPIOB			              /* GPIO端口 */
#define LED2_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define LED2_GPIO_PIN		    GPIO_Pin_1			          /* 连接到SCL时钟线的GPIO */

#define LED3_GPIO_PORT    	GPIOB			              /* GPIO端口 */
#define LED3_GPIO_CLK 	    RCC_APB2Periph_GPIOB		/* GPIO端口时钟 */
#define LED3_GPIO_PIN		    GPIO_Pin_0			          /* 连接到SCL时钟线的GPIO */


/** the macro definition to trigger the led on or off 
  * 1 - off
  *0 - on
  */
#define ON  0
#define OFF 1

/* 带参宏，可以像内联函数一样使用 */
#define LED1(a)	if (a)	\
					GPIO_SetBits(LED1_GPIO_PORT,LED1_GPIO_PIN);\
					else		\
					GPIO_ResetBits(LED1_GPIO_PORT,LED1_GPIO_PIN)

#define LED2(a)	if (a)	\
					GPIO_SetBits(LED2_GPIO_PORT,LED2_GPIO_PIN);\
					else		\
					GPIO_ResetBits(LED2_GPIO_PORT,LED2_GPIO_PIN)

#define LED3(a)	if (a)	\
					GPIO_SetBits(LED2_GPIO_PORT,LED3_GPIO_PIN);\
					else		\
					GPIO_ResetBits(LED2_GPIO_PORT,LED3_GPIO_PIN)


/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)			{p->BSRR=i;}			//设置为高电平		
#define digitalLo(p,i)			{p->BRR=i;}				//输出低电平
#define digitalToggle(p,i)		{p->ODR ^=i;}			//输出反转状态


/* 定义控制IO的宏 */
#define LED1_TOGGLE		digitalToggle(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_OFF		digitalHi(LED1_GPIO_PORT,LED1_GPIO_PIN)
#define LED1_ON			digitalLo(LED1_GPIO_PORT,LED1_GPIO_PIN)

#define LED2_TOGGLE		digitalToggle(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED2_OFF		digitalHi(LED2_GPIO_PORT,LED2_GPIO_PIN)
#define LED2_ON			digitalLo(LED2_GPIO_PORT,LED2_GPIO_PIN)

#define LED3_TOGGLE		digitalToggle(LED2_GPIO_PORT,LED3_GPIO_PIN)
#define LED3_OFF		digitalHi(LED2_GPIO_PORT,LED3_GPIO_PIN)
#define LED3_ON			digitalLo(LED2_GPIO_PORT,LED3_GPIO_PIN)

void LED_GPIO_Config(void);

#endif /* __BSP_LED_H */
