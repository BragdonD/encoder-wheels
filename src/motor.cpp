/**
 * @file motor.cpp
 * @author DUCLOS Thomas
 * @brief This file contain all the function relative to the motor.h header. It represents the first part of the project.
 * @version 1.0
 * @date 2022-02-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "motor.h"

/**
 * @brief This function allows us to go forward at a given speed
 * 
 * @param x the motor to change
 * @param speed the speed which we wanna go
 */
void MooveForward (motor *x, float speed) {
    x->direction = FORWARD;
    if(speed >= 0) {
        x->speed = speed;
    }
}

/**
 * @brief This function allows us to go backward at a given speed
 * 
 * @param x the motor to change
 * @param speed the speed which we wanna go
 */
void MooveBackward (motor *x, float speed) {
    x->direction = BACKWARD;
    if(speed >= 0) {
        x->speed = speed;
    }
}

/**
 * @brief This function takes a motor structure as parameter and sett all the pin at the correct value. 
 * warning, you need to have set the motor informations before calling this function
 * @param x the motor containing all the informations
 */
void Moove (motor x) {
    if(x.direction == FORWARD) {
        digitalWrite(x.dir_pin, HIGH);
    }
    else if(x.direction == BACKWARD) {
        digitalWrite(x.dir_pin, LOW);
    }
    analogWrite(x.spd_pin, x.speed);
}