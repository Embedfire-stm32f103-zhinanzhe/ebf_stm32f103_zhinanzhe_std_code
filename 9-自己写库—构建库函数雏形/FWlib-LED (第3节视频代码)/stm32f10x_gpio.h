#ifndef __STM32F10X_GPIO_H
#define __STM32F10X_GPIO_H

#include "stm32f10x.h"

#define GPIO_Pin_0    ((uint16_t)0x0001)  /*!< 选择Pin0 */    //(00000000 00000001)b
#define GPIO_Pin_1    ((uint16_t)0x0002)  /*!< 选择Pin1 */    //(00000000 00000010)b
#define GPIO_Pin_2    ((uint16_t)0x0004)  /*!< 选择Pin2 */    //(00000000 00000100)b
#define GPIO_Pin_3    ((uint16_t)0x0008)  /*!< 选择Pin3 */    //(00000000 00001000)b
#define GPIO_Pin_4    ((uint16_t)0x0010)  /*!< 选择Pin4 */    //(00000000 00010000)b
#define GPIO_Pin_5    ((uint16_t)0x0020)  /*!< 选择Pin5 */    //(00000000 00100000)b
#define GPIO_Pin_6    ((uint16_t)0x0040)  /*!< 选择Pin6 */    //(00000000 01000000)b
#define GPIO_Pin_7    ((uint16_t)0x0080)  /*!< 选择Pin7 */    //(00000000 10000000)b

#define GPIO_Pin_8    ((uint16_t)0x0100)  /*!< 选择Pin8 */    //(00000001 00000000)b
#define GPIO_Pin_9    ((uint16_t)0x0200)  /*!< 选择Pin9 */    //(00000010 00000000)b
#define GPIO_Pin_10   ((uint16_t)0x0400)  /*!< 选择Pin10 */   //(00000100 00000000)b
#define GPIO_Pin_11   ((uint16_t)0x0800)  /*!< 选择Pin11 */   //(00001000 00000000)b
#define GPIO_Pin_12   ((uint16_t)0x1000)  /*!< 选择Pin12 */   //(00010000 00000000)b
#define GPIO_Pin_13   ((uint16_t)0x2000)  /*!< 选择Pin13 */   //(00100000 00000000)b
#define GPIO_Pin_14   ((uint16_t)0x4000)  /*!< 选择Pin14 */   //(01000000 00000000)b
#define GPIO_Pin_15   ((uint16_t)0x8000)  /*!< 选择Pin15 */   //(10000000 00000000)b
#define GPIO_Pin_All  ((uint16_t)0xFFFF)  /*!< 选择全部引脚*/ //(11111111 11111111)b


typedef enum
{ 
  GPIO_Speed_10MHz = 1,         // 10MHZ        (01)b
  GPIO_Speed_2MHz,              // 2MHZ         (10)b
  GPIO_Speed_50MHz              // 50MHZ        (11)b
}GPIOSpeed_TypeDef;

typedef enum
{ GPIO_Mode_AIN = 0x0,           // 模拟输入     (0000 0000)b
  GPIO_Mode_IN_FLOATING = 0x04,  // 浮空输入     (0000 0100)b
  GPIO_Mode_IPD = 0x28,          // 下拉输入     (0010 1000)b
  GPIO_Mode_IPU = 0x48,          // 上拉输入     (0100 1000)b
  
  GPIO_Mode_Out_OD = 0x14,       // 开漏输出     (0001 0100)b
  GPIO_Mode_Out_PP = 0x10,       // 推挽输出     (0001 0000)b
  GPIO_Mode_AF_OD = 0x1C,        // 复用开漏输出 (0001 1100)b
  GPIO_Mode_AF_PP = 0x18         // 复用推挽输出 (0001 1000)b
}GPIOMode_TypeDef;

typedef struct
{
	uint16_t GPIO_Pin;
	uint16_t GPIO_Speed;
	uint16_t GPIO_Mode;
}GPIO_InitTypeDef;




void GPIO_SetBits(GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin);
void GPIO_ResetBits( GPIO_TypeDef *GPIOx,uint16_t GPIO_Pin );
void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);

#endif /* __STM32F10X_GPIO_H */

