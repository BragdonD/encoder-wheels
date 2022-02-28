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

#endif // GLOBAL_H
