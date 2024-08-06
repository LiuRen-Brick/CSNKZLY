#ifndef __DEV_PID_H
#define __DEV_PID_H

// �����ֱ��Ͳ���
#define INTEGRAL_LIMIT 100.0

void adjust_pid_parameters(double error);
double calculate_pid(double setpoint, double current_temp);

#endif
