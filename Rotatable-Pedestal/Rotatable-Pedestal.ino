#include "PS2_Controller.h"
#include "Servo_Handler.h"

void setup() {
  Serial.begin(57600);
  Serial.println("Setup starting ...");
  //
  initializePS2();
  //
  initServos();
  //
  Serial.println("Setup has done!");
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
