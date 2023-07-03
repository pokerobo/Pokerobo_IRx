// #include "IR_Controller.h"
#include "PS2_Controller.h"
#include "Pedestal_Handler.h"

// IRController irController;
PS2Controller ps2Controller;

PedestalHandler pedestalHandler0(0, 1);
PedestalHandler pedestalHandler1(4, 5);
PedestalHandler pedestalHandler3(8, 9);

void setup() {
  while (!Serial) {// Wait for the serial connection to be establised.
    delay(100);
  }
  Serial.begin(57600);
  Serial.println("main() - Setup starting ...");
  //
  PedestalHandler::init();
  pedestalHandler0.begin(30, 120, 30, 90);
  pedestalHandler1.begin(30, 120, 30, 90);
  pedestalHandler3.begin(30, 120, 30, 90);
  //
  ps2Controller.begin();
  ps2Controller.onStartButtonPressed(processStartButtonPressedEvent);
  ps2Controller.onDPadButtonPressed(processDPadButtonPressedEvent);
  ps2Controller.onLeftJoystickChanged(processLeftJoystickChangeEvent);
  // ps2Controller.onRightJoystickChanged(processRightJoystickChangeEvent);
  //
  // irController.begin();
  //
  Serial.println("main() - Setup has done!");
}

void loop() {
  delay(getDelayAmount(ps2Controller.loop()));
}

uint32_t getDelayAmount(int status) {
  if (status >= 1) {
    return 1;
  } else {
    return 5;
  }
}

void processStartButtonPressedEvent() {
  processStartButtonPressedEventFor(pedestalHandler0);
  processStartButtonPressedEventFor(pedestalHandler1);
  processStartButtonPressedEventFor(pedestalHandler3);
}

void processStartButtonPressedEventFor(PedestalHandler pedestalHandler) {
  pedestalHandler.reset();
}

void processDPadButtonPressedEvent(uint16_t padButton) {
  processDPadButtonPressedEventFor(pedestalHandler0, padButton);
  processDPadButtonPressedEventFor(pedestalHandler1, padButton);
  processDPadButtonPressedEventFor(pedestalHandler3, padButton);
}

void processDPadButtonPressedEventFor(PedestalHandler pedestalHandler, uint16_t padButton) {
  bool activated = false;
  //
  // MOVE FORWARD
  if(padButton == PSB_PAD_UP) {
    Serial.println("main() - PSB_PAD_UP is pushed");
    activated = pedestalHandler.verticalServoUp();
  }
  // MOVE BACK
  if(padButton == PSB_PAD_DOWN) {
    Serial.println("main() - PSB_PAD_DOWN is pushed");
    activated = pedestalHandler.verticalServoDown();
  }
  // TURN LEFT
  if(padButton == PSB_PAD_LEFT) {
    Serial.println("main() - PSB_PAD_LEFT is pushed");
    activated = pedestalHandler.horizontalServoLeft();
  }
  // TURN RIGHT
  if(padButton == PSB_PAD_RIGHT) {
    Serial.println("main() - PSB_PAD_RIGHT is pushed");
    activated = pedestalHandler.horizontalServoRight();
  }
}

void processLeftJoystickChangeEvent(int nJoyX, int nJoyY) {
  processLeftJoystickChangeEventFor(&pedestalHandler0, nJoyX, nJoyY);
  processLeftJoystickChangeEventFor(&pedestalHandler1, nJoyX, nJoyY);
  processLeftJoystickChangeEventFor(&pedestalHandler3, nJoyX, nJoyY);
}

void processLeftJoystickChangeEventFor(PedestalHandler *pedestalHandler, int nJoyX, int nJoyY) {
  bool changed = pedestalHandler->change(nJoyX, nJoyY);
  if (changed) {
#ifdef __RUNNING_LOG_ENABLED__
    Serial.println("main() - processLeftJoystickChangeEvent() is invoked");
#endif
  }
}

void processRightJoystickChangeEvent(int nJoyX, int nJoyY) {
  processRightJoystickChangeEventFor(pedestalHandler0, nJoyX, nJoyY);
  processRightJoystickChangeEventFor(pedestalHandler1, nJoyX, nJoyY);
  processRightJoystickChangeEventFor(pedestalHandler3, nJoyX, nJoyY);
}

void processRightJoystickChangeEventFor(PedestalHandler pedestalHandler, int nJoyX, int nJoyY) {
  // do nothing
}
