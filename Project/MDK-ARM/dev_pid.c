#include "dev_pid.h"
#include <math.h>

// PID����������
double Kp = 1.0;
double Ki = 0.5;
double Kd = 0.1;

// �������
double setpoint = 38.0; // �趨�¶�
double current_temp = 25.0; // ��ǰ�¶�
double error = 0.0; // ���
double integral = 0.0; // �������
double prev_error = 0.0; // ��һ�����
double control_output = 0.0; // �������
double prev_output = 0.0; // ��һ�ο������

// ʱ����Ʊ���
double dt = 1.0;

// ����ӦPID������������
void adjust_pid_parameters(double error) {
    // �򵥵�����ӦPID���������߼�
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

// PID���㺯��
double calculate_pid(double setpoint, double current_temp) {
    error = setpoint - current_temp;

    // ����ӦPID��������
    adjust_pid_parameters(error);

    // �����ֱ���
    if (prev_output < 100.0 && prev_output > 0.0) {
        integral += error * dt;
    }
    if (integral > INTEGRAL_LIMIT) {
        integral = INTEGRAL_LIMIT;
    } else if (integral < -INTEGRAL_LIMIT) {
        integral = -INTEGRAL_LIMIT;
    }

    // ΢������
    double derivative = (error - prev_error) / dt;

    control_output = Kp * error + Ki * integral + Kd * derivative;
    prev_error = error;
    prev_output = control_output;

    return control_output;
}
