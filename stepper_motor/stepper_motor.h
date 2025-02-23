#ifndef STEPPER_MOTOR_H
#define STEPPER_MOTOR_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "../joystick/joystick.h"

typedef struct {
    uint dir_pin; // Pino de Direção
    uint step_pin; // Pino de step
    uint enable_pin; // Pino de Enable
    int min_angle;  // Usado apenas para motor2
    int max_angle;  // Usado apenas para motor2
    int current_angle; // Armazena o ângulo atual do motor
    int default_revolution_steps;   // Número de passos por revolução
} StepperMotor;

// Função para inicializar GPIOs dos motores de passo
void init_stepper_motor(StepperMotor *motor);

// Função para movimentar um motor de passo
void move_stepper_motor(StepperMotor *motor, bool direction, uint angle, int delay);

// Função para calcular a quantidade de passos por ângulo
int calculate_steps(int revolution_steps, uint angle);

void control_stepper_by_joystick(StepperMotor *motor_x, StepperMotor *motor_y);

#endif // STEPPER_MOTOR_H
