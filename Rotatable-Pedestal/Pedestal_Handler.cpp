#include "Pedestal_Handler.h"

#if __HCPCA9685_ENABLED__
/* I2C slave address for the device/module. For the HCMODU0097 the default I2C address is 0x40 */
#define  I2C_ADDR             0x40

/* Create an instance of the library */
HCPCA9685 hcpca9685(I2C_ADDR);
#endif

static bool PedestalHandler::initialized = false;

static void PedestalHandler::init() {
  if (initialized) return;
  initialized = true;

#if __HCPCA9685_ENABLED__
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
#if __LOADING_LOG_ENABLED__
  if (debugEnabled) {
    char p_[5], sp_[5];
    debugLog("PedestalHandler", "::", "PedestalHandler", "()", " - ",
        "h", "Pin", ": ", itoa(hPin, p_, 10), " => ", itoa(horizontalServoPin, sp_, 10));
    debugLog("PedestalHandler", "::", "PedestalHandler", "()", " - ",
        "v", "Pin", ": ", itoa(vPin, p_, 10), " => ", itoa(verticalServoPin, sp_, 10));
  }
#endif
}

void PedestalHandler::begin(int hMinAngle, int hMaxAngle, int vMinAngle, int vMaxAngle) {
#if __LOADING_LOG_ENABLED__
  if (debugEnabled) {
    char num_[7];
    debugLog("PedestalHandler", "::", "begin", "()", ": ");
    debugLog(" - ", "h", "Min", "Angle", ": ", itoa(hMinAngle, num_, 10));
    debugLog(" - ", "h", "Max", "Angle", ": ", itoa(hMaxAngle, num_, 10));
    debugLog(" - ", "v", "Min", "Angle", ": ", itoa(vMinAngle, num_, 10));
    debugLog(" - ", "v", "Max", "Angle", ": ", itoa(vMaxAngle, num_, 10));
  }
#endif
  //
#if !__HCPCA9685_ENABLED__
  horizontalServo.attach(horizontalServoPin);
#endif
  horizontalMinAngle = (hMinAngle < 0) ? 0 : hMinAngle;
  horizontalMaxAngle = (hMaxAngle > 180) ? 180 : hMaxAngle;
  //
#if !__HCPCA9685_ENABLED__
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
#if !__HCPCA9685_ENABLED__
  return horizontalServo.read();
#else
#if __RUNNING_LOG_ENABLED__
  char num_[7];
  debugLog("old ", "Horizontal", "Servo", "Pos", ": ", itoa(horizontalServoPos, num_, 10));
#endif
  return horizontalServoPos;
#endif
}

void PedestalHandler::setHorizontalPosition(int hPos) {
#if !__HCPCA9685_ENABLED__
  horizontalServo.write(hPos);
#else
  horizontalServoPos = hPos;
  int horizontalHcpcaPos = map(horizontalServoPos, 0, 180, 10, 450);
#if __RUNNING_LOG_ENABLED__
  char n1_[7], n2_[7];
  debugLog("new ", "Horizontal", "Servo", "Pos", ": ", itoa(horizontalServoPos, n1_, 10),
      "; ", "Horizontal", "Hcpca", "Pos", ": ", itoa(horizontalHcpcaPos, n2_, 10));
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
#if !__HCPCA9685_ENABLED__
  return verticalServo.read();
#else
  return verticalServoPos;
#endif
}

void PedestalHandler::setVerticalPosition(int vPos) {
#if !__HCPCA9685_ENABLED__
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
  char n_[7];
  if (count >= 3) {
    debugLog(itoa(count, n_, 10), " rounds are done!");
    return;
  }
  count++;
  //
  for (int pos = 0; pos <= 180; pos += 5) {
    setHorizontalPosition(pos);
    setVerticalPosition(pos);
    debugLog("h", "Pos", ": ", itoa(pos, n_, 10));
    delay(100);
  }
  //
  for (int pos = 180; pos >= 0; pos -= 5) {
    setHorizontalPosition(pos);
    setVerticalPosition(pos);
    debugLog("h", "Pos", ": ", itoa(pos, n_, 10));
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
#if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      debugLog("PedestalHandler", "::", "change", "Horizontal", "Servo", "()", " - ",
          "h", "Delta", ": ", "0, do nothing");
    }
#endif
    return false;
  }
  int hCurrentPos = getHorizontalPosition();
  int hPos = hCurrentPos;
  //
#if __RUNNING_LOG_ENABLED__
  if (debugEnabled) {
    char hDelta_[7], hPos_[7];
    debugLog("PedestalHandler", "::", "change", "Horizontal", "Servo", "()", " - ",
        "h", "Delta", ": ", itoa(hDelta, hDelta_, 10), "; ",
        "h", "Pos", ": ", itoa(hPos, hPos_, 10));
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
#if __RUNNING_LOG_ENABLED__
    if (debugEnabled) {
      debugLog("PedestalHandler", "::", "change", "Vertical", "Servo", "()", " - ",
          "v", "Delta", ": ", "0, do nothing");
    }
#endif
    return false;
  }
  int vCurrentPos = getVerticalPosition();
  int vPos = vCurrentPos;
  //
#if __RUNNING_LOG_ENABLED__
  if (debugEnabled) {
    char vDelta_[7], vPos_[7];
    debugLog("PedestalHandler", "::", "change", "Vertical", "Servo", "()", " - ",
        "v", "Delta", ": ", itoa(vDelta, vDelta_, 10), "; ",
        "v", "Pos", ": ", itoa(vPos, vPos_, 10));
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
#if __RUNNING_LOG_ENABLED__
  if (debugEnabled) {
    char h_[7], v_[7];
    debugLog("PedestalHandler", "::", "change", "()", " - ",
        "h", "Delta", ": ", itoa(hDelta, h_, 10), "; ",
        "v", "Delta", ": ", itoa(vDelta, v_, 10));
  }
#endif
  int hChanged = changeHorizontalServo(hDelta);
  int vChanged = changeVerticalServo(vDelta);
  return (hChanged != 0) || (vChanged != 0);
}
