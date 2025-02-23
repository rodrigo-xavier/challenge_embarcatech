// servo_motor/servo_motor.h
#ifndef SERVO_MOTOR_H
#define SERVO_MOTOR_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define SERVO_PIN 17

void init_servo();
void move_servo(int angle);
void open_servo();
void close_servo();

#endif // SERVO_MOTOR_H
