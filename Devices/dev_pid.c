#include "dev_pid.h"


PIDController Pid_Contronl;
// ��ʼ��PID������
void PIDController_Init(PIDController *pid, float kp, float ki, float kd, float setpoint, float output_min, float output_max) 
{
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->setpoint = setpoint;
    pid->previous_error = 0;
    pid->integral = 0;
    pid->output_min = output_min;
    pid->output_max = output_max;
}

// ����PID���������
float PIDController_Update(PIDController *pid, float current_value, float dt) {
    float error = pid->setpoint - current_value;  // �������

    // ��������£����ǿ����ֱ���
    pid->integral += error * dt;
    if (pid->integral > pid->output_max) {
        pid->integral = pid->output_max;
    } else if (pid->integral < pid->output_min) {
        pid->integral = pid->output_min;
    }

    float derivative = (error - pid->previous_error) / dt;  // ����΢����
    float output = pid->kp * error + pid->ki * pid->integral + pid->kd * derivative;  // ����������

    // �������
    if (output > pid->output_max) {
        output = pid->output_max;
    } else if (output < pid->output_min) {
        output = pid->output_min;
    }

    pid->previous_error = error;  // ����ǰһ�����
    return output;
}

