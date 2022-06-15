/**
 * @file main.cpp
 * @author Thomas DUCLOS - KEBE Ibrahim - VIDAL Hugo - FEVE Quentin
 * @brief This is the main file of the project, with the setup and loop function
 * @version 0.1
 * @date 2022-02-21
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <Arduino.h>
#include <Servo.h>
#include "ws.h"
#include "global.h"
#include "PID.h"

#define KP_MOTOR_PID 0.1
#define KI_MOTOR_PID 1
#define KD_MOTOR_PID 0.1

/// variables Declaration
motor *motorA, *motorB;          /// Both motors
captor *captorA, *captorB;       /// Both Captors
SimpleTimer SpeedPrinting_timer; /// calcul timer
SimpleTimer SendData_timer;      /// ws timer
ESP8266WiFi wifis(true, MDNS_NAME);
///WebServer server(100);
PID pid_motorA;
PID pid_motorB;
float kp_A = KP_MOTOR_PID, ki_A = KI_MOTOR_PID, kd_A = KD_MOTOR_PID;
float kp_B = KP_MOTOR_PID, ki_B = KI_MOTOR_PID, kd_B = KD_MOTOR_PID;

uint16_t port = 1063;
WiFiServer WiFiServ(port);

Servo flag;
long long start = 0;

/// Declaration of the function
void IRAM_ATTR ISR_IncreaseCaptorBCount();
void IRAM_ATTR ISR_IncreaseCaptorACount();
void printSpeedMotors();

void setup()
{
  /// Start the communication series with a 115200 baudrates which is the basic baudrate for ESP8266 cards
  Serial.begin(115200UL);

  /// Setup the multiple wifi written in secret.h
  wifis.setup();
  flag.attach(15U);
  //server.setup();

  /// Initialisation of every pin as INPUT or OUTPUT
  pinMode(MOTOR1_DIR_PIN, OUTPUT);
  pinMode(MOTOR2_DIR_PIN, OUTPUT);
  pinMode(MOTOR1_SPD_PIN, OUTPUT);
  pinMode(MOTOR2_SPD_PIN, OUTPUT);
  pinMode(LED_STATE_MOTOR1, OUTPUT);
  pinMode(LED_STATE_MOTOR2, OUTPUT);
  pinMode(CAPTOR_MOT1_PIN, INPUT_PULLUP);
  pinMode(CAPTOR_MOT2_PIN, INPUT_PULLUP);

  /// Initialisation of both motors
  motorA = InitMotor(MOTOR1_DIR_PIN, MOTOR1_SPD_PIN);
  motorB = InitMotor(MOTOR2_DIR_PIN, MOTOR2_SPD_PIN);

  /// Initialisation of both captors
  captorA = InitCaptor(CAPTOR_MOT1_PIN);
  captorB = InitCaptor(CAPTOR_MOT2_PIN);

  /// Declaration of both Interrupt function
  attachInterrupt(digitalPinToInterrupt(CAPTOR_MOT1_PIN), ISR_IncreaseCaptorACount, RISING);
  attachInterrupt(digitalPinToInterrupt(CAPTOR_MOT2_PIN), ISR_IncreaseCaptorBCount, RISING);

  /// Initialisation of the timer to print and calcul the motors's speed.
  SpeedPrinting_timer.setInterval(1000, printSpeedMotors);
  /// Initialisation of the timer to send the speed of both motors to all clients
  ///SendData_timer.setInterval(100, sendCurrentsSpeed);

#if MOTOR_TEST == true
  motorA->actualSpeed = 1.7;
  motorB->actualSpeed = 1.7;
  motorA->state = ON;
  motorB->state = ON;
#endif
  pid_motorA.setKp(kp_A);
  pid_motorA.setKi(ki_A);
  pid_motorA.setKd(kd_A);
  pid_motorB.setKp(kp_B);
  pid_motorB.setKi(ki_B);
  pid_motorB.setKd(kd_B);
  WiFiServ.begin();
  flag.write(0);
}

bool setFlag = false;

void loop()
{
  WiFiClient client = WiFiServ.available();
  if(client) {
    if(client.connected())
    {
      
      Serial.println("Client Connected.");
      
    }
    while(client.connected())
    {
      if(millis() - start >= 90000)
      { 
        motorA->state = OFF;
        motorB->state = OFF;
        if(!setFlag)
        {
          setFlag = true;
          flag.write(180);
        }
      }
      char data = '1';
      while(client.available()>0){
        // read data from the connected client
        data = client.read(); 
      }
      if(millis() - start >= 90000)
      { 
        motorA->state = OFF;
        motorB->state = OFF;
      }
      else 
      {
        switch (data)
        {
        case 'I':
          motorA->state = ON;
          motorB->state = ON;
          start = millis();
          break;
        case '0':
          motorA->state = OFF;
          motorB->state = OFF;
          break;
        case 'Z':
        case 'z':
          motorA->state = ON;
          motorB->state = ON;
          MooveForward(motorA, 255);
          MooveForward(motorB, 255);
          break;
        case 'Q':
        case 'q':
          motorA->state = ON;
          motorB->state = ON;
          MooveForward(motorA, 0);
          MooveForward(motorB, 255);
          break;
        case 'S':
        case 's':
          motorA->state = ON;
          motorB->state = ON;
          MooveBackward(motorA, 255);
          MooveBackward(motorB, 255);
          break;
        case 'D':
        case 'd':
          motorA->state = ON;
          motorB->state = ON;
          MooveForward(motorA, 255);
          MooveForward(motorB, 0);
          break;
        
        default:
          break;
        }
      }
      
      Moove(*motorA);
      Moove(*motorB);
      SpeedPrinting_timer.run(); /// Need to be called to make the timer works
      SendData_timer.run();
      #if MOTOR_TEST == false
        wifis.run();
        //server.run();
      #endif
      digitalWrite(LED_STATE_MOTOR1, motorA->state);
      digitalWrite(LED_STATE_MOTOR2, motorB->state);
    }
    motorA->state = OFF;
    motorB->state = OFF;
  }
  SpeedPrinting_timer.run(); /// Need to be called to make the timer works
  SendData_timer.run();
  wifis.run();
}
/**
 * @brief ISR function to be call inside Interrupt function to increase the hole count of the A captor
 *
 */
