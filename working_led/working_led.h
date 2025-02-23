#ifndef WORKING_LED_H
#define WORKING_LED_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "pico/cyw43_arch.h"

// Pico W devices use a GPIO on the WIFI chip for the LED,
// so when building for Pico W, CYW43_WL_GPIO_LED_PIN will be defined
#ifdef CYW43_WL_GPIO_LED_PIN
#endif
#define PICO_DEFAULT_LED_PIN 11 // GPIO11 para o LED
#define LED_DELAY_MS 100  // Delay para piscar o led
#define LED_BRIGHTNESS 100 * (255/100)  // Brilho ajustável (0 a 255)
#define LED_COLOR RBG     // Defina a cor do LED (exemplo: RGB, R, G, B)


int pico_led_init(void);
void pico_set_led(bool led_on);
void set_working_led(void);

#endif // WORKING_LED_H