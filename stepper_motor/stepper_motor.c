#include "stepper_motor.h"
#include "pico/stdlib.h"

// Função para inicializar GPIOs dos motores de passo
void init_stepper_motor(StepperMotor *motor) {
    gpio_init(motor->dir_pin);
    gpio_set_dir(motor->dir_pin, GPIO_OUT);

    gpio_init(motor->step_pin);
    gpio_set_dir(motor->step_pin, GPIO_OUT);

    gpio_init(motor->enable_pin);
    gpio_set_dir(motor->enable_pin, GPIO_OUT);
    gpio_put(motor->enable_pin, 0);  // Habilita os drivers

    motor->current_angle = 0;  // Inicializa o ângulo do motor como 0
}

int calculate_steps(int revolution_steps, uint angle){
    uint steps = (revolution_steps / 360) * angle;
    return steps;
}

void move_stepper_motor(StepperMotor *motor, bool direction, uint angle, int delay_us) {
    int new_angle = direction ? (motor->current_angle + angle) : (motor->current_angle - angle);

    // Verificar se min_angle ou max_angle são NULL (sem limite de ângulo)
    if (motor->min_angle != -1 && new_angle < motor->min_angle) {
        angle = motor->min_angle;  // Ajusta para o ângulo mínimo
    }
    if (motor->max_angle != -1 && new_angle > motor->max_angle) {
        angle = motor->max_angle;  // Ajusta para o ângulo máximo
    }

    // Calcular o número de passos com base no ângulo ajustado
    uint steps = calculate_steps(motor->default_revolution_steps, angle);

    // Define a direção de rotação
    gpio_put(motor->dir_pin, direction);

    // Move o motor o número de passos calculado
    for (int i = 0; i < steps; i++) {
        gpio_put(motor->step_pin, 1);      // Envia pulso de STEP
        sleep_us(delay_us);                 // Delay entre os pulsos
        gpio_put(motor->step_pin, 0);      // Desliga o pulso de STEP
        sleep_us(delay_us);                 // Delay entre os pulsos
    }

    // Atualiza o ângulo atual do motor após o movimento
    motor->current_angle = angle;
}


