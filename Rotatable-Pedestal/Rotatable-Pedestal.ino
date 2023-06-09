#include "PS2_Controller.h"
#include "Pedestal_Handler.h"

PS2Controller ps2Controller;
PedestalHandler pedestalHandler;

void setup() {
  Serial.begin(57600);
  Serial.println("Setup starting ...");
  //
  ps2Controller.begin();
  ps2Controller.onDPadButtonPressed(processDPadButtonPressedEvent);
  //
  pedestalHandler.begin();
  //
  Serial.println("Setup has done!");
}

void loop() {
  bool activated = false;
  //
  if(ps2Controller.hasError()) { //skip loop if no controller found
    ps2Controller.showError();
    Serial.println("Reload ...");
    delay(1000);
    ps2Controller.reload();
    return;
  } else { //DualShock Controller
    activated = ps2Controller.check();
    //
    if (activated) {
      delay(10);
    } else {
      delay(200);
    }
  }
}

void processDPadButtonPressedEvent(uint16_t padButton) {
  bool activated = false;
  //
  // MOVE FORWARD
  if(padButton == PSB_PAD_UP) {
    Serial.println("PSB_PAD_UP is pushed");
    activated = pedestalHandler.verticalServoUp();
  }
  // MOVE BACK
  if(padButton == PSB_PAD_DOWN) {
    Serial.println("PSB_PAD_DOWN is pushed");
    activated = pedestalHandler.verticalServoDown();
  }
  // TURN LEFT
  if(padButton == PSB_PAD_LEFT) {
    Serial.println("PSB_PAD_LEFT is pushed");
    activated = pedestalHandler.horizontalServoLeft();
  }
  // TURN RIGHT
  if(padButton == PSB_PAD_RIGHT) {
    Serial.println("PSB_PAD_RIGHT is pushed");
    activated = pedestalHandler.horizontalServoRight();
  }
}
