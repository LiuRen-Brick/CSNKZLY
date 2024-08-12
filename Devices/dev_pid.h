#ifndef __DEV_PID_H
#define __DEV_PID_H

typedef struct {
    float kp;           // ����ϵ��
    float ki;           // ����ϵ��
    float kd;           // ΢��ϵ��
    float setpoint;     // �趨ֵ
    float previous_error; // ǰһ�����
    float integral;     // ������
    float output_min;   // �����Сֵ
    float output_max;   // ������ֵ
} PIDController;

void PIDController_Init(PIDController *pid, float kp, float ki, float kd, float setpoint, float output_min, float output_max);
float PIDController_Update(PIDController *pid, float current_value, float dt);

#endif
