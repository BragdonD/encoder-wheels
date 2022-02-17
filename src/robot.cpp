#include "robot.h"

/**
 * @brief Function which calculs the distance traveled by the robot
 * 
 * @param holesCounter Number of holes detected in the last period
 * @return float the distance traveled
 */
float calculDistance(int holesCounter) {
    return HOLE_ADVANCE_CM * (float)holesCounter;
}

/**
 * @brief Function which calculs the speed of the robot
 * 
 * @param distance Distance traveled in the last period
 * @param time Time of the last period
 * @return float the last period robot's speed
 */
float calculSpeed(float distance, float time) {
    return distance / time;
}