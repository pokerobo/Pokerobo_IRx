#include "Commons.h"

// #include "IR_Controller.h"

#if (CONTROLLER == CONTROLLER_PS2)
#include "PS2_Controller.h"
#endif

#if (CONTROLLER == CONTROLLER_RF24)
#include "RF24_Controller.h"
#endif

#include "Pedestal_Handler.h"

// IRController irController;
#if (CONTROLLER == CONTROLLER_PS2)
PS2Controller ps2Controller;
#endif

#if (CONTROLLER == CONTROLLER_RF24)
RF24Controller rf24Controller;
#endif

PedestalHandler pedestalHandler0(0, 1);
PedestalHandler pedestalHandler1(4, 5);
PedestalHandler pedestalHandler3(8, 9);

void setup() {
  while (!Serial) {// Wait for the serial connection to be establised.
    delay(100);
  }
  Serial.begin(57600);
  Serial.print("main() - ");
  Serial.println("Setup starting");

  PedestalHandler::init();
  pedestalHandler0.begin(30, 120, 30, 90);
  pedestalHandler1.begin(30, 120, 30, 90);
  pedestalHandler3.begin(30, 120, 30, 90);

#if (CONTROLLER == CONTROLLER_PS2)
  ps2Controller.begin();

  ps2Controller.onStartButtonPressed(processStartButtonPressedEvent);

  ps2Controller.onDPadUpButtonPressed(processDPadUpButtonPressedEvent);
  ps2Controller.onDPadRightButtonPressed(processDPadRightButtonPressedEvent);
  ps2Controller.onDPadDownButtonPressed(processDPadDownButtonPressedEvent);
  ps2Controller.onDPadLeftButtonPressed(processDPadLeftButtonPressedEvent);

  ps2Controller.onLeftJoystickChanged(processLeftJoystickChangeEvent);
  ps2Controller.onRightJoystickChanged(processRightJoystickChangeEvent);
#endif

#if (CONTROLLER == CONTROLLER_RF24)
  rf24Controller.begin();
  rf24Controller.onLeftJoystickChanged(processLeftJoystickChangeEvent);
#endif

  //
  // irController.begin();
  //
  Serial.print("main() - ");
  Serial.println("Setup has done!");
}

void loop() {
#if (CONTROLLER == CONTROLLER_PS2)
  delay(getDelayAmount(ps2Controller.loop()));
#endif
#if (CONTROLLER == CONTROLLER_RF24)
  delay(getDelayAmount(rf24Controller.loop()));
#endif
}

uint32_t getDelayAmount(int status) {
  if (status >= 1) {
    return 10;
  } else {
    return 50;
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

void processDPadUpButtonPressedEvent() {
  processDPadUpButtonPressedEventFor(pedestalHandler0);
  processDPadUpButtonPressedEventFor(pedestalHandler1);
  processDPadUpButtonPressedEventFor(pedestalHandler3);
#ifdef __RUNNING_LOG_ENABLED__
  Serial.print("main() - "), Serial.print("UP"), Serial.println(" is pushed");
#endif
}

void processDPadUpButtonPressedEventFor(PedestalHandler pedestalHandler) {
  bool activated = false;
  activated = pedestalHandler.verticalServoUp();
}

void processDPadRightButtonPressedEvent() {
  processDPadRightButtonPressedEventFor(pedestalHandler0);
  processDPadRightButtonPressedEventFor(pedestalHandler1);
  processDPadRightButtonPressedEventFor(pedestalHandler3);
#ifdef __RUNNING_LOG_ENABLED__
  Serial.print("main() - "), Serial.print("RIGHT"), Serial.println(" is pushed");
#endif
}

void processDPadRightButtonPressedEventFor(PedestalHandler pedestalHandler) {
  bool activated = false;
  activated = pedestalHandler.horizontalServoRight();
}

void processDPadDownButtonPressedEvent() {
  processDPadDownButtonPressedEventFor(pedestalHandler0);
  processDPadDownButtonPressedEventFor(pedestalHandler1);
  processDPadDownButtonPressedEventFor(pedestalHandler3);
#ifdef __RUNNING_LOG_ENABLED__
  Serial.print("main() - "), Serial.print("DOWN"), Serial.println(" is pushed");
#endif
}

void processDPadDownButtonPressedEventFor(PedestalHandler pedestalHandler) {
  bool activated = false;
  activated = pedestalHandler.verticalServoDown();
}

void processDPadLeftButtonPressedEvent() {
  processDPadLeftButtonPressedEventFor(pedestalHandler0);
  processDPadLeftButtonPressedEventFor(pedestalHandler1);
  processDPadLeftButtonPressedEventFor(pedestalHandler3);
#ifdef __RUNNING_LOG_ENABLED__
  Serial.print("main() - "), Serial.print("LEFT"), Serial.println(" is pushed");
#endif
}

void processDPadLeftButtonPressedEventFor(PedestalHandler pedestalHandler) {
  bool activated = false;
  activated = pedestalHandler.horizontalServoLeft();
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
    Serial.print("main() - ");
    Serial.println("processLeftJoystickChangeEvent() is invoked");
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
