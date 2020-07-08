/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "./spwm/bsp_spwm.h"




/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 

 



extern uint16_t indexWave[];

extern __IO uint32_t rgb_color;

/* 呼吸灯中断服务函数 */
void BRE_TIMx_IRQHandler(void)
{	
	static uint16_t pwm_index = 0;			//用于PWM查表
	static uint16_t period_cnt = 0;		//用于计算周期数
	static uint16_t amplitude_cnt = 0;	//用于计算幅值等级

	if (TIM_GetITStatus(BRE_TIMx, TIM_IT_Update) != RESET)	//TIM_IT_Update
 	{		
			amplitude_cnt++;
				
			//每个PWM表中的每个元素有AMPLITUDE_CLASS个等级，
		  //每增加一级多输出一次脉冲，即PWM表中的元素多使用一次
		  //使用256次，根据RGB颜色分量设置通道输出
			if(amplitude_cnt > (AMPLITUDE_CLASS-1))		 						
			{		
				period_cnt++;
				
				//每个PWM表中的每个元素使用period_class次
				if(period_cnt > period_class)
				{				

					pwm_index++;												//标志PWM表指向下一个元素
				
					//若PWM表已到达结尾，重新指向表头
					if( pwm_index >=  POINT_NUM)			
					{
						pwm_index=0;								
					}
					
					period_cnt = 0;											//重置周期计数标志
				}
				
				amplitude_cnt=0;											//重置幅值计数标志
			}
			else
			{	
					//每个PWM表中的每个元素有AMPLITUDE_CLASS个等级，
					//每增加一级多输出一次脉冲，即PWM表中的元素多使用一次

					//根据RGB颜色分量值，设置各个通道是否输出当前的PWM表元素表示的亮度
					//红
					if(((rgb_color&0xFF0000)>>16) >= amplitude_cnt)				
						BRE_TIMx->BRE_RED_CCRx = indexWave[pwm_index];	//根据PWM表修改定时器的比较寄存器值
					else
						BRE_TIMx->BRE_RED_CCRx = 0;		//比较寄存器值为0，通道输出高电平，该通道LED灯灭
					
					//绿
					if(((rgb_color&0x00FF00)>>8) >= amplitude_cnt)				
						BRE_TIMx->BRE_GREEN_CCRx = indexWave[pwm_index];	//根据PWM表修改定时器的比较寄存器值
					else
						BRE_TIMx->BRE_GREEN_CCRx = 0;	//比较寄存器值为0，通道输出高电平，该通道LED灯灭
					
					//蓝
					if((rgb_color&0x0000FF) >= amplitude_cnt)				
						BRE_TIMx->BRE_BLUE_CCRx = indexWave[pwm_index];	//根据PWM表修改定时器的比较寄存器值
					else
						BRE_TIMx->BRE_BLUE_CCRx = 0;		//比较寄存器值为0，通道输出高电平，该通道LED灯灭	

			}					
		
		TIM_ClearITPendingBit (BRE_TIMx, TIM_IT_Update);	//必须要清除中断标志位
	}
}
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
