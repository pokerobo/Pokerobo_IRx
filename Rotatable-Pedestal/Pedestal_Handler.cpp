#include <Arduino.h>
#include "Pedestal_Handler.h"

#define MOVING_AMOUNT      2

PedestalHandler::PedestalHandler() {
  count = 0;
}

void PedestalHandler::begin() {
  horizontalServo.attach(HORIZONTAL_SERVO_PIN);
  horizontalServo.write(90);
  verticalServo.attach(VERTICAL_SERVO_PIN);
  verticalServo.write(0);
}

void PedestalHandler::test() {
  if (count >= 3) {
    Serial.print(count);
    Serial.print(" rounds are done!");
    return;
  }
  count++;
  //
  for (int pos = 0; pos <= 180; pos += 5) {
    horizontalServo.write(pos);
    verticalServo.write(pos);
    Serial.print("Pos= ");
    Serial.print(pos);
    Serial.println();
    delay(100);
  }
  //
  for (int pos = 180; pos >= 0; pos -= 5) {
    horizontalServo.write(pos);
    verticalServo.write(pos);
    Serial.print("Pos= ");
    Serial.print(pos);
    Serial.println();
    delay(100);
  }
}

bool PedestalHandler::verticalServoUp() {
  int vPos = verticalServo.read();
  if (vPos >= 180) {
    return false;
  }
  vPos += MOVING_AMOUNT;
  if (vPos > 180) {
    vPos = 180;
  }
  verticalServo.write(vPos);
  return true;
}

bool PedestalHandler::verticalServoDown() {
  int vPos = verticalServo.read();
  if (vPos <= 0) {
    return false;
  }
  vPos -= MOVING_AMOUNT;
  if (vPos < 0) {
    vPos = 0;
  }
  verticalServo.write(vPos);
  return true;
}

bool PedestalHandler::horizontalServoLeft() {
  int hPos = horizontalServo.read();
  if (hPos >= 180) {
    return false;
  }
  hPos += MOVING_AMOUNT;
  if (hPos > 180) {
    hPos = 180;
  }
  horizontalServo.write(hPos);
  return true;
}

bool PedestalHandler::horizontalServoRight() {
  int hPos = horizontalServo.read();
  if (hPos <= 0) {
    return false;
  }
  hPos -= MOVING_AMOUNT;
  if (hPos < 0) {
    hPos = 0;
  }
  horizontalServo.write(hPos);
  return true;
}
