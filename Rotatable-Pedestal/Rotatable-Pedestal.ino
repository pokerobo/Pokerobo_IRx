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
  bool activated = false;
  // testServos();
  //
  if(errorCode != 0) { //skip loop if no controller found
    if (!errorDisplayed) {
      Serial.println("Error, terminated!");
      errorDisplayed = true;
    }
    Serial.println("Reload ...");
    delay(1000);
    initializePS2();
    return;
  } else { //DualShock Controller
    ps2x.read_gamepad(false, vibrate); // disable vibration of the controller
    //
    // Perform movements based on D-pad buttons
    //
    // MOVE FORWARD
    if(ps2x.Button(PSB_PAD_UP)) {
      Serial.println("PSB_PAD_UP is pushed");
      activated = verticalServoUp();
    }
    // MOVE BACK
    if(ps2x.Button(PSB_PAD_DOWN)){
      Serial.println("PSB_PAD_DOWN is pushed");
      activated = verticalServoDown();
    }
    // TURN LEFT
    if(ps2x.Button(PSB_PAD_LEFT)){
      Serial.println("PSB_PAD_LEFT is pushed");
      activated = horizontalServoLeft();
    }
    // TURN RIGHT
    if(ps2x.Button(PSB_PAD_RIGHT)){
      Serial.println("PSB_PAD_RIGHT is pushed");
      activated = horizontalServoRight();
    }
    //
    if (activated) {
      delay(10);
    } else {
      delay(200);
    }
  }
}
