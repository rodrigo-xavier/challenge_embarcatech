#ifndef STEPPER_MOTOR_H
#define STEPPER_MOTOR_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

// Número de passos para 15° (considerando motor de 200 passos por revolução)
#define STEPS_15_DEGREES (200 / 360.0 * 50)

typedef struct {
    uint dir_pin;
    uint step_pin;
    uint enable_pin;
    int min_angle;  // Usado apenas para motor2
    int max_angle;  // Usado apenas para motor2
    int current_angle; // Armazena o ângulo atual do motor
} StepperMotor;

// Função para inicializar GPIOs dos motores de passo
void init_stepper_motor(StepperMotor *motor);

// Função para movimentar um motor de passo
void move_stepper_motor(StepperMotor *motor, bool direction, int steps, int delay);

#endif // STEPPER_MOTOR_H
