#include "working_led.h"

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
            pwm_set_gpio_level(PICO_DEFAULT_LED_PIN, LED_BRIGHTNESS);  // Usa o brilho ajustável
        } else {
            pwm_set_gpio_level(PICO_DEFAULT_LED_PIN, 0);   // Desliga o LED
        }
    #elif defined(CYW43_WL_GPIO_LED_PIN)
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);
    #endif
}

// Função para controlar o LED de trabalho
void set_working_led(void) {
    pico_set_led(true);
    sleep_ms(LED_DELAY_MS);
    pico_set_led(false);
    sleep_ms(LED_DELAY_MS);
}
