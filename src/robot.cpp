/**
 * @file robot.cpp
 * @author your name (you@domain.com)
 * @brief This file contain all the function relative to the robot.h header. It represents the first part of the project.
 * @version 0.1
 * @date 2022-02-21
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "robot.h"

/**
 * @brief Function to init a captor stucture
 *
 * @param pin Digital pin of the captor
 * @return captor structure which has been created
 */
captor *InitCaptor(uint8_t pin)
{
    captor *x = (captor *)malloc(sizeof(captor));

    x->pin = pin;
    /// Init both counter to 0
    x->count = 0;
    x->time = 0UL;

    return x;
}

/**
 * @brief Function which reset the count inside the captor struct
 *
 * @param x pointer towards the captor structure
 */
void ResetCaptor(captor *x)
{
    x->count = 0;       /// reset hole count to 0
    x->time = millis(); /// reset captor time to actual time
}

/**
 * @brief Function which calculs the distance traveled by the robot
 *
 * @param holesCounter Number of holes detected in the last period
 * @return float the distance traveled in cm
 */
[[deprecated("Not working with the current project's direction")]] float calculDistance(int holesCounter)
{
    return HOLE_ADVANCE_CM * (float)holesCounter;
}

/**
 * @brief Function which calculs the speed of the robot
 *
 * @param distance Distance traveled in the last period
 * @param time Time of the last period
 * @return float the last period robot's speed
 */
[[deprecated("Not working with the current project's direction")]] float calculSpeed(float distance, float time)
{
    return distance / time;
}