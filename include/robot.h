/**
 * @file robot.h
 * @author Thomas DUCLOS - KEBE Ibrahim - VIDAL Hugo - FEVE Quentin
 * @brief This file is the header file for all the functions and structures related to the captor
 * @version 0.1
 * @date 2022-02-21
 *
 * @copyright Copyright (c) 2022
 *
 */
#if !defined(ROBOT_H)
#define ROBOT_H

#include <Arduino.h>

/// Define pins
#define CAPTOR_MOT1_PIN 13U /// D5
#define CAPTOR_MOT2_PIN 12U /// D6

/// Define material characteristics
#define CAPTOR_HOLES_NB 20
#define WHEEL_DIAMETER_CM 6.844f
#define WHEEL_PERIMETER_CM 21.5f
#define HOLE_ADVANCE_ANGLE 18.0f
#define HOLE_ADVANCE_CM 1.075f

typedef struct captor
{
    uint8_t pin;                     /// digital pin
    volatile int count;              /// number of HIGH the captor pin has detected in the last period
    volatile unsigned long int time; /// last time the captor data has been reset
} t_captor, captor;

captor *InitCaptor(uint8_t pin);
void ResetCaptor(captor *x);
float calculDistance(int holesCounter);
float calculSpeed(float distance, float time);

#endif // ROBOT_H
