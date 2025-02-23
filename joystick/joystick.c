#include "joystick.h"


// Inicializa o ADC para os pinos do joystick
void init_joystick() {
    adc_init();
    adc_gpio_init(JOYSTICK_X_PIN);
    adc_gpio_init(JOYSTICK_Y_PIN);
}

// Lê os valores X e Y do joystick e retorna em um array
void read_joystick(uint16_t *x, uint16_t *y) {
    adc_select_input(0); // Canal ADC correspondente ao pino X
    *x = adc_read();

    adc_select_input(1); // Canal ADC correspondente ao pino Y
    *y = adc_read();
}

// Calcula ângulos e direções do joystick
void get_joystick_angle_and_direction(bool *direction_x, float *angle_x, bool *direction_y, float *angle_y) {
    uint16_t x_raw, y_raw;
    read_joystick(&x_raw, &y_raw);

    // Converte os valores ADC para um intervalo centrado em 0 (assumindo ADC de 12 bits: 0-4095)
    int x = x_raw - 2048;
    int y = y_raw - 2048;

    // Define direções
    *direction_x = (x >= 0);
    *direction_y = (y >= 0);

    // Calcula ângulos em graus
    *angle_x = fabs((x / 2048.0) * 90.0);
    *angle_y = fabs((y / 2048.0) * 90.0);
}
