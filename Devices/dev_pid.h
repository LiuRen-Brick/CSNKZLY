#ifndef __DEV_PID_H
#define __DEV_PID_H

typedef struct {
    float kp;           // 比例系数
    float ki;           // 积分系数
    float kd;           // 微分系数
    float setpoint;     // 设定值
    float previous_error; // 前一次误差
    float integral;     // 积分项
    float output_min;   // 输出最小值
    float output_max;   // 输出最大值
} PIDController;

void PIDController_Init(PIDController *pid, float kp, float ki, float kd, float setpoint, float output_min, float output_max);
float PIDController_Update(PIDController *pid, float current_value, float dt);

#endif
