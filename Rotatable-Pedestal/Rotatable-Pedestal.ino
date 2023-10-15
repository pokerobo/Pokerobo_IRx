#include "Commons.h"

#define PEDESTALS_MAX   8

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

PedestalHandler pedestalHandler1(4, 5);
PedestalHandler pedestalHandler3(8, 9);

PedestalHandler* pedestalHandlers[PEDESTALS_MAX] = {
  &pedestalHandler1,
  &pedestalHandler3,
};

int pedestalsTotal = PEDESTALS_MAX;

void setup() {
  while (!Serial) {// Wait for the serial connection to be establised.
    delay(100);
  }
  Serial.begin(57600);

#if __LOADING_LOG_ENABLED__
  Serial.print("main()"), Serial.print(" - "), Serial.println("Setup starting");
#endif

  for (int i=0; i<PEDESTALS_MAX; i++) {
    if (pedestalHandlers[i] != NULL) {
      pedestalsTotal = (i + 1);
    }
  }
#if __LOADING_LOG_ENABLED__
  Serial.print("main()"), Serial.print(" - "), Serial.print("total: "), Serial.println(pedestalsTotal);
#endif

  PedestalHandler::init();

  for (int i=0; i<pedestalsTotal; i++) {
    if (pedestalHandlers[i] == NULL) continue;
    pedestalHandlers[i]->begin(30, 120, 30, 90);
  }

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

  rf24Controller.onStartButtonPressed(processStartButtonPressedEvent);
  rf24Controller.onAnalogButtonPressed(processAnalogButtonPressedEvent);

  rf24Controller.onDPadUpButtonPressed(processDPadUpButtonPressedEvent);
  rf24Controller.onDPadRightButtonPressed(processDPadRightButtonPressedEvent);
  rf24Controller.onDPadDownButtonPressed(processDPadDownButtonPressedEvent);
  rf24Controller.onDPadLeftButtonPressed(processDPadLeftButtonPressedEvent);

  rf24Controller.onLeftJoystickChanged(processLeftJoystickChangeEvent);
#endif

  //
  // irController.begin();
  //
#if __LOADING_LOG_ENABLED__
  Serial.print("main()"), Serial.print(" - "), Serial.println("Setup has done!");
#endif
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
    return 5;
  } else {
    return 10;
  }
}

void processStartButtonPressedEvent() {
  for (int i=0; i<pedestalsTotal; i++) {
    processStartButtonPressedEventFor(pedestalHandlers[i]);
  }
}

void processStartButtonPressedEventFor(PedestalHandler *pedestalHandler) {
  if (pedestalHandler == NULL) return;
  pedestalHandler->reset();
}

void processAnalogButtonPressedEvent() {
  for (int i=0; i<pedestalsTotal; i++) {
    processAnalogButtonPressedEventFor(pedestalHandlers[i]);
  }
}

void processAnalogButtonPressedEventFor(PedestalHandler *pedestalHandler) {
  if (pedestalHandler == NULL) return;
  // do something here
}

void processDPadUpButtonPressedEvent() {
  for (int i=0; i<pedestalsTotal; i++) {
    processDPadUpButtonPressedEventFor(pedestalHandlers[i]);
  }
#if __RUNNING_LOG_ENABLED__
  Serial.print("main()"), Serial.print(" - "), Serial.print("UP"), Serial.println(" is pushed");
#endif
}

void processDPadUpButtonPressedEventFor(PedestalHandler *pedestalHandler) {
  if (pedestalHandler == NULL) return;
  pedestalHandler->verticalServoUp();
}

void processDPadRightButtonPressedEvent() {
  for (int i=0; i<pedestalsTotal; i++) {
    processDPadRightButtonPressedEventFor(pedestalHandlers[i]);
  }
#if __RUNNING_LOG_ENABLED__
  Serial.print("main()"), Serial.print(" - "), Serial.print("RIGHT"), Serial.println(" is pushed");
#endif
}

void processDPadRightButtonPressedEventFor(PedestalHandler *pedestalHandler) {
  if (pedestalHandler == NULL) return;
  pedestalHandler->horizontalServoRight();
}

void processDPadDownButtonPressedEvent() {
  for (int i=0; i<pedestalsTotal; i++) {
    processDPadDownButtonPressedEventFor(pedestalHandlers[i]);
  }
#if __RUNNING_LOG_ENABLED__
  Serial.print("main()"), Serial.print(" - "), Serial.print("DOWN"), Serial.println(" is pushed");
#endif
}

void processDPadDownButtonPressedEventFor(PedestalHandler *pedestalHandler) {
  if (pedestalHandler == NULL) return;
  pedestalHandler->verticalServoDown();
}

void processDPadLeftButtonPressedEvent() {
  for (int i=0; i<pedestalsTotal; i++) {
    processDPadLeftButtonPressedEventFor(pedestalHandlers[i]);
  }
#if __RUNNING_LOG_ENABLED__
  Serial.print("main()"), Serial.print(" - "), Serial.print("LEFT"), Serial.println(" is pushed");
#endif
}

void processDPadLeftButtonPressedEventFor(PedestalHandler *pedestalHandler) {
  if (pedestalHandler == NULL) return;
  pedestalHandler->horizontalServoLeft();
}

void processLeftJoystickChangeEvent(int nJoyX, int nJoyY) {
  for (int i=0; i<pedestalsTotal; i++) {
    processLeftJoystickChangeEventFor(pedestalHandlers[i], nJoyX, nJoyY);
  }
}

void processLeftJoystickChangeEventFor(PedestalHandler *pedestalHandler, int nJoyX, int nJoyY) {
  if (pedestalHandler == NULL) return;
  bool changed = pedestalHandler->change(nJoyX, nJoyY);
  if (changed) {
#if __RUNNING_LOG_ENABLED__
    Serial.print("main()"), Serial.print(" - "), Serial.println("processLeftJoystickChangeEvent() is invoked");
#endif
  }
}

void processRightJoystickChangeEvent(int nJoyX, int nJoyY) {
  for (int i=0; i<pedestalsTotal; i++) {
    processRightJoystickChangeEventFor(pedestalHandlers[i], nJoyX, nJoyY);
  }
}

void processRightJoystickChangeEventFor(PedestalHandler *pedestalHandler, int nJoyX, int nJoyY) {
  if (pedestalHandler == NULL) return;
  // do something here
}
