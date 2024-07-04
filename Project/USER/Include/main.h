/********** (C) COPYRIGHT 2021 Fremont Micro Devices (SZ) Corporation *********
* @file      : main.h
* @author    : FMD AE
* @brief     : Header for main.c module
* @version   : V1.0.0
* @data      : 2021-09-14
******************************************************************************
* @attention
* COPYRIGHT (C) 2021 Fremont Micro Devices (SZ) Corporation All rights reserved.
* This software is provided by the copyright holders and contributors,and the
*   software is believed to be accurate and reliable. However, Fremont Micro
*   Devices (SZ) Corporation assumes no responsibility for the consequences of
*   use of such software or for any infringement of patents of other rights
*   of third parties, which may result from its use. No license is granted by
*   implication or otherwise under any patent rights of Fremont Micro Devices (SZ)
*   Corporation.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "ft32f0xx.h"
#include "ft32f0xx_tim.h"
#include "ft32f0xx_gpio.h"
#include <stdbool.h>
#include "bsp_systick.h"


#define ADC1_DR_Address		0x40012400 + 0X40

void FT32_GPIOA_Init(void);
void FT32_GPIOB_Init(void);
void FT32_EXIT_Init(void);
void ADC_DMA_Config(void);
void Tim3BaseInit(void);
void Tim14BaseInit(void);
void Tim16BaseInit(void);
void Tim17BaseInit(void);
void IWDG_Config(void);
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT Fremont Micro Devices *****END OF FILE****/
