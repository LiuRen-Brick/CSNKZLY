/**
  *********************************************************************************
  * @file    	    FT32F0xx_it.c
  * @author  	    FMD AE
  * @brief   		FT32F0xx_it program body 	
  * @version 	    V1.0.0           
  * @data		    2021-09-27
  *********************************************************************************
  * @attention
  * COPYRIGHT (C) 2021 Fremont Micro Devices (SZ) Corporation All rights reserved.
  *    This software is provided by the copyright holders and contributors,and the
  *software is believed to be accurate and reliable. However, Fremont Micro Devices
  *(SZ) Corporation assumes no responsibility for the consequences of use of such
  *software or for any infringement of patents of other rights of third parties,
  *which may result from its use. No license is granted by implication or otherwise
  *under any patent rights of Fremont Micro Devices (SZ) Corporation.
  *  ******************************************************************************
  */
/* Includes ----------------------------------------------------------------------*/ 
#include "ft32f0xx_it.h"
/* Private Constant --------------------------------------------------------------*/
#include "dev_gpio.h"
#include "dev_flash.h"
#include "dev_ad9877.h"
#include "dev_beep.h"
#include "App.h"
/* Public Constant ---------------------------------------------------------------*/
/* Private typedef ---------------------------------------------------------------*/
/* Private variables -------------------------------------------------------------*/

/* Public variables --------------------------------------------------------------*/
extern uint8_t PowerOff_Flg;
extern DEV_WORK_STATE DevWorkState;
extern BEEP_STATE BeepState;
extern uint8_t UltraDuty;

uint32_t WorkTime = 0;

uint32_t KEY1_Count = 0;      //按键1计数器
uint32_t KEY2_Count = 0;			//按键2计数器
uint32_t POWER_Count = 0;     //电源键计数器

uint8_t KEY1_Flg = 0;					//按键1标志位
uint8_t KEY2_Flg = 0;					//按键2计数器
uint8_t PowerOn_Flg = 0;			//电源键计数器

uint32_t StandyTime = 0;

static uint8_t ChargeFlg = 0;
/* Private function prototypes ---------------------------------------------------*/
/* Public function ------ --------------------------------------------------------*/
/* Private function ------ -------------------------------------------------------*/

/**********************************************************************************
  * @brief  NMI_Handler program.
  * @param  None
  * @note
  * @retval None
  *********************************************************************************
*/
void NMI_Handler(void)
{
}

/**********************************************************************************
  * @brief  HardFault_Handler program.
  * @param  None
  * @note
  * @retval None
  *********************************************************************************
*/
void HardFault_Handler(void)
{
  while (1)
  {
  }
}
/**********************************************************************************
  * @brief  TIM1_BRK_UP_TRG_COM_IRQHandler program.
  * @param  None
  * @note
  * @retval None
  *********************************************************************************
*/

//5ms周期
void TIM14_IRQHandler(void)
{
	static uint16_t ChargeCount = 0;
	if (TIM_GetITStatus(TIM14, TIM_IT_Update) != RESET)
	{
		StandyTime++;
		if(DevGpio_ReadInPut(BATCHARGE))
		{
				ChargeCount++;
			if(ChargeCount > 200)
			{
					PowerOff_Flg = 1;
					DevWorkState = CLOSE_STATE;
				
					ChargeFlg = 1;
			}
		}else
		{
				ChargeCount = 0;
		}
		
		if(DevWorkState == WORK_STATE)
		{
				WorkTime++;
				if(WorkTime > 360000)
				{
						DevWorkState = IDLE_STATE;
						BeepState = DEV_FINISH;
				}else
				{
				}
		}else if(DevWorkState == PAUSE_STATE)
		{
				
		}else
		{
				WorkTime = 0;
		}
		//Task_Count();
		TIM_ClearITPendingBit(TIM14, TIM_IT_Update);
	}
}
/**********************************************************************************
  * @brief  TIM16_IRQHandler program.
  * @param  None
  * @note
  * @retval None
  *********************************************************************************
*/

/**********************************************************************************
  * @brief  TIM17_IRQHandler program.
  * @param  None
  * @note
  * @retval None
  *********************************************************************************
*/
//1ms
void TIM17_IRQHandler(void)
{
	static uint8_t SetFlg = 0;
	if (TIM_GetITStatus(TIM17, TIM_IT_Update) != RESET)
	{
		/**************************按键1触发事件**************************/ 
		if(KEY1_Flg == 1)
		{
			KEY1_Count++;
		}else{
			if ((KEY1_Count > 3) && (KEY1_Count < 1000))
			{
				BeepState = DEV_WORK;
				DevWorkState = WORK_STATE;
				UltraDuty = 25;
			}else
			{
				
			}
			KEY1_Count = 0;
		}
		/**********************按键2触发事件*********************** */
		if(KEY2_Flg == 1)
		{
			KEY2_Count++;
		}else{
			if ((KEY2_Count > 3) && (KEY2_Count < 1000))
			{		
					BeepState = DEV_WORK;
					DevWorkState = WORK_STATE;
					UltraDuty = 66;
			}else
			{
			}
			KEY2_Count = 0;
		}
		/**********************电源键触发事件**********************************/
		PowerOn_Flg = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10) ^ 0x01;
		if (PowerOn_Flg == 1)
		{
			POWER_Count++;
			if((POWER_Count > 2000) && (SetFlg == 0) && (ChargeFlg != 1))
			{
					PowerOff_Flg = PowerOff_Flg ^ 0x01;
					BeepState = DEV_STARTUP;
					SetFlg = 1;
			}
		}else
		{
			if ((POWER_Count > 3) && (POWER_Count < 1000))
			{
					BeepState = DEV_WORK;
					DevWorkState = PAUSE_STATE;
			}else
			{
			}
			POWER_Count = 0;
			SetFlg = 0;
		}
		TIM_ClearITPendingBit(TIM17, TIM_IT_Update);
	}
}

/**
 * @brief  This function handles EXTI4_15 interrupt request.
 * @param  None
 * @retval None
 */
void EXTI4_15_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line5) != RESET)
	{
		KEY1_Flg = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) ^ 0x01;
		/* Clear the EXTI line 11 pending bit */
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
	else if (EXTI_GetITStatus(EXTI_Line4) != RESET)
	{
		KEY2_Flg = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4) ^ 0x01;
		EXTI_ClearITPendingBit(EXTI_Line4);
	}
	else
	{
	}
}
/************************* (C) COPYRIGHT FMD *****END OF FILE*********************/
