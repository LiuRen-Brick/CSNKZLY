/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef	__SYSTICK_H
#define	__SYSTICK_H

/* Includes ------------------------------------------------------------------*/
#include "ft32f0xx.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void SystickInit(void);
void Delay(__IO uint32_t nTime);
void TimingDelay_Decrement(void);

#endif	/* __BSP_SYSTICK_H */
												
/************************ (C) COPYRIGHT Fremont Micro Devices *****END OF FILE****/												




