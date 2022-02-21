#include <Arduino.h>
#include <SimpleTimer.h>
#include "robot.h"
#include "motor.h"

motor motorA, motorB;
captor captorA, captorB;

void setup() {
  Serial.begin(115200); ///Start the communication series with a 115200 baudrates which is the basic baudrate for ESP8266 cards
  
  ///Initialisation of every pin as INPUT or OUTPUT
  pinMode(MOTOR1_DIR_PIN, OUTPUT);
  pinMode(MOTOR2_DIR_PIN, OUTPUT);
  pinMode(MOTOR1_SPD_PIN, OUTPUT);
  pinMode(MOTOR2_SPD_PIN, OUTPUT);
  pinMode(CAPTOR_MOT1_PIN, INPUT);
  pinMode(CAPTOR_MOT2_PIN, INPUT);

  motorA = InitMotor( MOTOR1_DIR_PIN, MOTOR1_SPD_PIN );
  motorB = InitMotor( MOTOR2_DIR_PIN, MOTOR2_SPD_PIN );

  captorA = InitCaptor( CAPTOR_MOT1_PIN );
  captorB = InitCaptor( CAPTOR_MOT2_PIN );
}

void loop() {

}