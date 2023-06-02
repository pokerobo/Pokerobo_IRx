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
  if(error == 1) { //skip loop if no controller found
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
    }
    // MOVE BACK
    if(ps2x.Button(PSB_PAD_DOWN)){
      Serial.println("PSB_PAD_UP is pushed");
    }
    // TURN LEFT
    if(ps2x.Button(PSB_PAD_LEFT)){
      Serial.println("PSB_PAD_LEFT is pushed");
    }
    // TURN RIGHT
    if(ps2x.Button(PSB_PAD_RIGHT)){
      Serial.println("PSB_PAD_RIGHT is pushed");
    }
  }
  //
  delay(500);
}

void initServos() {
  horizontalServo.attach(HORIZONTAL_SERVO_PIN);
  verticalServo.attach(VERTICAL_SERVO_PIN);
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
