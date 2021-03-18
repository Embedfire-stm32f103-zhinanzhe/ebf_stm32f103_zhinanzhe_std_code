/**
  ******************************************************************************
  * @file    bsp_tempad.c
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   获取芯片温度
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-指南者 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 

//温度传感器在内部和ADC1_IN16输入通道相连接）
#include "./temp/bsp_tempad.h"

//ADC1_BASE地址：0x4000 0000+0x2400 即ADC1的基地址，而他的规则数据寄存器的偏移地址是：0x4c
#define ADC1_DR_Address    ((u32)ADC1_BASE+0x4c)

__IO u16 ADC_ConvertedValue;
//__IO u16 ADC_ConvertedValueLocal;


/*
 * 函数名：ADC1_GPIO_Config
 * 描述  ：使能ADC1和DMA1的时钟
 * 输入  :无
 * 输出  ：无
 * 调用  ：内部调用
 */
static void ADC1_GPIO_Config(void)		       
{

	/* Enable DMA clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	/* Enable ADC1 and GPIOC clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

 }


/* 函数名：ADC1_Mode_Config
 * 描述  ：配置ADC1的工作模式为DMA模式
 * 输入  : 无
 * 输出  ：无
 * 调用  ：内部调用
 */
static void ADC1_Mode_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	/* DMA channel1 configuration */
		   
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;		        // 外设基地址
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue;	        // AD转换值所存放的内存基地址	（就是给个地址）
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                        // 外设作为数据传输的来源	
  DMA_InitStructure.DMA_BufferSize = 1;                                     // 定义指定DMA通道 DMA缓存的大小
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;          // 外设地址寄存器不变
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;			        // 内存地址寄存器不变
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // 数据宽度为16位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         // HalfWord
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		                    //工作在循环模式下
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;	                    //高优先级
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;		                        //没有设置为内存到内存的传输
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  
  /* Enable DMA channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);											//ENABLE她

  /* ADC1 configuration */
  ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                        //独立工作模式
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;			                    //多通道
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;			            //连续转换
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;       //由软件触发启动
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;					//Right
  ADC_InitStructure.ADC_NbrOfChannel = 1;				                    //仅一个通道转换
  ADC_Init(ADC1, &ADC_InitStructure);
	
  	/*配置ADC时钟，为PCLK2的8分频，即9Hz*/
	RCC_ADCCLKConfig(RCC_PCLK2_Div8); 

  /* ADC1 regular channel16 configuration */ 
  //设置采样通道IN16, 设置采样时间
  ADC_RegularChannelConfig(ADC1, ADC_Channel_16, 1, ADC_SampleTime_239Cycles5);	

  //使能温度传感器和内部参考电压   
  ADC_TempSensorVrefintCmd(ENABLE);                                    

   /* Enable ADC1 DMA */	  
  ADC_DMACmd(ADC1, ENABLE);
  
  /* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);

  /* Enable ADC1 reset calibaration register */   
  ADC_ResetCalibration(ADC1);			                              
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));                         

  /* Start ADC1 calibaration */
  ADC_StartCalibration(ADC1);				                        
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));	  
     
  /* Start ADC1 Software Conversion */ 
  ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}												

/*
 * 函数名：Temp_ADC1_Init
 * 描述  ：无
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
void Temp_ADC1_Init(void)
{
	ADC1_GPIO_Config();
	ADC1_Mode_Config();
}
