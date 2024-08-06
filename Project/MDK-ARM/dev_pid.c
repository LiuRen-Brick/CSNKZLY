#include "dev_pid.h"
#include <math.h>

// PID控制器参数
double Kp = 1.0;
double Ki = 0.5;
double Kd = 0.1;

// 定义变量
double setpoint = 38.0; // 设定温度
double current_temp = 25.0; // 当前温度
double error = 0.0; // 误差
double integral = 0.0; // 积分误差
double prev_error = 0.0; // 上一次误差
double control_output = 0.0; // 控制输出
double prev_output = 0.0; // 上一次控制输出

// 时间控制变量
double dt = 1.0;

// 自适应PID参数调整函数
void adjust_pid_parameters(double error) {
    // 简单的自适应PID参数调整逻辑
    if (fabs(error) < 1.0) {
        Kp += 0.01;
        Ki += 0.005;
        Kd += 0.001;
    } else {
        Kp -= 0.01;
        Ki -= 0.005;
        Kd -= 0.001;
    }
}

// PID计算函数
double calculate_pid(double setpoint, double current_temp) {
    error = setpoint - current_temp;

    // 自适应PID参数调整
    adjust_pid_parameters(error);

    // 抗积分饱和
    if (prev_output < 100.0 && prev_output > 0.0) {
        integral += error * dt;
    }
    if (integral > INTEGRAL_LIMIT) {
        integral = INTEGRAL_LIMIT;
    } else if (integral < -INTEGRAL_LIMIT) {
        integral = -INTEGRAL_LIMIT;
    }

    // 微分先行
    double derivative = (error - prev_error) / dt;

    control_output = Kp * error + Ki * integral + Kd * derivative;
    prev_error = error;
    prev_output = control_output;

    return control_output;
}
