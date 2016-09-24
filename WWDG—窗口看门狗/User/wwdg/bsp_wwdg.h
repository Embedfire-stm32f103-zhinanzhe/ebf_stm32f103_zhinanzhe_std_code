#ifndef __BSP_WWDG_H
#define	__BSP_WWDG_H


#include "stm32f10x.h"
// 窗口计数器值，范围为：0x40~0x7f，一般设置成最大0X7F
#define WWDG_CNT	0X7F


void WWDG_Config(uint8_t tr, uint8_t wr, uint32_t prv);
void WWDG_Feed(void);

#endif /* __BSP_WWDG_H */
