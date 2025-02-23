/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "servo_motor.h"
#include "stepper_motor.h"
#include "joystick.h"
#include "working_led.h"


// GPIOS MAP
// GPIO18 (PICO W) - DIR (A4988 DRIVER 1)
// GPIO19 (PICO W) - STEP (A4988 DRIVER 1)
// GPIO20 (PICO W) - ENABLE (A4988 DRIVER 1)

// GPIO4 (PICO W) - ENABLE (A4988 DRIVER 2)
// GPIO9 (PICO W) - STEP (A4988 DRIVER 2)
// GPIO8 (PICO W) - DIR (A4988 DRIVER 2)

// GPIO17 (PICO W) - S (MICRO SERVO SG90)

// GPIO26 (PICO W) - X (JOYSTICK X AXIS)
// GPIO27 (PICO W) - Y (JOYSTICK Y AXIS)


// Definição dos pinos dos drivers A4988
#define DIR1  18
#define STEP1 19
#define EN1   20

#define DIR2  8
#define STEP2 9
#define EN2   4

#define DEFAULT_REVOLUTION_STEPS 400 // Quantidade de passos para completar uma volta 360º

#define DIREITA false
#define ESQUERDA true


int main() {
    stdio_init_all();

    // Configura o Working Led
    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);

    // Inicializar o joystick
    init_joystick();

    // Inicializa servo motor
    init_servo();


    // Inicializa motores de passo
    StepperMotor motor_ovo = {DIR1, STEP1, EN1, -1, -1, 0, DEFAULT_REVOLUTION_STEPS};
    StepperMotor motor_caneta = {DIR2, STEP2, EN2, 45, 135, 90, DEFAULT_REVOLUTION_STEPS};

    init_stepper_motor(&motor_ovo);
    init_stepper_motor(&motor_caneta);

    while (true) {
        set_working_led();

        control_stepper_by_joystick(&motor_ovo, &motor_caneta);

        // printf("Movendo motor_ovo 360° para a direita...\n");
        // move_stepper_motor(&motor_ovo, DIREITA, 360, 2000); // 360° para a direita
        // sleep_ms(500);

        // printf("Movendo motor_caneta 50° para a direita...\n");
        // move_stepper_motor(&motor_caneta, DIREITA, 30, 2000);
        // sleep_ms(500);

        // printf("Movendo motor_caneta 100° para a esquerda...\n");
        // move_stepper_motor(&motor_caneta, ESQUERDA, 60, 2000);
        // sleep_ms(500);

        // printf("Movendo motor_caneta 50° para a direita...\n");
        // move_stepper_motor(&motor_caneta, DIREITA, 30, 2000);
        // sleep_ms(500);

        // open_servo();
        // sleep_ms(1000);
        // close_servo();
        // sleep_ms(1000);

    }
}