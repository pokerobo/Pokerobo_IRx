// #include "IR_Controller.h"
#include "PS2_Controller.h"
#include "Pedestal_Handler.h"

// IRController irController;
PS2Controller ps2Controller;
PedestalHandler pedestalHandler;

void setup() {
  while (!Serial) {// Wait for the serial connection to be establised.
    delay(100);
  }
  Serial.begin(57600);
  Serial.println("Setup starting ...");
  //
  pedestalHandler.begin();
  //
  ps2Controller.begin();
  ps2Controller.onDPadButtonPressed(processDPadButtonPressedEvent);
  ps2Controller.onLeftJoystickChanged(processLeftJoystickChangeEvent);
  ps2Controller.onRightJoystickChanged(processRightJoystickChangeEvent);
  //
  // irController.begin();
  //
  Serial.println("Setup has done!");
}

void loop() {
  delay(getDelayAmount(ps2Controller.loop()));
}

uint32_t getDelayAmount(int status) {
  if (status >= 1) {
    return 10;
  } else {
    return 200;
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
  return pedestalHandler.change(nJoyLX, nJoyLY);
}

void processRightJoystickChangeEvent(int nJoyRX, int nJoyRY) {
}
