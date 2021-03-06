/**
 * @file motor.cpp
 * @author DUCLOS Thomas - KEBE Ibrahim - VIDAL Hugo - FEVE Quentin
 * @brief This file contain all the function relative to the motor.h header. It represents the first part of the project.
 * @version 1.0
 * @date 2022-02-15
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "motor.h"
#include "robot.h"

/**
 * @brief Function to init a motor stucture
 *
 * @param dir_pin Direction pin
 * @param spd_pin Speed pin
 * @return motor structure which has been created
 */
motor *InitMotor(uint8_t dir_pin, uint8_t spd_pin)
{
    motor *x = (motor *)malloc(sizeof(motor));

    x->dir_pin = dir_pin;
    x->spd_pin = spd_pin;
    x->direction = FORWARD;
    x->speed = 0;
    x->wantedSpeed = 0;
    x->state = OFF;
    
    return x;
}

/**
 * @brief This function allows us to go forward at a given speed
 *
 * @param x the motor to change
 * @param speed the speed which we wanna go
 */
void MooveForward(motor *x, float speed)
{
    x->direction = FORWARD;
    if (speed >= 0)
    {
        x->wantedSpeed = speed;
    }
}

/**
 * @brief This function allows us to go backward at a given speed
 *
 * @param x the motor to change
 * @param speed the speed which we wanna go
 */
void MooveBackward(motor *x, float speed)
{
    x->direction = BACKWARD;
    if (speed >= 0)
    {
        x->wantedSpeed = speed;
    }
}

/**
 * @brief This function takes a motor structure as parameter and sett all the pin at the correct value.
 * warning, you need to have set the motor informations before calling this function
 * @param x the motor containing all the informations
 */
void Moove(motor x)
{
    if (x.state == ON)
    {
        if (x.direction == FORWARD)
        {
            digitalWrite(x.dir_pin, HIGH);
        }
        else if (x.direction == BACKWARD)
        {
            digitalWrite(x.dir_pin, LOW);
        }
        analogWrite(x.spd_pin, (x.actualSpeed / 1.7) * 255);
    }
    else
    {
        analogWrite(x.spd_pin, 0);
    }
}

/**
 * @brief Function to print the speed of a motor inside the serial
 *
 * @param x motor to print
 * @param str name of the motor
 */
void PrintMotorSpeed(motor x, const char *str)
{
#if DEBUG
    Serial.print("Speed Motor ");
    Serial.print(str);
    Serial.print(" : ");
    Serial.print(x.speed);
    Serial.println("turn.s^(-1)");
#endif
}

/**
 * @brief Function to turn on the motor
 *
 * @param x motor to change state value
 */
void ActivateMotor(motor *x)
{
    x->state = ON;
}

/**
 * @brief Function to turn off the motor
 *
 * @param x motor to change state value
 */
void DeactivateMotor(motor *x)
{
    x->state = OFF;
}
