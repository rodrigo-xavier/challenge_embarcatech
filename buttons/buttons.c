#include "buttons.h"


void init_buttons(){
    gpio_init(BUTTON_A_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);

    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_B_PIN);
}

// Função de debounce para o botão
bool debounce_button(uint pin) {
    static uint32_t last_press_time_A = 0;
    static uint32_t last_press_time_B = 0;

    uint32_t *last_press_time = (pin == BUTTON_A_PIN) ? &last_press_time_A : &last_press_time_B;
    uint32_t current_time = to_ms_since_boot(get_absolute_time());

    if (gpio_get(pin) == 0) { // Botão pressionado
        if (current_time - *last_press_time > DEBOUNCE_DELAY) {
            *last_press_time = current_time;
            return true;
        }
    }
    return false;
}

