#include "PS2_Controller.h"

#include <Servo.h>

#define HORIZONTAL_SERVO_PIN 8
#define VERTICAL_SERVO_PIN 9

Servo horizontalServo;
Servo verticalServo;

int count = 0;
int pos = 0;

void setup() {
  Serial.begin(57600);
  Serial.println("Starting ...");
  //
  initializePS2();
  //
  horizontalServo.attach(HORIZONTAL_SERVO_PIN);
  verticalServo.attach(VERTICAL_SERVO_PIN);
  //
  Serial.println("Bootstrap has done!");
}

void loop() {
  if (count >= 3) {
    Serial.print(count);
    Serial.print(" rounds are done!");
    return;
  }
  count++;
  //
  for ( pos = 0; pos <= 180; pos += 5) {
    horizontalServo.write(pos);
    verticalServo.write(pos);
    Serial.print("Pos= ");
    Serial.print(pos);
    Serial.println();
    delay(100);
  }
  //
  for ( pos = 180; pos >= 0; pos -= 5) {
    horizontalServo.write(pos);
    verticalServo.write(pos);
    Serial.print("Pos= ");
    Serial.print(pos);
    Serial.println();
    delay(100);
  }
  //
  delay(500);
}