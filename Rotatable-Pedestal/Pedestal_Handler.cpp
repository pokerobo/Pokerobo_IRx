#include <Arduino.h>
#include "Pedestal_Handler.h"

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
  return changeVerticalServo(MOVING_AMOUNT);
}

bool PedestalHandler::verticalServoDown() {
  return changeVerticalServo(-MOVING_AMOUNT);
}

bool PedestalHandler::horizontalServoLeft() {
  return changeHorizontalServo(MOVING_AMOUNT);
}

bool PedestalHandler::horizontalServoRight() {
  return changeHorizontalServo(-MOVING_AMOUNT);
}

bool PedestalHandler::changeHorizontalServo(int hDelta) {
  if (hDelta == 0) {
    return false;
  }
  int hPos = horizontalServo.read();
  //
  Serial.print("changeHorizontalServo, hDelta: ");
  Serial.print(hDelta);
  Serial.print("; hPos: ");
  Serial.println(hPos);
  //
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
  Serial.print("changeVerticalServo: ");
  Serial.println(vDelta);
  //
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
  Serial.print("hDelta: ");
  Serial.print(hDelta);
  Serial.print("vDelta: ");
  Serial.print(vDelta);
  Serial.println();
  bool hChanged = changeHorizontalServo(hDelta);
  bool vChanged = changeVerticalServo(vDelta);
  return hChanged || vChanged;
}
