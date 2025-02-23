#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include <math.h>

// Definição dos pinos do Joystick
#define JOYSTICK_X_PIN  26
#define JOYSTICK_Y_PIN  27
#define JOYSTICK_BUTTON_PIN  8

// Função para inicializar o joystick
void init_joystick();

void read_joystick(uint16_t *x, uint16_t *y);

// Função para ler a posição do joystick e retornar o ângulo e a direção
void get_joystick_angle_and_direction(bool *direction_x, float *angle_x, bool *direction_y, float *angle_y);

#endif // JOYSTICK_H
