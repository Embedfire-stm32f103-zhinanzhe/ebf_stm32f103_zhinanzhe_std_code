/**
  ******************************************************************************
  * @file    main.c
  * @author  LYC
  * @version V1.0
  * @date    2014-04-22
  * @brief   MPU6050 软件IIC测试
  ******************************************************************************
  * @attention
  * 实验平台:野火 霸道 STM32 开发板 
  ******************************************************************************
  */
  
#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "./systick/bsp_SysTick.h"
#include "./led/bsp_led.h"
#include "./usart/bsp_usart.h"
#include "./mpu6050/mpu6050.h"
#include "./i2c/bsp_i2c.h"


//设置是否使用LCD进行显示，不需要的话把这个宏注释掉即可
#define USE_LCD_DISPLAY

#ifdef USE_LCD_DISPLAY
 #include "./lcd/bsp_ili9341_lcd.h"
#endif


#define TASK_ENABLE 0
extern unsigned int Task_Delay[NumOfTask];
/*
 * t : 定时时间 
 * Ticks : 多少个时钟周期产生一次中断 
 * f : 时钟频率 72000000
 * t = Ticks * 1/f = (72000000/100000) * (1/72000000) = 10us 
 */ 

/**
  * @brief  主函数
  * @param  无  
  * @retval 无
  */
int main(void)
{
	short Accel[3];
	short Gyro[3];
	float Temp;
	
#ifdef USE_LCD_DISPLAY

	ILI9341_Init ();         //LCD 初始化
  ILI9341_GramScan ( 6 );
	LCD_SetFont(&Font8x16);
	LCD_SetColors(RED,BLACK);
	
	ILI9341_Clear ( 0, 0, 240, 320);	
  ILI9341_DispString_EN ( 20, 20, "This is a MPU6050 demo" );
#endif
  
  //初始化systick
	SysTick_Init();
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;
	/* LED 端口初始化 */
	LED_GPIO_Config();
	/* 串口1通信初始化 */
	USART_Config();

	//I2C初始化
	i2c_GPIO_Config();
  //MPU6050初始化
	MPU6050_Init();
	
	printf("\r\n 欢迎使用野火  STM32 开发板。\r\n");		 

	printf("\r\n 这是一个MPU6050测试例程 \r\n");


	//检测MPU6050
	if (MPU6050ReadID() == 1)
	{	
	
		while(1)
		{
			if(Task_Delay[0]==TASK_ENABLE)
			{
				LED2_TOGGLE;
				Task_Delay[0]=1000;
			}
			
			if(Task_Delay[1]==0)
			{
				MPU6050ReadAcc(Accel);			
				printf("\r\n加速度： %8d%8d%8d    ",Accel[0],Accel[1],Accel[2]);
				MPU6050ReadGyro(Gyro);
				printf("陀螺仪： %8d%8d%8d    ",Gyro[0],Gyro[1],Gyro[2]);
				
				MPU6050_ReturnTemp(&Temp); 
				printf("温度： %8.2f",Temp);
				
				#ifdef USE_LCD_DISPLAY	
					{
						char cStr [ 70 ];

						ILI9341_DispStringLine_EN(LINE(7),"Acceleration");		
						sprintf ( cStr, "%8d%8d%8d",Accel[0],Accel[1],Accel[2] );	//加速度原始数据
						ILI9341_DispStringLine_EN(LINE(8),cStr);					


						ILI9341_DispStringLine_EN(LINE(11),"Gyro        :");			
						sprintf ( cStr, "%8d%8d%8d",Gyro[0],Gyro[1],Gyro[2] );	//角原始数据
						ILI9341_DispStringLine_EN(LINE(12),cStr);	

						sprintf ( cStr, "Temperture  :%8.2f",Temp );	//温度值
						ILI9341_DispStringLine_EN(LINE(15),cStr);			

					}
				#endif
				
				Task_Delay[1]=500;//更新一次数据，可根据自己的需求，提高采样频率，如100ms采样一次
			}

			//*************************************	下面是增加任务的格式************************************//
	//		if(Task_Delay[i]==0)
	//		{
	//			Task(i);
	//			Task_Delay[i]=;
	//		}

		}   
	}
	else
	{
			printf("\r\n没有检测到MPU6050传感器！\r\n");
			LED_RED; 
			#ifdef USE_LCD_DISPLAY			
				/*设置字体颜色及字体的背景颜色*/
				LCD_SetColors(BLUE,BLACK);	

				ILI9341_DispStringLine_EN(LINE(4),"No MPU6050 detected! ");			
				ILI9341_DispStringLine_EN(LINE(5),"Please check the hardware connection! ");		

			#endif
		while(1);	
	}
	
}

/*********************************************END OF FILE**********************/
