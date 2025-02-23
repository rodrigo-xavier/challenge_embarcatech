// servo_motor/servo_motor.c

#include "servo_motor.h"

// Função para configurar o PWM do servo
void init_servo(uint pin) {
    gpio_set_function(pin, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(pin);
    pwm_set_wrap(slice, 20000);  // Período de 20ms
    pwm_set_clkdiv(slice, 125.0); // Divide clock para frequência correta
    pwm_set_enabled(slice, true);

    // Inicializa o servo em 180° (ou outro valor desejado)
    move_servo(pin, 180); // Move para o ângulo inicial
}

// Função para mover o servo para um ângulo específico
void move_servo(uint pin, int angle) {
    uint slice = pwm_gpio_to_slice_num(pin);
    uint duty = 500 + (angle * 11); // Converte ângulo para PWM (500 a 2500us)
    pwm_set_gpio_level(pin, duty);
}

void open_servo(uint pin) {
    printf("Voltando servo para 90°...\n");
    for (int angle = 180; angle >= 90; angle--) {
        move_servo(pin, angle);
        sleep_ms(10);
    }
}

void close_servo(uint pin) {
    printf("Movendo servo de 90° para 180°...\n");
    for (int angle = 90; angle <= 180; angle++) {
        move_servo(pin, angle);
        sleep_ms(10);  // Pequeno atraso para suavizar o movimento
    }
}
