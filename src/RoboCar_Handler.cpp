#include "RoboCar_Handler.h"

// Motor A connections

#define IN_2    8
#define IN_1    7
#define EN_A    6

// Motor B connections
#define IN_3    5
#define IN_4    4
#define EN_B    3

#define ROBOCAR_DEADZONE_BOUND_X    52
#define ROBOCAR_DEADZONE_BOUND_Y    52

int RoboCarHandler::begin() {
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

bool RoboCarHandler::isActive() {
  return _active;
}

void RoboCarHandler::turnOn() {
  _active = true;
#if __ROBOCAR_RUNNING_LOG__
  debugLog("RoboCarHandler", "::", "turnOn", "()");
#endif
}

void RoboCarHandler::turnOff() {
#if __ROBOCAR_RUNNING_LOG__
  debugLog("RoboCarHandler", "::", "turnOff", "()");
#endif
  stop();
  _active = false;
}

void RoboCarHandler::flip() {
  if (isActive()) {
    turnOff();
  } else {
    turnOn();
  }
}

void RoboCarHandler::move(int x, int y, int coeff=1, bool rotatable=false) {
  uint8_t in1Val = LOW;
  uint8_t in2Val = LOW;
  uint8_t in3Val = LOW;
  uint8_t in4Val = LOW;
  int enaVal = 0;
  int enbVal = 0;

  if (!_active) {
    x = y = 0;
  }

  if (y > ROBOCAR_DEADZONE_BOUND_Y) {
    in1Val = in3Val = HIGH;
    if (x < -ROBOCAR_DEADZONE_BOUND_X) {
      int r = min(abs(x), abs(y));
      int dx = r * coeff / 10;
      enaVal = abs(y) - (r - dx);
      enbVal = abs(y) - dx;
    } else if (x >= -ROBOCAR_DEADZONE_BOUND_X && x <= ROBOCAR_DEADZONE_BOUND_X) {
      enaVal = enbVal = abs(y);
    } else {
      int r = min(abs(x), abs(y));
      int dx = r * coeff / 10;
      enaVal = abs(y) - dx;
      enbVal = abs(y) - (r - dx);
    }
  } else if (y <= ROBOCAR_DEADZONE_BOUND_Y && y >= -ROBOCAR_DEADZONE_BOUND_Y) {
    stop();
  } else {
    in2Val = in4Val = HIGH;
    if (x < -ROBOCAR_DEADZONE_BOUND_X) {
      int r = min(abs(x), abs(y));
      int dx = r * coeff / 10;
      enaVal = abs(y) - (r - dx);
      enbVal = abs(y) - dx;
    } else if (x >= -ROBOCAR_DEADZONE_BOUND_X && x <= ROBOCAR_DEADZONE_BOUND_X) {
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

#if __ROBOCAR_RUNNING_LOG__
  char num_[7];
  debugLog(" - ", "active", ": ", _active ? "On" : "Off");
  debugLog(" - ", "IN_1", ": ", itoa(in1Val, num_, 10));
  debugLog(" - ", "IN_2", ": ", itoa(in2Val, num_, 10));
  debugLog(" - ", "IN_3", ": ", itoa(in3Val, num_, 10));
  debugLog(" - ", "IN_4", ": ", itoa(in4Val, num_, 10));

  debugLog(" - ", "EN_A", ": ", itoa(enaVal, num_, 10));
  debugLog(" - ", "EN_B", ": ", itoa(enbVal, num_, 10));
#endif

  digitalWrite(IN_1, in1Val);
  digitalWrite(IN_2, in2Val);
  digitalWrite(IN_3, in3Val);
  digitalWrite(IN_4, in4Val);

  analogWrite(EN_A, enaVal);
  analogWrite(EN_B, enbVal);
}

void RoboCarHandler::stop() {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, LOW);
  analogWrite(EN_A, 0);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, LOW);
  analogWrite(EN_B, 0);

#if __ROBOCAR_RUNNING_LOG__
  debugLog("RoboCarHandler", "::", "stop", "()");
#endif
}

void RoboCarHandler::moveForward(int speed=DEFAULT_SPEED) {
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

void RoboCarHandler::turnLeft(int coeff=DEFAULT_COEFF, int speed=DEFAULT_SPEED) {
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

void RoboCarHandler::turnRight(int coeff=DEFAULT_COEFF, int speed=DEFAULT_SPEED) {
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

void RoboCarHandler::moveBack(int speed=DEFAULT_SPEED) {
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

void RoboCarHandler::backLeft(int coeff=DEFAULT_COEFF, int speed=DEFAULT_SPEED) {
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

void RoboCarHandler::backRight(int coeff=DEFAULT_COEFF, int speed=DEFAULT_SPEED) {
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

void RoboCarHandler::rotateLeft(int speed=DEFAULT_SPEED) {
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

void RoboCarHandler::rotateRight(int speed=DEFAULT_SPEED) {
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
