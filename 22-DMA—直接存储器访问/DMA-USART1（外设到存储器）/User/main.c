
// DMA 外设（串口）到存储器数据传输实验

#include "stm32f10x.h"
#include "bsp_usart_dma.h"
#include "bsp_led.h"
static void Delay(__IO u32 nCount);
/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
  /* 初始化USART */
  USART_Config(); 

  /* 配置使用DMA模式 */
  USARTx_DMA_Config();
  
  /* 配置RGB彩色灯 */
  LED_GPIO_Config();
  
  printf("\r\nDMA外设到存储器模式，用电脑向开发板串口发送数据，数据会返回到电脑。\r\n");
  
  /* USART1 向 DMA发出RX请求 */
  USART_DMACmd(DEBUG_USARTx, USART_DMAReq_Rx, ENABLE);
 //用电脑向开发板串口发送数据，数据会返回到电脑。
  while(1)
  {
    LED1_TOGGLE
    Delay(0xFFFFF);
  }
}
static void Delay(__IO uint32_t nCount)	 //简单的延时函数
{
	for(; nCount != 0; nCount--);
}
/*********************************************END OF FILE**********************/
