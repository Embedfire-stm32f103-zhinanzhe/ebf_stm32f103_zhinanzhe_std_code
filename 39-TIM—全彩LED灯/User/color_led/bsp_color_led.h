#ifndef __COLOR_LED_H
#define	__COLOR_LED_H


#include "stm32f10x.h"



/********************定时器通道**************************/

#define   COLOR_TIMx                      TIM3

#define   COLOR_TIM_APBxClock_FUN        RCC_APB1PeriphClockCmd
#define   COLOR_TIM_CLK                   RCC_APB1Periph_TIM3
#define   COLOR_TIM_GPIO_CLK              (RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO)

//红灯的引脚需要重映射
#define   COLOR_GPIO_REMAP_FUN()						GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); 				

/************红灯***************/
#define  COLOR_RED_TIM_LED_PORT               GPIOB
#define  COLOR_RED_TIM_LED_PIN                GPIO_Pin_5

#define  COLOR_RED_TIM_OCxInit                TIM_OC2Init            //通道初始化函数
#define  COLOR_RED_TIM_OCxPreloadConfig       TIM_OC2PreloadConfig //通道重载配置函数

//通道比较寄存器，以TIMx->CCRx方式可访问该寄存器，设置新的比较值，控制占空比
//以宏封装后，使用这种形式：COLOR_TIMx->COLOR_RED_CCRx ，可访问该通道的比较寄存器
#define  COLOR_RED_CCRx                       	CCR2		

/************绿灯***************/

#define  COLOR_GREEN_TIM_LED_PORT               GPIOB
#define  COLOR_GREEN_TIM_LED_PIN                GPIO_Pin_0

#define  COLOR_GREEN_TIM_OCxInit                TIM_OC3Init            //通道初始化函数
#define  COLOR_GREEN_TIM_OCxPreloadConfig       TIM_OC3PreloadConfig //通道重载配置函数

//通道比较寄存器，以TIMx->CCRx方式可访问该寄存器，设置新的比较值，控制占空比
//以宏封装后，使用这种形式：COLOR_TIMx->COLOR_GREEN_CCRx ，可访问该通道的比较寄存器
#define  COLOR_GREEN_CCRx                       CCR3

/************蓝灯***************/
#define   COLOR_BLUE_TIM_LED_PORT             GPIOB
#define   COLOR_BLUE_TIM_LED_PIN              GPIO_Pin_1

#define   COLOR_BLUE_TIM_OCxInit              TIM_OC4Init            //通道初始化函数
#define   COLOR_BLUE_TIM_OCxPreloadConfig    TIM_OC4PreloadConfig  //通道重载配置函数

//通道比较寄存器，以TIMx->CCRx方式可访问该寄存器，设置新的比较值，控制占空比
//以宏封装后，使用这种形式：COLOR_TIMx->COLOR_BLUE_CCRx ，可访问该通道的比较寄存器
#define   COLOR_BLUE_CCRx                      CCR4



void COLOR_TIMx_LED_Init(void);
void COLOR_TIMx_LED_Close(void);
void SetRGBColor(uint32_t rgb);
void SetColorValue(uint8_t r,uint8_t g,uint8_t b);


#endif /* __COLOR_LED_H */

