/**
  ******************************************************************************
  * @file    	    main.h
  * @author  	    FMD AE
  * @brief   		main Header File.             	
  * @version 	    V1.0.0           
  * @data		    2021-09-27
  ******************************************************************************
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
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ---------------------------------------------------------------------*/
#include "config.h"
#include "ft32f0xx_conf.h"
/* Public Constant prototypes----------------------------------------------------*/
/* Public typedef ---------------------------------------------------------------*/
/* Public define ----------------------------------------------------------------*/
#define FALSE 0
#define TRUE	1
#define ADC1_DR_Address		0x40012400 + 0X40
#define POWER_ON() 	  GPIO_WriteBit(GPIOA,GPIO_Pin_9,Bit_SET);
#define POWER_OFF() 	GPIO_WriteBit(GPIOA,GPIO_Pin_9,Bit_RESET);

/* Public variables prototypes --------------------------------------------------*/

/* Public function prototypes----------------------------------------------------*/

#endif /* __MAIN_H */

/************************* (C) COPYRIGHT FMD *****END OF FILE*********************/
