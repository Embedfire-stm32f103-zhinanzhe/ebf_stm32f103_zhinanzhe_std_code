
// DMA 存储器到外设（串口）数据传输实验

#include "stm32f10x.h"
#include "bsp_usart_dma.h"
#include "bsp_led.h"

extern uint8_t SendBuff[SENDBUFF_SIZE];
static void Delay(__IO u32 nCount); 

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
  uint16_t i;
  /* 初始化USART */
  USART_Config(); 

  /* 配置使用DMA模式 */
  USARTx_DMA_Config();
  
  /* 配置RGB彩色灯 */
  LED_GPIO_Config();

  //printf("\r\n USART1 DMA TX 测试 \r\n");
  
  /*填充将要发送的数据*/
  for(i=0;i<SENDBUFF_SIZE;i++)
  {
    SendBuff[i]	 = 'P';
    
  }

  /*为演示DMA持续运行而CPU还能处理其它事情，持续使用DMA发送数据，量非常大，
  *长时间运行可能会导致电脑端串口调试助手会卡死，鼠标乱飞的情况，
  *或把DMA配置中的循环模式改为单次模式*/		
  
  /* USART1 向 DMA发出TX请求 */
  USART_DMACmd(DEBUG_USARTx, USART_DMAReq_Tx, ENABLE);

  /* 此时CPU是空闲的，可以干其他的事情 */  
  //例如同时控制LED
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
