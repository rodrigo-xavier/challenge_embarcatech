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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/binary_info.h"
#include "ssd1306.h"
#include "hardware/i2c.h"
#include "play_audio.h"
#include "buttons.h"

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


#define CIRCLE 1
#define TRIANGLE 2
#define SQUARE 3
#define RANDOM 4

#define BUTTON_A_PIN 5          // GPIO5 para o botão A
#define BUTTON_B_PIN 6          // GPIO6 para o botão B


const uint I2C_SDA_PIN = 14;
const uint I2C_SCL_PIN = 15;


int main() {
    stdio_init_all();
    setup_audio();

    // Configura o Working Led
    pico_led_init();
    // hard_assert(rc == PICO_OK);

    // Inicializar o joystick
    init_joystick();

    // Inicializa servo motor
    init_servo();

    // Inicializa botões
    init_buttons();


    // Inicializa motores de passo
    StepperMotor motor_ovo = {DIR1, STEP1, EN1, -1, -1, 0, DEFAULT_REVOLUTION_STEPS};
    StepperMotor motor_caneta = {DIR2, STEP2, EN2, 70, 110, 90, DEFAULT_REVOLUTION_STEPS};

    init_stepper_motor(&motor_ovo);
    init_stepper_motor(&motor_caneta);






    // I2C is "open drain", pull ups to keep signal high when no data is being
    // sent
    i2c_init(i2c1, SSD1306_I2C_CLK * 1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    // run through the complete initialization process
    SSD1306_init();

    // Initialize render area for entire frame (SSD1306_WIDTH pixels by SSD1306_NUM_PAGES pages)
    struct render_area frame_area = {
        start_col : 0,
        end_col : SSD1306_WIDTH - 1,
        start_page : 0,
        end_page : SSD1306_NUM_PAGES - 1
    };

    calc_render_area_buflen(&frame_area);

    // zero the entire display
    uint8_t buf[SSD1306_BUF_LEN];
    memset(buf, 0, SSD1306_BUF_LEN);
    render(buf, &frame_area);

    char *text[] = {
        "   Bem-Vindo ",
        " ao EmbarcaTech ",
        "      2024 ",
        "  SOFTEX/MCTI "};

    int y = 0;
    for (uint i = 0; i < count_of(text); i++)
    {
        WriteString(buf, 5, y, text[i]);
        y += 8;
    }
    render(buf, &frame_area);
    sleep_ms(3000); // Exibir por 3 segundos

    memset(buf, 0, SSD1306_BUF_LEN);
    WriteString(buf, 10, 20, "Projeto Final");
    render(buf, &frame_area);
    sleep_ms(3000); // Exibir por 3 segundos

    memset(buf, 0, SSD1306_BUF_LEN);
    WriteString(buf, 10, 10, "Escolha:");
    WriteString(buf, 10, 20, "A - Manual");
    WriteString(buf, 10, 30, "B - Random");
    render(buf, &frame_area);

    while (true) {
        set_working_led();

        // Verifica se o usuário pressionou A (Modo Manual)
        if (debounce_button(BUTTON_A_PIN)) {
            memset(buf, 0, SSD1306_BUF_LEN);
            WriteString(buf, 10, 20, "Modo Manual");
            render(buf, &frame_area);
            sleep_ms(1000); // Pequeno delay para exibição

            // Volta para o menu ao pressionar A novamente
            while (!debounce_button(BUTTON_A_PIN)) {
                control_stepper_by_joystick(&motor_ovo, &motor_caneta);
                sleep_ms(500);
            }

            memset(buf, 0, SSD1306_BUF_LEN);
            WriteString(buf, 10, 10, "Escolha:");
            WriteString(buf, 10, 20, "A - Manual");
            WriteString(buf, 10, 30, "B - Random");
            render(buf, &frame_area);
        }

        // Verifica se o usuário pressionou B (Modo Aleatório)
        if (debounce_button(BUTTON_B_PIN)) {
            memset(buf, 0, SSD1306_BUF_LEN);
            WriteString(buf, 10, 20, "Modo Random");
            render(buf, &frame_area);
            sleep_ms(1000); // Pequeno delay para exibição

            // Volta para o menu ao pressionar A novamente
            while (!debounce_button(BUTTON_A_PIN)) {
                draw(&motor_ovo, &motor_caneta, RANDOM, 30);
                sleep_ms(500);
            }

            memset(buf, 0, SSD1306_BUF_LEN);
            WriteString(buf, 10, 10, "Escolha:");
            WriteString(buf, 10, 20, "A - Manual");
            WriteString(buf, 10, 30, "B - Random");
            render(buf, &frame_area);
        }

        sleep_ms(10); // Evita loop excessivo
    }
}