#include <Arduino.h>
#include "Pedestal_Handler.h"

#ifdef __HCPCA9685_ENABLED__
/* I2C slave address for the device/module. For the HCMODU0097 the default I2C address is 0x40 */
#define  I2C_ADDR             0x40

/* Create an instance of the library */
HCPCA9685 hcpca9685(I2C_ADDR);
#endif

static void PedestalHandler::init() {
#ifdef __HCPCA9685_ENABLED__
  /* Initialise the library and set it to 'servo mode' */ 
  hcpca9685.Init(SERVO_MODE);

  /* Wake the device up */
  hcpca9685.Sleep(false);
#endif
}

PedestalHandler::PedestalHandler(byte hPin, byte vPin, bool _debugEnabled) {
  count = 0;
  horizontalServoPin = (hPin >= 0) ? hPin : HORIZONTAL_SERVO_PIN;
  horizontalServoPos = 0;
  verticalServoPin = (vPin >= 0) ? vPin : VERTICAL_SERVO_PIN;
  verticalServoPos = 0;
  debugEnabled = _debugEnabled;
#if (__LOADING_LOG_ENABLED__)
  if (debugEnabled) {
    Serial.print("PedestalHandler::");
    Serial.print("PedestalHandler() - ");
    Serial.print("hPin: ");
    Serial.print(hPin);
    Serial.print(" => ");
    Serial.println(horizontalServoPin);
    Serial.print("PedestalHandler::");
    Serial.print("PedestalHandler() - ");
    Serial.print("vPin: ");
    Serial.print(vPin);
    Serial.print(" => ");
    Serial.println(verticalServoPin);
  }
#endif
}

void PedestalHandler::begin(int hMinAngle, int hMaxAngle, int vMinAngle, int vMaxAngle) {
#if (__LOADING_LOG_ENABLED__)
  if (debugEnabled) {
    Serial.print("PedestalHandler::");
    Serial.println("begin():");
    Serial.print(" - hMinAngle: ");
    Serial.println(hMinAngle);
    Serial.print(" - hMaxAngle: ");
    Serial.println(hMaxAngle);
    Serial.print(" - vMinAngle: ");
    Serial.println(vMinAngle);
    Serial.print(" - vMaxAngle: ");
    Serial.println(vMaxAngle);
  }
#endif
  //
#ifndef __HCPCA9685_ENABLED__
  horizontalServo.attach(horizontalServoPin);
#endif
  horizontalMinAngle = (hMinAngle < 0) ? 0 : hMinAngle;
  horizontalMaxAngle = (hMaxAngle > 180) ? 180 : hMaxAngle;
  //
#ifndef __HCPCA9685_ENABLED__
  verticalServo.attach(verticalServoPin);
#endif
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
#ifndef __HCPCA9685_ENABLED__
  return horizontalServo.read();
#else
#if (__RUNNING_LOG_ENABLED__)
  Serial.print("current horizontalServoPos: ");
  Serial.println(horizontalServoPos);
#endif
  return horizontalServoPos;
#endif
}

void PedestalHandler::setHorizontalPosition(int hPos) {
#ifndef __HCPCA9685_ENABLED__
  horizontalServo.write(hPos);
#else
  horizontalServoPos = hPos;
#if (__RUNNING_LOG_ENABLED__)
  Serial.print("new horizontalServoPos: ");
  Serial.print(horizontalServoPos);
#endif
  int horizontalHcpcaPos = map(horizontalServoPos, 0, 180, 10, 450);
#if (__RUNNING_LOG_ENABLED__)
  Serial.print("; horizontalHcpcaPos: ");
  Serial.println(horizontalHcpcaPos);
#endif
  hcpca9685.Servo(horizontalServoPin, horizontalHcpcaPos);
#endif
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
#ifndef __HCPCA9685_ENABLED__
  return verticalServo.read();
#else
  return verticalServoPos;
#endif
}

void PedestalHandler::setVerticalPosition(int vPos) {
#ifndef __HCPCA9685_ENABLED__
  verticalServo.write(vPos);
#else
  verticalServoPos = vPos;
  int verticalHcpcaPos = map(verticalServoPos, 0, 180, 10, 450);
  hcpca9685.Servo(verticalServoPin, verticalHcpcaPos);
#endif
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
    Serial.print(count), Serial.println(" rounds are done!");
    return;
  }
  count++;
  //
  for (int pos = 0; pos <= 180; pos += 5) {
    setHorizontalPosition(pos);
    setVerticalPosition(pos);
    Serial.print("hPos: "), Serial.println(pos);
    delay(100);
  }
  //
  for (int pos = 180; pos >= 0; pos -= 5) {
    setHorizontalPosition(pos);
    setVerticalPosition(pos);
    Serial.print("hPos: "), Serial.println(pos);
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
#if (__RUNNING_LOG_ENABLED__)
    if (debugEnabled) {
      Serial.print("PedestalHandler::");
      Serial.print("changeHorizontalServo() - ");
      Serial.print("hDelta: ");
      Serial.println("0, do nothing");
    }
#endif
    return false;
  }
  int hCurrentPos = getHorizontalPosition();
  int hPos = hCurrentPos;
  //
#if (__RUNNING_LOG_ENABLED__)
  if (debugEnabled) {
    Serial.print("PedestalHandler::");
    Serial.print("changeHorizontalServo() - ");
    Serial.print("hDelta: ");
    Serial.print(hDelta);
    Serial.print("; ");
    Serial.print("hPos: ");
    Serial.println(hPos);
  }
#endif
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
#if (__RUNNING_LOG_ENABLED__)
    if (debugEnabled) {
      Serial.print("PedestalHandler::");
      Serial.print("changeVerticalServo() - ");
      Serial.print("vDelta: ");
      Serial.println("0, do nothing");
    }
#endif
    return false;
  }
  int vCurrentPos = getVerticalPosition();
  int vPos = vCurrentPos;
  //
#if (__RUNNING_LOG_ENABLED__)
  if (debugEnabled) {
    Serial.print("PedestalHandler::");
    Serial.print("changeVerticalServo() - ");
    Serial.print("vDelta: ");
    Serial.print(vDelta);
    Serial.print("; ");
    Serial.print("vPos: ");
    Serial.println(vPos);
  }
#endif
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
#if (__RUNNING_LOG_ENABLED__)
  if (debugEnabled) {
    Serial.print("PedestalHandler::change() - ");
    Serial.print("hDelta: ");
    Serial.print(hDelta);
    Serial.print("; ");
    Serial.print("vDelta: ");
    Serial.print(vDelta);
    Serial.println();
  }
#endif
  int hChanged = changeHorizontalServo(hDelta);
  int vChanged = changeVerticalServo(vDelta);
  return (hChanged != 0) || (vChanged != 0);
}
