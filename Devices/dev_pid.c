#include "dev_pid.h"


PIDController Pid_Contronl;
// 初始化PID控制器
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

// 更新PID控制器输出
float PIDController_Update(PIDController *pid, float current_value, float dt) {
    float error = pid->setpoint - current_value;  // 计算误差

    // 积分项更新，考虑抗积分饱和
    pid->integral += error * dt;
    if (pid->integral > pid->output_max) {
        pid->integral = pid->output_max;
    } else if (pid->integral < pid->output_min) {
        pid->integral = pid->output_min;
    }

    float derivative = (error - pid->previous_error) / dt;  // 计算微分项
    float output = pid->kp * error + pid->ki * pid->integral + pid->kd * derivative;  // 计算控制输出

    // 输出限制
    if (output > pid->output_max) {
        output = pid->output_max;
    } else if (output < pid->output_min) {
        output = pid->output_min;
    }

    pid->previous_error = error;  // 更新前一次误差
    return output;
}

