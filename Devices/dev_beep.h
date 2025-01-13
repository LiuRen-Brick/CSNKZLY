#ifndef __DEV_BEEP_H
#define __DEV_BEEP_H

typedef enum {
    DEVBEEP_OFF,
    DEV_STARTUP,
    DEV_WORK,
    DEV_FINISH,
}BEEP_STATE;

void DevBeep_MainFunc(void);

#endif
