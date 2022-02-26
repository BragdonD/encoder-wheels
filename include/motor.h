/**
 * @file motor.h
 * @author your name (you@domain.com)
 * @brief This file is the header file for all the functions and structures related to the motor
 * @version 0.1
 * @date 2022-02-21
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#if !defined(MOTOR_H)
#define MOTOR_H

#include <Arduino.h>
#include <string>
#include "settings.h"

///define direction
#define FORWARD 0x1
#define BACKWARD 0x0

///define state
#define ON 0x1
#define OFF 0x0

///Define motors pin
#define MOTOR1_DIR_PIN 0U ///D3
#define MOTOR2_DIR_PIN 2U ///D4
#define MOTOR1_SPD_PIN 5U ///D1
#define MOTOR2_SPD_PIN 4U ///D2

typedef struct motor
{
    uint8_t dir_pin;    //direction pin (D3, D4)
    uint8_t spd_pin;    //speed pin (D1, D2)
    uint8_t direction;  //direction of the motor (FORWARD, BACKWARD)
    uint8_t state; //state of the motor (ON, OFF)
    volatile float speed;        //rotation speed
}t_motor, motor;

motor *InitMotor (uint8_t dir_pin, uint8_t  spd_pin);
void MooveForward (motor *x, float speed);
void MooveBackward (motor *x, float speed);
void Moove (motor x);
void PrintMotorSpeed(motor x, const char* str);
void ActivateMotor (motor *x);
void DeactivateMotor (motor *x);

#endif // MOTOR_H
