#if !defined(ROBOT_H)
#define ROBOT_H

#include <Arduino.h>

///Define pins
#define CAPTOR_MOT1_PIN 14U ///D5
#define CAPTOR_MOT2_PIN 12U ///D6

///Define material characteristics
#define CAPTOR_HOLES_NB 20

typedef struct captor {
    uint8_t pin; ///digital pin
    int cmpt; ///number of 1 the captor has detected
}t_captor, captor;


#endif // ROBOT_H
