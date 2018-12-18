/**
  ******************************************************************************
  * @file    bsp_internalFlash.c
  * @author  fire
  * @version V1.0
  * @date    2015-xx-xx
  * @brief   内部FLASH读写保护测试范例
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火  STM32 指南者 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
  
#include "./protect/bsp_readWriteProtect.h"   
#include "./usart/bsp_usart.h"



/**
  * @brief  反转写保护的配置，用于演示
	          若芯片处于写保护状态，则解除，
						若不是写保护状态，则设置成写保护
  * @param  无
  * @retval 无
  */
void WriteProtect_Toggle(void)
{
	/* 获取写保护寄存器的值进行判断，寄存器位为0表示有保护，为1表示无保护 */
	/*  若不等于0xFFFFFFFF，则说明有部分页被写保护了 */
	if(FLASH_GetWriteProtectionOptionByte() != 0xFFFFFFFF )
	{
			FLASH_DEBUG("芯片处于写保护状态，即将执行解保护过程...");
			
			//解除对FLASH_CR寄存器的访问限制
			FLASH_Unlock();

			/* 擦除所有选项字节的内容 */
			FLASH_EraseOptionBytes();

			/* 对所有页解除 */
			FLASH_EnableWriteProtection(0x00000000);
			
			FLASH_DEBUG("配置完成，芯片将自动复位加载新配置，复位后芯片会解除写保护状态\r\n");

			/* 复位芯片，以使选项字节生效 */
			NVIC_SystemReset();
	}
	else //无写保护
	{
			FLASH_DEBUG("芯片处于无写保护状态，即将执行写保护过程...");
			
			//解除对FLASH_CR寄存器的访问限制
			FLASH_Unlock();

			/* 先擦除所有选项字节的内容，防止因为原有的写保护导致无法写入新的保护配置 */
			FLASH_EraseOptionBytes();

			/* 对所有页进行写保护 */
			FLASH_EnableWriteProtection(FLASH_WRProt_AllPages);

			FLASH_DEBUG("配置完成，芯片将自动复位加载新配置，复位后芯片会处于写保护状态\r\n");

			/* 复位芯片，以使选项字节生效 */
			NVIC_SystemReset();		
	}

}


/**
  * @brief  反转读保护的配置，用于演示
	          若芯片处于读保护状态，则解除，
						若不是读保护状态，则设置成读保护
  * @param  无
  * @retval 无
  */
void ReadProtect_Toggle(void)
{
	if(FLASH_GetReadOutProtectionStatus () == SET )
	{
		FLASH_DEBUG("芯片处于读保护状态\r\n");
		
		//解除对FLASH_CR寄存器的访问限制
		FLASH_Unlock();
		
		FLASH_DEBUG("即将解除读保护，解除读保护会把FLASH的所有内容清空");
		FLASH_DEBUG("由于解除后程序被清空，所以后面不会有任何提示输出");
		FLASH_DEBUG("等待20秒后即可给芯片下载新的程序...\r\n");
		
		FLASH_ReadOutProtection (DISABLE);		

		//即使在此处加入printf串口调试也不会执行的，因为存储程序的整片FLASH都已被擦除。
		FLASH_DEBUG("由于FLASH程序被清空，所以本代码不会被执行，串口不会有本语句输出（SRAM调试模式下例外）\r\n");

	}
	else
	{
		FLASH_DEBUG("芯片处于无读保护状态，即将执行读保护过程...\r\n");
		
		//解除对FLASH_CR寄存器的访问限制
		FLASH_Unlock();				

		FLASH_ReadOutProtection (ENABLE);
		
		printf("芯片已被设置为读保护，上电复位后生效（必须重新给开发板上电，只按复位键无效）\r\n");
		printf("处于保护状态下无法正常下载新程序，必须要先解除保护状态再下载\r\n");

	}
}