void IRAM_ATTR ISR_IncreaseCaptorACount()
{
  captorA->count++;
}

/**
 * @brief ISR function to be call inside Interrupt function to increase the hole count of the B captor
 *
 */
void IRAM_ATTR ISR_IncreaseCaptorBCount()
{
  captorB->count++;
}

/**
 * @brief Function to use inside the Simple timer. It will calcul the speed for each motors from the captors's data.
 * Then it will print this speed for each motors. And finally it will reset both captors.
 *
 */
void printSpeedMotors()
{
  /// Calcul both speeds from captor data
  motorA->speed = ((float)captorA->count / (float)CAPTOR_HOLES_NB);
  motorB->speed = ((float)captorB->count / (float)CAPTOR_HOLES_NB);
#if MOTOR_TEST == true
  Serial.print("motor A speed : "); Serial.println(motorA->speed);
  Serial.print("motor B speed : "); Serial.println(motorB->speed);
  Serial.print("\n\n");
#endif
/// print speed for both motors
#if DEBUG
  PrintMotorSpeed(*motorA, "A");
  PrintMotorSpeed(*motorB, "B");
#endif
#if MOTOR_TEST == false
  /// calcul both corrected speed with pid
  motorA->actualSpeed = pid_motorA.subjugationFunction(motorA->state, motorA->speed, motorA->wantedSpeed);
  motorB->actualSpeed = pid_motorB.subjugationFunction(motorB->state, motorB->speed, motorB->wantedSpeed);
#endif
  /// Reset Captors data
  ResetCaptor(captorA);
  ResetCaptor(captorB);
}

/**
 * @brief Send the current calculated speed of both motors to all clients accepted on the webSocket.
 *
 */
void sendCurrentsSpeed()
{
  /// default size for our object
  const size_t CAPACITY = JSON_OBJECT_SIZE(2);
  StaticJsonDocument<CAPACITY> doc;

  // create our obejct
  JsonObject object = doc.to<JsonObject>();
  /// fill our object
  object["CurrentSpeedA"] = motorA->speed;
  object["CurrentSpeedB"] = motorB->speed;
  /// serialize our object
  String Data;
  serializeJson(object, Data);
  /// send our object to all clients
  //notifyClients(Data, &(const_cast<AsyncWebSocket &>(server.getWS())));
}