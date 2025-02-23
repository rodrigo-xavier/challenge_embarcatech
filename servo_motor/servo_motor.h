// servo_motor/servo_motor.h
#ifndef SERVO_MOTOR_H
#define SERVO_MOTOR_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define SERVO_PIN 17

void init_servo(uint pin);
void move_servo(uint pin, int angle);
void open_servo(uint pin);
void close_servo(uint pin);

#endif // SERVO_MOTOR_H
