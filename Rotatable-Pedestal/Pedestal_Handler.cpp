#include <Arduino.h>
#include "Pedestal_Handler.h"

#define MOVING_AMOUNT      2

PedestalHandler::PedestalHandler() {
  PedestalHandler(HORIZONTAL_SERVO_PIN, VERTICAL_SERVO_PIN);
}

PedestalHandler::PedestalHandler(int hPin, int vPin) {
  count = 0;
  horizontalServoPin = hPin;
  verticalServoPin = vPin;
}

void PedestalHandler::begin() {
  horizontalServo.attach(horizontalServoPin);
  horizontalServo.write(90);
  verticalServo.attach(verticalServoPin);
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

bool PedestalHandler::changeHorizontalServo(int hDelta) {
  if (hDelta == 0) {
    return false;
  }
  int hPos = horizontalServo.read();
  if (hDelta < 0 && hPos <= 0) {
    return false;
  }
  if (hDelta > 0 && hPos >= 180) {
    return false;
  }
  hPos += hDelta;
  if (hPos < 0) {
    hPos = 0;
  }
  if (hPos > 180) {
    hPos = 180;
  }
  horizontalServo.write(hPos);
  return true;
}

bool PedestalHandler::changeVerticalServo(int vDelta) {
  if (vDelta == 0) {
    return false;
  }
  int vPos = verticalServo.read();
  if (vDelta < 0 && vPos <= 0) {
    return false;
  }
  if (vDelta > 0 && vPos >= 180) {
    return false;
  }
  vPos += vDelta;
  if (vPos < 0) {
    vPos = 0;
  }
  if (vPos > 180) {
    vPos = 180;
  }
  verticalServo.write(vPos);
  return true;
}

bool PedestalHandler::change(int hDelta, int vDelta) {
  bool hChanged = changeHorizontalServo(hDelta);
  bool vChanged = changeVerticalServo(vDelta);
  return hChanged || vChanged;
}
