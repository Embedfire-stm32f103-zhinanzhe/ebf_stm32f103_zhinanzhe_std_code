#ifndef __DEBUG_USART_H
#define	__DEBUG_USART_H

#include "stm32f10x.h"
#include <stdio.h>


/** 
  * 串口宏定义，不同的串口挂载的总线不一样，移植时需要修改这几个宏
  */
#define  DEBUG_USART                   	USART1
#define  DEBUG_USART_CLK                RCC_APB2Periph_USART1
#define  DEBUG_USART_APBxClkCmd        RCC_APB2PeriphClockCmd
#define  DEBUG_USART_BAUDRATE          115200

// USART GPIO 引脚宏定义
#define  DEBUG_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)
#define  DEBUG_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  DEBUG_USART_TX_GPIO_PORT       GPIOA   
#define  DEBUG_USART_TX_GPIO_PIN        GPIO_Pin_9
#define  DEBUG_USART_RX_GPIO_PORT       GPIOA
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_Pin_10




void Debug_USART_Config(void);
//int fputc(int ch, FILE *f);

#endif /* __USART1_H */
