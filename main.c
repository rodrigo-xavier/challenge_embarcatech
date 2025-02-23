/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "pico/cyw43_arch.h"

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

// Função para inicializar GPIOs dos motores de passo
void init_stepper_motor(uint dir, uint step, uint enable) {
    gpio_init(dir);
    gpio_set_dir(dir, GPIO_OUT);

    gpio_init(step);
    gpio_set_dir(step, GPIO_OUT);

    gpio_init(enable);
    gpio_set_dir(enable, GPIO_OUT);
    gpio_put(enable, 0);  // Habilita os drivers
}

// Função para movimentar um motor de passo
void move_stepper_motor(uint dir, uint step, bool direction, int steps, int delay) {
    gpio_put(dir, direction); // Define a direção
    for (int i = 0; i < steps; i++) {
        gpio_put(step, 1);
        sleep_us(delay);
        gpio_put(step, 0);
        sleep_us(delay);
    }
}

// Função para configurar o PWM do servo
void init_servo(uint pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(pin);
    pwm_set_wrap(slice, 20000);  // Período de 20ms
    pwm_set_clkdiv(slice, 125.0); // Divide clock para frequência correta
    pwm_set_enabled(slice, true);
}

// Função para mover o servo para um ângulo específico
void move_servo(uint pin, int angle) {
    uint slice = pwm_gpio_to_slice_num(pin);
    uint duty = 500 + (angle * 11); // Converte ângulo para PWM (500 a 2500us)
    pwm_set_gpio_level(pin, duty);
}


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
    init_stepper_motor(DIR1, STEP1, EN1);
    init_stepper_motor(DIR2, STEP2, EN2);

    // Inicializa servo motor
    init_servo(SERVO_PIN);

    printf("Movendo servo para 90°...\n");
    move_servo(SERVO_PIN, 180);
    sleep_ms(500);

    while (1) {
        pico_set_led(true);
        sleep_ms(LED_DELAY_MS);
        pico_set_led(false);
        sleep_ms(LED_DELAY_MS);

        pico_set_led(true);
        sleep_ms(250);
        pico_set_led(false);
        sleep_ms(250);

        printf("Movendo motores de passo 15° para a esquerda...\n");
        move_stepper_motor(DIR1, STEP1, true, STEPS_15_DEGREES, 1000);
        move_stepper_motor(DIR2, STEP2, false, STEPS_15_DEGREES, 1000);
        sleep_ms(500);

        printf("Voltando ao centro...\n");
        move_stepper_motor(DIR1, STEP1, false, STEPS_15_DEGREES, 1000);
        move_stepper_motor(DIR2, STEP2, true, STEPS_15_DEGREES, 1000);
        sleep_ms(500);

        printf("Movendo motores de passo 15° para a direita...\n");
        move_stepper_motor(DIR1, STEP1, false, STEPS_15_DEGREES, 1000);
        move_stepper_motor(DIR2, STEP2, true, STEPS_15_DEGREES, 1000);
        sleep_ms(500);

        printf("Voltando ao centro...\n");
        move_stepper_motor(DIR1, STEP1, true, STEPS_15_DEGREES, 1000);
        move_stepper_motor(DIR2, STEP2, false, STEPS_15_DEGREES, 1000);
        sleep_ms(500);

        printf("Voltando servo para 90°...\n");
        for (int angle = 180; angle >= 90; angle--) {
            move_servo(SERVO_PIN, angle);
            sleep_ms(10);
        }

        printf("Movendo servo de 90° para 180°...\n");
        for (int angle = 90; angle <= 180; angle++) {
            move_servo(SERVO_PIN, angle);
            sleep_ms(10);  // Pequeno atraso para suavizar o movimento
        }

    }
}