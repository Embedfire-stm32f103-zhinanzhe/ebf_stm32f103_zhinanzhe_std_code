#ifndef __PWM_BREATHING_H
#define	__PWM_BREATHING_H

#include "stm32f10x.h"

/*PWM表中的点数*/
extern uint16_t  POINT_NUM	;
//控制输出波形的频率
extern __IO uint16_t period_class ;


#define RED_LIGHT 		1
#define GREEN_LIGHT 	2
#define BLUE_LIGHT		3

/*要使用什么颜色的呼吸灯，可选RED_LIGHT、GREEN_LIGHT、BLUE_LIGHT*/
#define LIGHT_COLOR 	RED_LIGHT

/********************定时器通道**************************/
#if  LIGHT_COLOR == RED_LIGHT
/************红灯***************/
	#define   BRE_TIMx                      TIM3

	#define   BRE_TIM_APBxClock_FUN        RCC_APB1PeriphClockCmd
	#define   BRE_TIM_CLK                   RCC_APB1Periph_TIM3
	#define   BRE_TIM_GPIO_APBxClock_FUN   RCC_APB2PeriphClockCmd
	#define   BRE_TIM_GPIO_CLK              (RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO)

	//红灯的引脚需要重映射
	#define   BRE_GPIO_REMAP_FUN()						GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); 				

	#define  BRE_TIM_LED_PORT               GPIOB
	#define  BRE_TIM_LED_PIN                GPIO_Pin_5

	#define  BRE_TIM_OCxInit                TIM_OC2Init            //通道选择，1~4
	#define  BRE_TIM_OCxPreloadConfig       TIM_OC2PreloadConfig 
	#define  BRE_CCRx                       CCR2

	#define   BRE_TIMx_IRQn                TIM3_IRQn              //中断
	#define   BRE_TIMx_IRQHandler          TIM3_IRQHandler

#elif LIGHT_COLOR == GREEN_LIGHT
/************绿灯***************/
	#define   BRE_TIMx                      TIM3

	#define   BRE_TIM_APBxClock_FUN        RCC_APB1PeriphClockCmd
	#define   BRE_TIM_CLK                   RCC_APB1Periph_TIM3
	#define   BRE_TIM_GPIO_APBxClock_FUN   RCC_APB2PeriphClockCmd
	#define   BRE_TIM_GPIO_CLK              (RCC_APB2Periph_GPIOB)

	//绿灯不需要重映射
	#define   BRE_GPIO_REMAP_FUN()				


	#define  BRE_TIM_LED_PORT               GPIOB
	#define  BRE_TIM_LED_PIN                GPIO_Pin_0

	#define  BRE_TIM_OCxInit                TIM_OC3Init            //通道选择，1~4
	#define  BRE_TIM_OCxPreloadConfig       TIM_OC3PreloadConfig 
	#define  BRE_CCRx                       CCR3

	#define   BRE_TIMx_IRQn                TIM3_IRQn              //中断
	#define   BRE_TIMx_IRQHandler          TIM3_IRQHandler

#elif LIGHT_COLOR == BLUE_LIGHT
/************蓝灯***************/
	#define   BRE_TIMx                      TIM3

	#define   BRE_TIM_APBxClock_FUN        RCC_APB1PeriphClockCmd
	#define   BRE_TIM_CLK                   RCC_APB1Periph_TIM3
	#define   BRE_TIM_GPIO_APBxClock_FUN   RCC_APB2PeriphClockCmd
	#define   BRE_TIM_GPIO_CLK              (RCC_APB2Periph_GPIOB)

	//蓝灯不需要重映射
	#define   BRE_GPIO_REMAP_FUN()	

	#define   BRE_TIM_LED_PORT             GPIOB
	#define   BRE_TIM_LED_PIN              GPIO_Pin_1

	#define   BRE_TIM_OCxInit              TIM_OC4Init            //通道选择，1~4
	#define   BRE_TIM_OCxPreloadConfig    TIM_OC4PreloadConfig 
	#define   BRE_CCRx                      CCR4

	#define   BRE_TIMx_IRQn                TIM3_IRQn              //中断
	#define   BRE_TIMx_IRQHandler          TIM3_IRQHandler
	

#endif



void      TIMx_Breathing_Init          (void);



#endif /* __PWM_BREATHING_H */
