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

// Função para controlar os motores com o joystick
void control_stepper_by_joystick(StepperMotor *motor_x, StepperMotor *motor_y) {
    bool direction_x, direction_y;
    float angle_x, angle_y;

    // Obtém os valores do joystick
    get_joystick_angle_and_direction(&direction_x, &angle_x, &direction_y, &angle_y);

    // Define um delay padrão para controle do motor
    int delay_us = 2000;

    // Move o motor X conforme os valores do joystick
    if (angle_x > 5.0) {  // Define um limite mínimo para evitar pequenas variações
        move_stepper_motor(motor_x, direction_x, (uint)angle_x, delay_us);
    }

    // Move o motor Y conforme os valores do joystick
    if (angle_y > 5.0) {  // Define um limite mínimo para evitar pequenas variações
        move_stepper_motor(motor_y, direction_y, (uint)angle_y, delay_us);
    }
}





// Função para desenhar um triângulo
void draw_triangle(StepperMotor *motor_x, StepperMotor *motor_y, int side_length) {
    for (int i = 0; i < 3; i++) {
        move_stepper_motor(motor_x, 1, side_length, 4000);  // Move para frente
        move_stepper_motor(motor_y, 1, side_length, 4000);  // Move para o lado
        move_stepper_motor(motor_x, 0, side_length, 4000);  // Move para trás
        move_stepper_motor(motor_y, 0, side_length, 4000);  // Move para o lado
    }
}

// Função para desenhar um quadrado
void draw_square(StepperMotor *motor_x, StepperMotor *motor_y, int side_length) {
    for (int i = 0; i < 4; i++) {
        move_stepper_motor(motor_x, 1, side_length, 4000);  // Move para frente
        move_stepper_motor(motor_y, 1, side_length, 4000);  // Move para o lado
        move_stepper_motor(motor_x, 0, side_length, 4000);  // Move para trás
        move_stepper_motor(motor_y, 0, side_length, 4000);  // Move para o lado
    }
}

// Função para desenhar um círculo
void draw_circle(StepperMotor *motor_x, StepperMotor *motor_y, int radius) {
    int steps = 360;  // Número de passos para um círculo completo
    for (int i = 0; i < steps; i++) {
        float angle = i * (2 * M_PI / steps);  // Calcula o ângulo de cada passo
        int x = radius * cos(angle);           // Calcula a posição X
        int y = radius * sin(angle);           // Calcula a posição Y

        move_stepper_motor(motor_x, 1, x, 4000); // Move no eixo X
        move_stepper_motor(motor_y, 1, y, 4000); // Move no eixo Y
    }
}

void draw_random(StepperMotor *motor_x, StepperMotor *motor_y) {
    // Semente aleatória
    srand(time(NULL));

    // Gera um número aleatório de passos entre 100 e 500
    int random_steps_x = rand() % 401 + 100;  // Número de passos aleatório para o motor X (entre 100 e 500)
    int random_steps_y = rand() % 401 + 100;  // Número de passos aleatório para o motor Y (entre 100 e 500)

    // Gera um valor aleatório para a direção de cada motor (1 = para frente, 0 = para trás)
    bool direction_x = rand() % 2;  // Direção aleatória para o motor X
    bool direction_y = rand() % 2;  // Direção aleatória para o motor Y

    // Gera um valor aleatório para o delay entre os passos (simulando a velocidade do movimento)
    int delay_us = 5000;

    // Move os motores X e Y pelos passos aleatórios
    move_stepper_motor(motor_x, direction_x, random_steps_x, delay_us);
    move_stepper_motor(motor_y, direction_y, random_steps_y, delay_us);
}

// Função para desenhar formas
void draw(StepperMotor *motor_x, StepperMotor *motor_y, int shape, int size) {
    switch (shape) {
        case 1:
            draw_circle(motor_x, motor_y, size);    // Desenha um círculo
            break;
        case 2:
            draw_triangle(motor_x, motor_y, size);  // Desenha um triângulo
            break;
        case 3:
            draw_square(motor_x, motor_y, size);    // Desenha um quadrado
            break;
        case 4:
            draw_random(motor_x, motor_y);    // Desenha um quadrado
            break;
        default:
            printf("Forma desconhecida\n");
            break;
    }
}

