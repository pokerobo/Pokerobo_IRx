#include <Arduino.h>
#include "Pedestal_Handler.h"

PedestalHandler::PedestalHandler(int hPin, int vPin, bool _debugEnabled) {
  count = 0;
  horizontalServoPin = (hPin > 0) ? hPin : HORIZONTAL_SERVO_PIN;
  verticalServoPin = (vPin > 0) ? vPin : VERTICAL_SERVO_PIN;
  debugEnabled = _debugEnabled;
  if (debugEnabled) {
    Serial.print("PedestalHandler::PedestalHandler() - hPin: ");
    Serial.print(hPin);
    Serial.print(" => horizontalServoPin: ");
    Serial.println(horizontalServoPin);
    Serial.print("PedestalHandler::PedestalHandler() - vPin: ");
    Serial.print(vPin);
    Serial.print(" => verticalServoPin: ");
    Serial.println(verticalServoPin);
  }
}

void PedestalHandler::begin(int hMinAngle, int hMaxAngle, int vMinAngle, int vMaxAngle) {
  if (debugEnabled) {
    Serial.print("PedestalHandler::begin() - hMinAngle: ");
    Serial.println(hMinAngle);
    Serial.print("PedestalHandler::begin() - hMaxAngle: ");
    Serial.println(hMaxAngle);
    Serial.print("PedestalHandler::begin() - vMinAngle: ");
    Serial.println(vMinAngle);
    Serial.print("PedestalHandler::begin() - vMaxAngle: ");
    Serial.println(vMaxAngle);
  }
  //
  horizontalServo.attach(horizontalServoPin);
  horizontalMinAngle = (hMinAngle < 0) ? 0 : hMinAngle;
  horizontalMaxAngle = (hMaxAngle > 180) ? 180 : hMaxAngle;
  //
  verticalServo.attach(verticalServoPin);
  verticalMinAngle = (vMinAngle < 0) ? 0 : vMinAngle;
  verticalMaxAngle = (vMaxAngle > 180) ? 180 : vMaxAngle;
  //
  reset();
}

void PedestalHandler::reset() {
  setHorizontalPosition( (horizontalMaxAngle + horizontalMinAngle) / 2);
  setVerticalPosition(verticalMinAngle);
}

int PedestalHandler::getHorizontalPosition() {
  return horizontalServo.read();
}

void PedestalHandler::setHorizontalPosition(int hPos) {
  horizontalServo.write(hPos);
}

int PedestalHandler::updateHorizontalPosition(int hPos, int hCurrentPos) {
  if (hPos < horizontalMinAngle) {
    hPos = horizontalMinAngle;
  }
  if (hPos > horizontalMaxAngle) {
    hPos = horizontalMaxAngle;
  }
  //
  if (hCurrentPos < 0) {
    hCurrentPos = getHorizontalPosition();
  }
  if (hPos != hCurrentPos) {
    setHorizontalPosition(hPos);
  }
  //
  return hPos - hCurrentPos;
}

int PedestalHandler::getVerticalPosition() {
  return verticalServo.read();
}

void PedestalHandler::setVerticalPosition(int vPos) {
  verticalServo.write(vPos);
}

int PedestalHandler::updateVerticalPosition(int vPos, int vCurrentPos) {
  if (vPos < verticalMinAngle) {
    vPos = verticalMinAngle;
  }
  if (vPos > verticalMaxAngle) {
    vPos = verticalMaxAngle;
  }
  //
  if (vCurrentPos < 0) {
    vCurrentPos = getVerticalPosition();
  }
  if (vPos != vCurrentPos) {
    setVerticalPosition(vPos);
  }
  //
  return vPos - vCurrentPos;
}

bool PedestalHandler::syncWith(PedestalHandler master) {
  int hDelta = updateHorizontalPosition(master.getHorizontalPosition());
  int vDelta = updateVerticalPosition(master.getVerticalPosition());
  return (hDelta != 0) || (vDelta != 0);
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
    setHorizontalPosition(pos);
    setVerticalPosition(pos);
    Serial.print("Pos= ");
    Serial.print(pos);
    Serial.println();
    delay(100);
  }
  //
  for (int pos = 180; pos >= 0; pos -= 5) {
    setHorizontalPosition(pos);
    setVerticalPosition(pos);
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

int PedestalHandler::changeHorizontalServo(int hDelta) {
  if (hDelta == 0) {
    if (debugEnabled) {
      Serial.print("PedestalHandler::changeVerticalServo() - vDelta is 0, do nothing");
    }
    return false;
  }
  int hCurrentPos = getHorizontalPosition();
  int hPos = hCurrentPos;
  //
  if (debugEnabled) {
    Serial.print("PedestalHandler::changeHorizontalServo() - hDelta: ");
    Serial.print(hDelta);
    Serial.print("; hPos: ");
    Serial.println(hPos);
  }
  //
  if (hDelta < 0 && hPos <= horizontalMinAngle) {
    return false;
  }
  if (hDelta > 0 && hPos >= horizontalMaxAngle) {
    return false;
  }
  hPos += hDelta;
  //
  return updateHorizontalPosition(hPos, hCurrentPos);
}

int PedestalHandler::changeVerticalServo(int vDelta) {
  if (vDelta == 0) {
    if (debugEnabled) {
      Serial.print("PedestalHandler::changeVerticalServo() - vDelta is 0, do nothing");
    }
    return false;
  }
  int vCurrentPos = getVerticalPosition();
  int vPos = vCurrentPos;
  //
  if (debugEnabled) {
    Serial.print("PedestalHandler::changeVerticalServo() - vDelta: ");
    Serial.println(vDelta);
    Serial.print("; vPos: ");
    Serial.println(vPos);
  }
  //
  if (vDelta < 0 && vPos <= verticalMinAngle) {
    return false;
  }
  if (vDelta > 0 && vPos >= verticalMaxAngle) {
    return false;
  }
  vPos += vDelta;
  //
  return updateVerticalPosition(vPos, vCurrentPos);
}

bool PedestalHandler::change(int hDelta, int vDelta) {
  if (debugEnabled) {
    Serial.print("PedestalHandler::change() - ");
    Serial.print("hDelta: ");
    Serial.print(hDelta);
    Serial.print("vDelta: ");
    Serial.print(vDelta);
    Serial.println();
  }
  int hChanged = changeHorizontalServo(hDelta);
  int vChanged = changeVerticalServo(vDelta);
  return (hChanged != 0) || (vChanged != 0);
}
