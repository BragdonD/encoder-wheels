#if !defined(GLOBAL_H)
#define GLOBAL_H

#include <SimpleTimer.h>
#include "connection.h"
#include "robot.h"
#include "motor.h"

///Variable Declaration
extern motor *motorA, *motorB; ///Both motors
extern captor *captorA, *captorB; ///Both Captors
extern SimpleTimer SpeedPrinting_timer; ///Timer for speed calcul
extern SimpleTimer Time;
extern ESP8266WiFi wifis(true, MDNS_NAME);
extern const int holeFrequence_test = 50;
extern int hole_frequence;
extern int Counter_of_hole_frequence;
extern PID Subjugation;
#endif // GLOBAL_H
