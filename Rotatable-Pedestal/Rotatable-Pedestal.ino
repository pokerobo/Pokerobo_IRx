#include "PS2_Controller.h"

#include <Servo.h>

#define HORIZONTAL_SERVO_PIN 8
#define VERTICAL_SERVO_PIN 9

Servo horizontalServo;
Servo verticalServo;

int count = 0;

void setup() {
  Serial.begin(57600);
  Serial.println("Starting ...");
  //
  initializePS2();
  //
  initServos();
  //
  Serial.println("Bootstrap has done!");
}

void loop() {
  testServos();
  //
  if(type != 1) { //skip loop if no controller found
    Serial.println("Error, terminated!");
    return;
  } else { //DualShock Controller
    ps2x.read_gamepad(false, vibrate); // disable vibration of the controller
    //
    // Perform movements based on D-pad buttons
    //
    // MOVE FORWARD
    if(ps2x.Button(PSB_PAD_UP)) {
      Serial.println("PSB_PAD_UP is pushed");
      verticalServoUp();
    }
    // MOVE BACK
    if(ps2x.Button(PSB_PAD_DOWN)){
      Serial.println("PSB_PAD_DOWN is pushed");
      verticalServoDown();
    }
    // TURN LEFT
    if(ps2x.Button(PSB_PAD_LEFT)){
      Serial.println("PSB_PAD_LEFT is pushed");
      horizontalServoLeft();
    }
    // TURN RIGHT
    if(ps2x.Button(PSB_PAD_RIGHT)){
      Serial.println("PSB_PAD_RIGHT is pushed");
      horizontalServoRight();
    }
  }
  //
  delay(500);
}

bool verticalServoUp() {
  int vPos = verticalServo.read();
  if (vPos >= 180) {
    return false;
  }
  vPos += 5;
  if (vPos > 180) {
    vPos = 180;
  }
  verticalServo.write(vPos);
  return true;
}

bool verticalServoDown() {
  int vPos = verticalServo.read();
  if (vPos <= 0) {
    return false;
  }
  vPos -= 5;
  if (vPos < 0) {
    vPos = 0;
  }
  verticalServo.write(vPos);
  return true;
}

bool horizontalServoLeft() {
  int hPos = horizontalServo.read();
  if (hPos >= 180) {
    return false;
  }
  hPos += 5;
  if (hPos > 180) {
    hPos = 180;
  }
  horizontalServo.write(hPos);
  return true;
}

bool horizontalServoRight() {
  int hPos = horizontalServo.read();
  if (hPos <= 0) {
    return false;
  }
  hPos -= 5;
  if (hPos < 0) {
    hPos = 0;
  }
  horizontalServo.write(hPos);
  return true;
}

void initServos() {
  horizontalServo.attach(HORIZONTAL_SERVO_PIN);
  horizontalServo.write(0);
  verticalServo.attach(VERTICAL_SERVO_PIN);
  verticalServo.write(0);
}

void testServos() {
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
