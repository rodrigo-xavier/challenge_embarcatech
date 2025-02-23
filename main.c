/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "pico/cyw43_arch.h"
#include "servo_motor.h"
#include "stepper_motor.h"

// Pico W devices use a GPIO on the WIFI chip for the LED,
// so when building for Pico W, CYW43_WL_GPIO_LED_PIN will be defined
#ifdef CYW43_WL_GPIO_LED_PIN
#endif
#define PICO_DEFAULT_LED_PIN 11              // GPIO11 para o LED

#ifndef LED_DELAY_MS
#define LED_DELAY_MS 100
#endif


// GPIOS MAP
// GPIO18 (PICO W) - DIR (A4988 DRIVER 1)
// GPIO19 (PICO W) - STEP (A4988 DRIVER 1)
// GPIO20 (PICO W) - ENABLE (A4988 DRIVER 1)

// GPIO4 (PICO W) - ENABLE (A4988 DRIVER 2)
// GPIO9 (PICO W) - STEP (A4988 DRIVER 2)
// GPIO8 (PICO W) - DIR (A4988 DRIVER 2)

// GPI28 (PICO W) - S (MICRO SERVO SG90)


// Definição dos pinos dos drivers A4988
#define DIR1  18
#define STEP1 19
#define EN1   20

#define DIR2  8
#define STEP2 9
#define EN2   4

// Definição do pino do servo SG90
#define SERVO_PIN 17

// Número de passos para 15° (considerando motor de 200 passos por revolução)
#define STEPS_15_DEGREES (200 / 360.0 * 50)

// Inicializa o LED como PWM
int pico_led_init(void) {
    #if defined(PICO_DEFAULT_LED_PIN)
        gpio_set_function(PICO_DEFAULT_LED_PIN, GPIO_FUNC_PWM);  // Configura como PWM
        uint slice = pwm_gpio_to_slice_num(PICO_DEFAULT_LED_PIN);

        pwm_set_wrap(slice, 255);   // Define resolução do PWM (0-255)
        pwm_set_clkdiv(slice, 4.0); // Ajusta a frequência do PWM

        pwm_set_enabled(slice, true);
    #elif defined(CYW43_WL_GPIO_LED_PIN)
        return cyw43_arch_init();  // Inicializa WiFi chip (caso do Pico W)
    #endif
    }

    // Controla o LED com brilho ajustável
    void pico_set_led(bool led_on) {
    #if defined(PICO_DEFAULT_LED_PIN)
        uint slice = pwm_gpio_to_slice_num(PICO_DEFAULT_LED_PIN);
        if (led_on) {
            pwm_set_gpio_level(PICO_DEFAULT_LED_PIN, 25);  // 10% de 255
        } else {
            pwm_set_gpio_level(PICO_DEFAULT_LED_PIN, 0);   // Desliga o LED
        }
    #elif defined(CYW43_WL_GPIO_LED_PIN)
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);
    #endif
    }

int main() {
    stdio_init_all();

    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);

    // Inicializa motores de passo
    StepperMotor motor_ovo = {DIR1, STEP1, EN1, -1, -1};  // Motor_ovo não tem ângulos mínimos ou máximos
    StepperMotor motor_caneta = {DIR2, STEP2, EN2, 0, 180}; // Motor_caneta com ângulos de 0 a 180

    init_stepper_motor(&motor_ovo);
    init_stepper_motor(&motor_caneta);

    // Inicializa servo motor
    init_servo(SERVO_PIN);

    while (1) {
        pico_set_led(true);
        sleep_ms(LED_DELAY_MS);
        pico_set_led(false);
        sleep_ms(LED_DELAY_MS);

        printf("Movendo motores de passo 15° para a esquerda...\n");
        move_stepper_motor(&motor_ovo, true, STEPS_15_DEGREES, 1000);
        move_stepper_motor(&motor_caneta, false, STEPS_15_DEGREES, 1000);
        sleep_ms(500);

        printf("Voltando ao centro...\n");
        move_stepper_motor(&motor_ovo, false, STEPS_15_DEGREES, 1000);
        move_stepper_motor(&motor_caneta, true, STEPS_15_DEGREES, 1000);
        sleep_ms(500);

        printf("Movendo motores de passo 15° para a direita...\n");
        move_stepper_motor(&motor_ovo, false, STEPS_15_DEGREES, 1000);
        move_stepper_motor(&motor_caneta, true, STEPS_15_DEGREES, 1000);
        sleep_ms(500);

        printf("Voltando ao centro...\n");
        move_stepper_motor(&motor_ovo, true, STEPS_15_DEGREES, 1000);
        move_stepper_motor(&motor_caneta, false, STEPS_15_DEGREES, 1000);
        sleep_ms(500);

        open_servo(SERVO_PIN);
        sleep_ms(1000);
        close_servo(SERVO_PIN);
        sleep_ms(1000);

    }
}