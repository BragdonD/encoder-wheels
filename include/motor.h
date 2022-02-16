#if !defined(MOTOR_H)
#define MOTOR_H

#include <Arduino.h>

///define direction
#define FORWARD 0x1
#define BACKWARD 0x0

///Define motors pin
#define MOTOR1_DIR_PIN 0U ///D3
#define MOTOR2_DIR_PIN 2U ///D4
#define MOTOR1_SPD_PIN 5U ///D1
#define MOTOR2_SPD_PIN 4U ///D2

typedef struct motor
{
    uint8_t dir_pin;    ///direction pin (D3, D4)
    uint8_t spd_pin;    ///speed pin (D1, D2)
    uint8_t direction;  ///direction of the motor (FORWARD, BACKWARD)
    float speed;        ///rotation speed
}t_motor, motor;

void MooveForward (motor *x, float speed);
void MooveBackward (motor *x, float speed);
void Moove (motor x);

#endif // MOTOR_H
