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
#include "PID.h"

#define KP_MOTOR_PID 0.1
#define KI_MOTOR_PID 1
#define KD_MOTOR_PID 0.1

///variables Declaration
motor *motorA, *motorB; ///Both motors
captor *captorA, *captorB; ///Both Captors
SimpleTimer SpeedPrinting_timer; ///calcul timer
SimpleTimer SendData_timer; ///ws timer
ESP8266WiFi wifis(true, MDNS_NAME);
WebServer server(100);
PID pid_motorA(0,0,0);
PID pid_motorB(0,0,0);
float kp_A = 0, ki_A = 0, kd_A = 0;
float kp_B = 0, ki_B = 0, kd_B = 0;

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
  SpeedPrinting_timer.setInterval(100, printSpeedMotors);
  ///Initialisation of the timer to send the speed of both motors to all clients
  SendData_timer.setInterval(100, sendCurrentsSpeed);
}

void loop() {
  Moove(*motorA);
  Moove(*motorB);
  SpeedPrinting_timer.run(); ///Need to be called to make the timer works
  SendData_timer.run();
  wifis.run();
  server.run();
  calculSpeed(10.0f,1.0f);
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
 * @brief Function to use inside the Simple timer. It will calcul the speed for each motors from the captors's data.
 * Then it will print this speed for each motors. And finally it will reset both captors.
 * 
 */
void printSpeedMotors() {
  pid_motorA.setKp(kp_A);
  pid_motorA.setKi(ki_A);
  pid_motorA.setKd(kd_A);
  pid_motorB.setKp(kp_B);
  pid_motorB.setKi(ki_B);
  pid_motorB.setKd(kd_B);
  ///Calcul both speeds from captor data
  motorA->speed = ( (float)captorA->count/(float)CAPTOR_HOLES_NB);
  motorB->speed = ( (float)captorB->count/(float)CAPTOR_HOLES_NB);
  ///print speed for both motors
  #if DEBUG
    PrintMotorSpeed(*motorA, "A");
    PrintMotorSpeed(*motorB, "B");
  #endif
  ///calcul both corrected speed with pid
  motorA->actualSpeed = pid_motorA.subjugationFunction(motorA->state, motorA->speed, motorA->wantedSpeed);
  motorB->actualSpeed = pid_motorB.subjugationFunction(motorB->state, motorB->speed, motorB->wantedSpeed);
  //Serial.println(motorA->actualSpeed);
  ///Reset Captors data
  ResetCaptor(captorA);
  ResetCaptor(captorB);
}

/**
 * @brief Send the current calculated speed of both motors to all clients accepted on the webSocket.
 * 
 */
void sendCurrentsSpeed() {
  ///default size for our object
  const size_t CAPACITY = JSON_OBJECT_SIZE(2);
  StaticJsonDocument<CAPACITY> doc;

  // create our obejct
  JsonObject object = doc.to<JsonObject>();
  ///fill our object
  object["CurrentSpeedA"] = motorA->actualSpeed;
  object["CurrentSpeedB"] = motorB->actualSpeed;
  ///serialize our object
  String Data;
  serializeJson(object, Data);
  ///send our object to all clients
  notifyClients(Data, &(const_cast<AsyncWebSocket&>(server.getWS())));
}