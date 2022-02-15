#include <Arduino.h>

int captor = 15; ///Pin of the optical fork 

void setup() {
  Serial.begin(115200); ///Start of the serie communication
  pinMode(captor, INPUT_PULLUP);
}

void loop() {
  Serial.print("Value Read : ");
  Serial.println( digitalRead(captor) );
}