/**
  ******************************************************************************
  * @file    bsp_led.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   usb mass storage 
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 F103-指南者 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
#include "stm32f10x.h"
#include "./usart/bsp_usart.h"   
#include "./led/bsp_led.h"
#include "hw_config.h" 
#include "usb_lib.h"
#include "usb_pwr.h" 
/*-------------   例程使用说明 ------------------*/

//本工程仅适用已插入SD卡用户，如果没有SD卡请使用名为：
//（2.STM32-F103-指南者_USB Mass Storage_flash）工程，不然在
//没有SD卡情况下使用本工程会导致模拟U盘速度非常慢。

/*---------------------------------------------*/
void USB_Delay(__IO uint32_t nCount)
{
	for(; nCount != 0; nCount--);
}

int main(void)
{
	 /* USART config */
	USART_Config();
	
  LED_GPIO_Config();
  
	/*初始化*/
	Set_System();
  	
	/*设置USB时钟为48M*/
	Set_USBClock();
 	
	/*配置USB中断(包括SDIO中断)*/
	USB_Interrupts_Config();
 
	/*USB初始化*/
 	USB_Init();
 
 	while (bDeviceState != CONFIGURED);	 //等待配置完成
	   
	printf("\r\n 野火 F103-指南者 STM32 USB MASS STORAGE 实验\r\n");
	 
  while (1)
  {
    LED2_TOGGLE;
    USB_Delay(0x0FFFFF);
  }
}

/* -------------------------------------end of file -------------------------------------------- */
