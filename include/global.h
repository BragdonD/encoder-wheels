/**
 * @file global.h
 * @author DUCLOS Thomas
 * @brief This file contain all the extern declaration for the global variables
 * @version 0.1
 * @date 2022-03-21
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#if !defined(GLOBAL_H)
#define GLOBAL_H
#include "ws.h"
#include <SimpleTimer.h>
#include "connection.h"
#include "robot.h"
#include "motor.h"


///Variable Declaration
extern motor *motorA, *motorB; ///Both motors
extern captor *captorA, *captorB; ///Both Captors
extern SimpleTimer SpeedPrinting_timer; ///Timer for speed calcul
extern float kp_A, ki_A, kd_A;
extern float kp_B, ki_B, kd_B;

#endif // GLOBAL_H
