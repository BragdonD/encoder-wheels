/**
 * @file main.cpp
 * @author Thomas DUCLOS
 * @brief This is the main file of the project, with the setup and loop function
 * @version 0.1
 * @date 2022-02-21
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <Arduino.h>
#include "ws.h"
#include "global.h"

///variable Declaration
motor *motorA, *motorB; ///Both motors
captor *captorA, *captorB; ///Both Captors
SimpleTimer SpeedPrinting_timer;
ESP8266WiFi wifis(true, MDNS_NAME);
WebServer server(100);

///Declaration of the function
void IRAM_ATTR ISR_IncreaseCaptorBCount();
void IRAM_ATTR ISR_IncreaseCaptorACount();
void printSpeedMotors();

void setup() {
  ///Start the communication series with a 115200 baudrates which is the basic baudrate for ESP8266 cards
  Serial.begin( 115200UL ); 

  ///Setup the multiple wifi written in secret.h
  wifis.setup();
  
  server.setup();
  
  ///Initialisation of every pin as INPUT or OUTPUT
  pinMode( MOTOR1_DIR_PIN, OUTPUT );
  pinMode( MOTOR2_DIR_PIN, OUTPUT );
  pinMode( MOTOR1_SPD_PIN, OUTPUT );
  pinMode( MOTOR2_SPD_PIN, OUTPUT );
  pinMode( CAPTOR_MOT1_PIN, INPUT_PULLUP );
  pinMode( CAPTOR_MOT2_PIN, INPUT_PULLUP );

  ///Initialisation of both motors
  motorA = InitMotor( MOTOR1_DIR_PIN, MOTOR1_SPD_PIN );
  motorB = InitMotor( MOTOR2_DIR_PIN, MOTOR2_SPD_PIN );

  ///Initialisation of both captors
  captorA = InitCaptor( CAPTOR_MOT1_PIN );
  captorB = InitCaptor( CAPTOR_MOT2_PIN );

  ///Declaration of both Interrupt function
  attachInterrupt( digitalPinToInterrupt( CAPTOR_MOT1_PIN ), ISR_IncreaseCaptorACount, RISING);
  attachInterrupt( digitalPinToInterrupt( CAPTOR_MOT2_PIN ), ISR_IncreaseCaptorBCount, RISING);

  ///Initialisation of the timer to print and calcul the motors's speed.
  SpeedPrinting_timer.setInterval(1000, printSpeedMotors);
}

void loop() {
  ///SpeedPrinting_timer.run(); ///Need to be called to make the timer works
  wifis.run();
  server.run();
}

/**
 * @brief ISR function to be call inside Interrupt function to increase the hole count of the A captor
 * 
 */
void IRAM_ATTR ISR_IncreaseCaptorACount() {
  captorA->count++;
}

/**
 * @brief ISR function to be call inside Interrupt function to increase the hole count of the B captor
 * 
 */
void IRAM_ATTR ISR_IncreaseCaptorBCount() {
  captorB->count++;
}

/**
 * @brief Function to use inside the Simple timer. It will calcul the speed for each motors from the captors data.
 * Then it will print this speed for each motors. And finally it will reset both captors.
 * 
 */
void printSpeedMotors() {
  ///Calcul of both speed from captor data
  motorA->speed = ( calculSpeed( calculDistance( captorA->count ), millis() - captorA->time ) * 36 );
  motorB->speed = ( calculSpeed( calculDistance( captorB->count ), millis() - captorB->time) * 36 );
  ///print speed for both motors
  PrintMotorSpeed(*motorA, "A");
  PrintMotorSpeed(*motorB, "B");
  ///Reset Captors data
  ResetCaptor(captorA);
  ResetCaptor(captorB);
}