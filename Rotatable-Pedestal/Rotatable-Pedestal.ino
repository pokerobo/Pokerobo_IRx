// #include "IR_Controller.h"
#include "PS2_Controller.h"
#include "Pedestal_Handler.h"

// IRController irController;
PS2Controller ps2Controller;
PedestalHandler pedestalHandler;

void setup() {
  Serial.begin(57600);
  while (!Serial)  // Wait for the serial connection to be establised.
    delay(100);
  Serial.println("Setup starting ...");
  //
  // irController.begin();
  //
  ps2Controller.begin();
  ps2Controller.onDPadButtonPressed(processDPadButtonPressedEvent);
  ps2Controller.onLeftJoystickChanged(processLeftJoystickChangeEvent);
  //
  pedestalHandler.begin();
  //
  Serial.println("Setup has done!");
}

void loop() {
  int changed = ps2Controller.loop();
  if (changed >= 1) {
    delay(10);
  } else {
    delay(200);
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

void processLeftJoystickChangeEvent(int nJoyLX, int nJoyLY) {
  bool hChanged = pedestalHandler.changeHorizontalServo(nJoyLX);
  bool vChanged = pedestalHandler.changeVerticalServo(nJoyLY);
  //
  return hChanged || vChanged;
}
