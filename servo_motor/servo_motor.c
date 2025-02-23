// servo_motor/servo_motor.c

#include "servo_motor.h"

// Função para configurar o PWM do servo
void init_servo() {
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(SERVO_PIN);
    pwm_set_wrap(slice, 20000);  // Período de 20ms
    pwm_set_clkdiv(slice, 125.0); // Divide clock para frequência correta
    pwm_set_enabled(slice, true);

    // Inicializa o servo em 180° (ou outro valor desejado)
    move_servo(180); // Move para o ângulo inicial
}

// Função para mover o servo para um ângulo específico
void move_servo(int angle) {
    uint slice = pwm_gpio_to_slice_num(SERVO_PIN);
    uint duty = 500 + (angle * 11); // Converte ângulo para PWM (500 a 2500us)
    pwm_set_gpio_level(SERVO_PIN, duty);
}

void open_servo() {
    printf("Voltando servo para 90°...\n");
    for (int angle = 180; angle >= 90; angle--) {
        move_servo(angle);
        sleep_ms(10);
    }
}

void close_servo() {
    printf("Movendo servo de 90° para 180°...\n");
    for (int angle = 90; angle <= 180; angle++) {
        move_servo(angle);
        sleep_ms(10);  // Pequeno atraso para suavizar o movimento
    }
}
