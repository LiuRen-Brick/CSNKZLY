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
/* Public Constant ---------------------------------------------------------------*/
/* Private typedef ---------------------------------------------------------------*/
/* Private variables -------------------------------------------------------------*/
static uint32_t WorkTime = 0;
/* Public variables --------------------------------------------------------------*/
extern uint8_t PowerOff_Flg;
extern uint8_t Led_RedFlg;
extern union DATA_STORE Data_Store;

uint32_t KEY1_Count = 0;      //按键1计数器
uint32_t KEY2_Count = 0;			//按键2计数器
uint32_t POWER_Count = 0;     //电源键计数器

uint8_t KEY1_Flg = 0;					//按键1标志位
uint8_t KEY2_Flg = 0;					//按键2计数器
uint8_t PowerOn_Flg = 0;			//电源键计数器

uint8_t Beep_SatrtFlg = 0;    //蜂鸣器启动标志位
uint8_t WorkStart_Flg = 0;		//超声工作标志位
uint8_t SetFreq_Flg = 0;

uint16_t CycleTime = 0;				//PID调控脉冲工作周期

uint8_t Clear_Flg = 0;				//
uint8_t UltraModule = 2;      //工作模式
uint8_t Motor_Level = 2;			//电机震动等级
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
		
		if(DevGpio_ReadInPut(BATCHARGE))
		{
				ChargeCount++;
			if(ChargeCount > 200)
			{
					PowerOff_Flg = 1;
			}
		}else
		{
				ChargeCount = 0;
		}
		if(WorkStart_Flg == 1)
		{
				WorkTime++;
				if(WorkTime > 360000)
				{
						WorkTime = 0;
					  WorkStart_Flg = 0;
					  Beep_SatrtFlg = 4;
				}else
				{}
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
void TIM17_IRQHandler(void)
{
	static uint8_t Key1_Sta = 0;
	static uint8_t Key2_Sta = 1;
	static uint8_t SetFlg = 0;
	

	if (TIM_GetITStatus(TIM17, TIM_IT_Update) != RESET)
	{
		/**************************按键1触发事件**************************/ 
		if(KEY1_Flg == 1)
		{
			KEY1_Count++;
			if((KEY1_Count >= 1000) && (Key1_Sta == 0))
			{
				Motor_Level++;
				if (Motor_Level > 5)
				{
					Motor_Level = 5;
				}else
				{
				}
				Beep_SatrtFlg = 3;
			}else
			{
			}
		}else{
			if ((KEY1_Count > 3) && (KEY1_Count < 1000))
			{
				UltraModule++;
				if (UltraModule > 2)
				{
					UltraModule = 2;
				}else
				{
				}
				Beep_SatrtFlg = 2;
				Key1_Sta = 1;
			}else
			{
			}
			Key1_Sta = 0;
			KEY1_Count = 0;
		}
		
		/**********************按键2触发事件*********************** */
		if(KEY2_Flg == 1)
		{
			KEY2_Count++;
			if ((KEY2_Count >= 1000) && (Key2_Sta == 0))
			{
					Motor_Level--;
					if (Motor_Level < 1)
					{
					Motor_Level = 1;
					}else
					{
					}
					Beep_SatrtFlg = 3;
			}else
			{
			}
		}else{
			if ((KEY2_Count > 3) && (KEY2_Count < 1000))
			{		
					UltraModule--;
					if (UltraModule < 1)
					{
					UltraModule = 1;
					}else
					{
					}
					Key2_Sta = 1;
					Beep_SatrtFlg = 2;
			}else
			{
			}
			Key2_Sta = 0;
			KEY2_Count = 0;
		}
		/**********************电源键触发事件**********************************/
		PowerOn_Flg = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10) ^ 0x01;
		if (PowerOn_Flg == 1)
		{
			POWER_Count++;
			if ((POWER_Count > 1000) && (PowerOff_Flg == 1) && (SetFlg == 0))
			{
				PowerOff_Flg = 0;
				SetFlg = 1;
			}else if ((POWER_Count > 1000) && (PowerOff_Flg == 0) && (SetFlg == 0))
			{
				PowerOff_Flg = 1;
				SetFlg = 1;
			}else
			{
			}
		}else
		{
			if ((POWER_Count > 3) && (POWER_Count < 1000))
			{
				if((WorkStart_Flg == 1) || (Led_RedFlg != 0))
				{
					WorkStart_Flg = 0;
					Beep_SatrtFlg = 4;
				}else if((WorkStart_Flg == 0) && (Led_RedFlg == 0))
				{	
					WorkStart_Flg = 1;
					Beep_SatrtFlg = 1;
				}else
				{
				}
			}else
			{
			}
			SetFlg = 0;
			POWER_Count = 0;
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
