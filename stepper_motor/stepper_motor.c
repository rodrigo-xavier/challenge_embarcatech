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

// Função para movimentar um motor de passo
void move_stepper_motor(StepperMotor *motor, bool direction, int steps, int delay) {
    // Verifica se o motor tem limites de ângulo definidos
    if ((motor->min_angle != -1 && direction == true && motor->current_angle <= motor->min_angle) ||
        (motor->max_angle != -1 && direction == false && motor->current_angle >= motor->max_angle)) {
        // Se o motor atingir o limite, não realiza o movimento
        printf("Limite de ângulo alcançado! Movimento interrompido.\n");
        return;
    }

    gpio_put(motor->dir_pin, direction); // Define a direção
    for (int i = 0; i < steps; i++) {
        gpio_put(motor->step_pin, 1);
        sleep_us(delay);
        gpio_put(motor->step_pin, 0);
        sleep_us(delay);

        // Atualiza o ângulo atual
        if (direction == true) {
            motor->current_angle--;  // Movimento para a esquerda diminui o ângulo
        } else {
            motor->current_angle++;  // Movimento para a direita aumenta o ângulo
        }

        // Verifica novamente após cada passo se o limite foi atingido
        if ((motor->min_angle != -1 && motor->current_angle <= motor->min_angle) ||
            (motor->max_angle != -1 && motor->current_angle >= motor->max_angle)) {
            printf("Limite de ângulo alcançado! Movimento interrompido.\n");
            break; // Interrompe o movimento se o limite for atingido
        }
    }
}
