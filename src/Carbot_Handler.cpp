#include "Carbot_Handler.h"

// Motor A connections

#define IN_2    8
#define IN_1    7
#define EN_A    6

// Motor B connections
#define IN_3    5
#define IN_4    4
#define EN_B    3

#define BOUND_X    40
#define BOUND_Y    40

int CarbotHandler::begin() {
  // Set all the motor control pins to outputs
	pinMode(EN_A, OUTPUT);
	pinMode(EN_B, OUTPUT);
	pinMode(IN_1, OUTPUT);
	pinMode(IN_2, OUTPUT);
	pinMode(IN_3, OUTPUT);
	pinMode(IN_4, OUTPUT);
	
	// Turn off motors - Initial state
	digitalWrite(IN_1, LOW);
	digitalWrite(IN_2, LOW);
	digitalWrite(IN_3, LOW);
	digitalWrite(IN_4, LOW);
}

bool CarbotHandler::isActive() {
  return _active;
}

void CarbotHandler::turnOn() {
  _active = true;
#if __CARBOT_RUNNING_LOG__
  debugLog("CarbotHandler", "::", "turnOn", "()");
#endif
}

void CarbotHandler::turnOff() {
#if __CARBOT_RUNNING_LOG__
  debugLog("CarbotHandler", "::", "turnOff", "()");
#endif
  stop();
  _active = false;
}

void CarbotHandler::flip() {
  if (isActive()) {
    turnOff();
  } else {
    turnOn();
  }
}

void CarbotHandler::move(int x, int y, int coeff=1, bool rotatable=false) {
  uint8_t in1Val = LOW;
  uint8_t in2Val = LOW;
  uint8_t in3Val = LOW;
  uint8_t in4Val = LOW;
  int enaVal = 0;
  int enbVal = 0;

  if (!_active) {
    x = y = 0;
  }

  if (y > BOUND_Y) {
    in1Val = in3Val = HIGH;
    if (x < -BOUND_X) {
      int r = min(abs(x), abs(y));
      int dx = r * coeff / 10;
      enaVal = abs(y) - (r - dx);
      enbVal = abs(y) - dx;
    } else if (x >= -BOUND_X && x <= BOUND_X) {
      enaVal = enbVal = abs(y);
    } else {
      int r = min(abs(x), abs(y));
      int dx = r * coeff / 10;
      enaVal = abs(y) - dx;
      enbVal = abs(y) - (r - dx);
    }
  } else if (y <= BOUND_Y && y >= -BOUND_Y) {
    stop();
  } else {
    in2Val = in4Val = HIGH;
    if (x < -BOUND_X) {
      int r = min(abs(x), abs(y));
      int dx = r * coeff / 10;
      enaVal = abs(y) - (r - dx);
      enbVal = abs(y) - dx;
    } else if (x >= -BOUND_X && x <= BOUND_X) {
      enaVal = enbVal = abs(y);
    } else {
      int r = min(abs(x), abs(y));
      int dx = r * coeff / 10;
      enaVal = abs(y) - dx;
      enbVal = abs(y) - (r - dx);
    }
  }

  enaVal = max(enaVal, 0);
  enbVal = max(enbVal, 0);

#if __CARBOT_RUNNING_LOG__
  char num_[7];
  debugLog(" - ", "active", ": ", _active ? "On" : "Off");
  debugLog(" - ", "IN_1", ": ", itoa(in1Val, num_, 10));
  debugLog(" - ", "IN_2", ": ", itoa(in2Val, num_, 10));
  debugLog(" - ", "IN_3", ": ", itoa(in3Val, num_, 10));
  debugLog(" - ", "IN_4", ": ", itoa(in4Val, num_, 10));

  debugLog(" - ", "EN_A", ": ", itoa(enaVal, num_, 10));
  debugLog(" - ", "EN_B", ": ", itoa(enbVal, num_, 10));
#endif

  enaVal = map(enaVal, 0, 128, 128, 256);
  enbVal = map(enbVal, 0, 128, 128, 256);

  digitalWrite(IN_1, in1Val);
  digitalWrite(IN_2, in2Val);
  digitalWrite(IN_3, in3Val);
  digitalWrite(IN_4, in4Val);

  analogWrite(EN_A, enaVal);
  analogWrite(EN_B, enbVal);
}

void CarbotHandler::stop() {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, LOW);
  analogWrite(EN_A, 0);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, LOW);
  analogWrite(EN_B, 0);

#if __CARBOT_RUNNING_LOG__
  debugLog("CarbotHandler", "::", "stop", "()");
#endif
}

void CarbotHandler::moveForward(int speed=DEFAULT_SPEED) {
  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(EN_B, speed);

  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(EN_A, speed);


  #if __RUNNING_LOG_ENABLED__
  Serial.println("Move forward");
  #endif
}

void CarbotHandler::turnLeft(int coeff=DEFAULT_COEFF, int speed=DEFAULT_SPEED) {
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(EN_A, speed/coeff);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(EN_B, speed);
  #if __RUNNING_LOG_ENABLED__
  Serial.println("Move forward and left");
  #endif
}

void CarbotHandler::turnRight(int coeff=DEFAULT_COEFF, int speed=DEFAULT_SPEED) {
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(EN_A, speed);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(EN_B, speed/coeff);
  #if __RUNNING_LOG_ENABLED__
  Serial.println("Move forward and right");
  #endif
}

void CarbotHandler::moveBack(int speed=DEFAULT_SPEED) {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(EN_A, speed);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(EN_B, speed);
  #if __RUNNING_LOG_ENABLED__
  Serial.println("Move back");
  #endif
}

void CarbotHandler::backLeft(int coeff=DEFAULT_COEFF, int speed=DEFAULT_SPEED) {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(EN_A, speed/coeff);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(EN_B, speed);
  #if __RUNNING_LOG_ENABLED__
  Serial.println("Back to left");
  #endif
}

void CarbotHandler::backRight(int coeff=DEFAULT_COEFF, int speed=DEFAULT_SPEED) {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(EN_A, speed);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(EN_B, speed/coeff);
  #if __RUNNING_LOG_ENABLED__
  Serial.println("Move forward and right");
  #endif
}

void CarbotHandler::rotateLeft(int speed=DEFAULT_SPEED) {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(EN_A, speed);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(EN_B, speed);
  #if __RUNNING_LOG_ENABLED__
  Serial.println("Rotate to left");
  #endif
}

void CarbotHandler::rotateRight(int speed=DEFAULT_SPEED) {
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(EN_A, speed);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(EN_B, speed);
  #if __RUNNING_LOG_ENABLED__
  Serial.println("Rotate to right");
  #endif
}
