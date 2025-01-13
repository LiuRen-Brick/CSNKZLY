#include "dev_beep.h"
#include "dev_pwm.h"

#define BEEP_ON() Devpwm_SetDuty(SET_PWM2, 15)
#define BEEP_OFF() Devpwm_SetDuty(SET_PWM2, 0)

uint16_t DuratTime = 0;
BEEP_STATE BeepState = DEVBEEP_OFF;

void DevBeep_MainFunc(void)
{
    static uint8_t cnt = 0;
    uint8_t sta = 0;

    sta = BeepState;
    DuratTime++;
    switch (sta)
    {
    case DEVBEEP_OFF:
        
        BEEP_OFF();
        DuratTime = 0;
        break;

    case DEV_STARTUP:

        BEEP_ON();
        if(DuratTime > 50)
        {
            BeepState = DEVBEEP_OFF;
        }
        break;

    case DEV_WORK:

        BEEP_ON();
        if(DuratTime > 10)
        {
            BeepState = DEVBEEP_OFF;
        }
        break;

    case DEV_FINISH:
    
        if(cnt % 2 == 0)
        {
            BEEP_ON();
        }else if(cnt % 2 == 1)
        {
            BEEP_OFF();
        }

        if(DuratTime > 40)
        {
            cnt++;
            DuratTime = 0;
            if(cnt >= 6)
            {
								cnt = 0;
                BeepState = DEVBEEP_OFF;
            }
        }
        break;

    default:
        break;
    }
}