/**
	******************************************************************************
	* @file 		ft32f0xx_it.c
	* @author 	FMD AE
  * @brief   	Main Interrupt Service Routines.
  *          	This file provides template for all exceptions handler and 
  *          	peripherals interrupt service routine.
	* @version 	V1.0.0
	* @data 		2021-09-14
	******************************************************************************
	* @attention
	* COPYRIGHT (C) 2021 Fremont Micro Devices (SZ) Corporation All rights reserved.
	* This software is provided by the copyright holders and contributors,and the
	*	software is believed to be accurate and reliable. However, Fremont Micro
	*	Devices (SZ) Corporation assumes no responsibility for the consequences of
	*	use of such software or for any infringement of patents of other rights
	*	of third parties, which may result from its use. No license is granted by
	*	implication or otherwise under any patent rights of Fremont Micro Devices (SZ)
	*	Corporation.
	******************************************************************************
	*/
	
/* Includes ------------------------------------------------------------------*/
#include "ft32f0xx_it.h"
#include "systick.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t KEY1_Count = 0;
uint32_t KEY2_Count = 0;

uint8_t KEY1_Flg = 0;
uint8_t KEY2_Flg = 0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
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
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
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
	 TimingDelay_Decrement();
	 KEY1_Count++;
	 KEY2_Count++;
}

/**
  * @brief  This function handles TIM14 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM14_IRQHandler(void)
{
	
}

/**
  * @brief  This function handles TIM1 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
{
	
}

/**
  * @brief  This function handles EXTI4_15 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI4_15_IRQHandler(void)
{
	
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)
	{
			if(KEY1_Count > 500)
			{
				KEY1_Flg = 1;
				KEY1_Count = 500;
			}
			 /* Clear the EXTI line 11 pending bit */
			EXTI_ClearITPendingBit(EXTI_Line4);
	}else
	{
			KEY1_Flg = 0;
			KEY1_Count = 0;
	}
	
	if(EXTI_GetITStatus(EXTI_Line5) != RESET)
	{
		if(KEY2_Count > 500)
		{
				KEY2_Flg = 1;
			  KEY2_Count = 500;
		}
		 /* Clear the EXTI line 11 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line5);
	}else
	{
			KEY2_Flg = 0;
			KEY2_Count = 0;
	}
}
/******************************************************************************/
/*                 FT32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_ft32f0xx.s).                                               */
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

/**
  * @}
  */

/************************ (C) COPYRIGHT Fremont Micro Devices *****END OF FILE****/
