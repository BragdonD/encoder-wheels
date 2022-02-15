#if !defined(MOTOR_H)
#define MOTOR_H

#include <Arduino.h>

typedef struct motor
{
    uint8_t pin;
    bool state;
    int vitesse;
};


#endif // MOTOR_H
