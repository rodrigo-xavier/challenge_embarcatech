#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "pico/stdlib.h"
#include "hardware/timer.h"
#include <stdio.h>


#define BUTTON_A_PIN 5          // GPIO5 para o botão A
#define BUTTON_B_PIN 6          // GPIO6 para o botão B
#define DEBOUNCE_DELAY 200       // Tempo de debounce em milissegundos

struct repeating_timer blink_timer;
volatile bool led_state = false;
volatile bool blinking = false;


void init_buttons();

bool debounce_button(uint pin);

#endif // JOYSTICK_H
