#ifndef __TEMPADC_H
#define	__TEMPADC_H


#include "stm32f10x.h"

//对于12位的ADC，3.3V的ADC值为0xfff,温度为25度时对应的电压值为1.43V即0x6EE
#define V25  0x6EE	 

//斜率 每摄氏度4.3mV 对应每摄氏度0x05
#define AVG_SLOPE 0x05 

void Temp_ADC1_Init(void);


#endif /* __TEMPADC_H */
